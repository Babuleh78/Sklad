import pymysql
from PIL import Image
import tempfile
import os
import io
import base64

from aiogram import F, types, Router, Bot
from aiogram.filters import CommandStart, Command, or_f
from klava import reply, inline
from klava.reply import get_keyboard
from klava.inline import get_callback_btns
user_router = Router()
ADMIN_ID = int(os.getenv('ADMIN'))
IS_ADMIN = False
IGNORE_CASE = False
bot = Bot(token = os.getenv('TOKEN'))
try:
     connection = pymysql.connect(
          host="localhost",
          port = 8888,
          user = "root",
          password= "root",
          database= "project",
    

     )
     print("Успешно подключились")
except Exception as ex:
     print("Ну че опять", ex)
async def get_top():
    try:
        with connection.cursor() as cursor:
            query = "SELECT * FROM user ORDER BY stars DESC"
            cursor.execute(query)
            result = cursor.fetchall()
            return result
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return []

async def ach_users(nickname):
    try:
        with connection.cursor() as cursor:
            query = "SELECT iduser FROM user WHERE usertoken = %s"
            cursor.execute(query, (nickname,))
            result = cursor.fetchall()
            if not result:
                return ["Пользователь не найден!"]
            
            id = result[0][0]
            query1 = "SELECT ach_id, is_open FROM open_ach WHERE user_id = %s"
            cursor.execute(query1, (id,))
            ach_mas = cursor.fetchall()

            achievements = []  

            for i in range(len(ach_mas)):
                aid = ach_mas[i][0]
                io = ach_mas[i][1]
                if io:  
                    querycicle = "SELECT title FROM achivements WHERE idach = %s"
                    cursor.execute(querycicle, (aid,))
                    title_result = cursor.fetchall()
                    print(title_result[0][0])
                    achievements.append(title_result[0][0])

            return achievements  

    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return []  
async def get_image():
    try:
        with connection.cursor() as cursor:
            query = "SELECT* FROM images ORDER BY image_id limit 1"
            cursor.execute(query)
            result = cursor.fetchall()
            
            
            if not result:
                return ["Изображение не найдено!"]
            id, name, url, uid, pid = result[0]
            data = [name, url, pid]
            return data
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return []  
async def good_photo():
    try:
        with connection.cursor() as cursor:
            query = "SELECT * FROM images ORDER BY image_id LIMIT 1"
            cursor.execute(query)
            result = cursor.fetchone() 
            if not result:
                return None 

            id, name, url, uid, pid = result
            query1 = "UPDATE visits SET is_visit = %s WHERE user_id = %s AND place_id = %s"
            cursor.execute(query1, (78, uid, pid))  
            query2 = "DELETE FROM images WHERE image_id = %s"
            cursor.execute(query2, (id,))
            query3 = "SELECT * FROM user WHERE usertoken = %s"
            cursor.execute(query3, (name,))
            user_result = cursor.fetchone()
            connection.commit()
            return user_result  
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return None  
async def bad_photo():
    try:
        with connection.cursor() as cursor:
            query = "SELECT * FROM images ORDER BY image_id LIMIT 1"
            cursor.execute(query)
            result = cursor.fetchone() 
            id, name, url, uid, pid = result
            if not result:
                return None 
            query1 = "UPDATE visits SET is_visit = %s WHERE user_id = %s AND place_id = %s"
            cursor.execute(query1, (0, uid, pid))  
            query2 = "DELETE FROM images WHERE image_id = %s"
            cursor.execute(query2, (id,))
            query3 = "SELECT telegram FROM user WHERE usertoken = %s"
            cursor.execute(query3, (name,))
            tg = cursor.fetchone()
            connection.commit()
            
            return [f"{name}, это че за хуйня, забыл как каша дома пахнет? ", url, tg]
          
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return None  

@user_router.message(Command('top'))
async def process_callback_button(message: types.Message):
    await message.answer("Топ 5 лучших участников")
    top_items = await get_top()
    ans_messages = []
    for i in range(min(len(top_items), 5)):
        data = top_items[i]  
        id, role, count, status = data  
        ans_message = f"{i + 1}. Ник: {role}, Звезды: {count}"
        ans_messages.append(ans_message)
    
    final_message = "\n".join(ans_messages)
    await message.answer(final_message)
@user_router.message(Command('user'))
async def cmd_user(message: types.Message):
    await message.answer("Введите ник пользователя, о котором хотите узнать в формате ИНФ-Пользователь" )
    
@user_router.message(F.text.lower().contains("инф-"))
async def info_panel(message: types.Message):
    nickname = message.text.split('-')[1]
    data = await ach_users(nickname)
    ans_message = "Достижения " + nickname + ":" + "\n"
    result_string = '\n'.join(map(str, data))
    await message.answer(ans_message+result_string)

def base64_to_image(base64_str):
    header, base64_data = base64_str.split(',', 1) 
    image_data = base64.b64decode(base64_data)
    image = Image.open(io.BytesIO(image_data))
    return image
def reg_user(name, id):
    try:
        with connection.cursor() as cursor:
            query = "UPDATE user SET telegram = %s WHERE (usertoken = %s)"
            cursor.execute(query, (id, name))
            connection.commit()  

            if cursor.rowcount > 0:  
                return "Вы успешно вошли!"
            return "Ошибка: пользователь не найден или вы уже зарегистрированы"
            
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return "ОШИБКА"
@user_router.message(Command('start'))
async def send_welcome(message: types.Message):
    keyboard = get_keyboard("Вход", "Топ Пользователей", "Узнать о пользователе")
    await message.answer("Здравствуй боец, выбери, что ты хочешь сделать", reply_markup = keyboard)
@user_router.message(F.text.lower().contains("вход"))
async def vhod(message: types.Message): 
    await message.answer("Введите ваше имя на сайте (какое указали при регистрации) в формате ВХ-Имя")
@user_router.message(F.text.lower().contains("вх-"))
async def vhod(message: types.Message): 
    name = message.text.split('-')[1]

    uid = message.from_user.id
    print(name, uid)
    string = reg_user(name, uid)
    await message.answer(string)
async def vhod(message: types.Message): 
    await message.answer("Введите ваше имя на сайте (какое указали при регистрации) в формате ВХ-Имя")
@user_router.message(F.text.lower().contains("топ пользователей"))
async def vhod(message: types.Message): 
    await message.answer("топ польхов")
@user_router.message(F.text.lower().contains("узнать о пользователе"))
async def vhod(message: types.Message): 
    await message.answer("уна")
@user_router.message(F.text.lower().contains("28 ноября"))
async def admin_panel(message: types.Message):
    print(message.from_user.id)
    if message.from_user.id == ADMIN_ID:
            
            keyboard = get_keyboard("ПРОВЕРКА ФОТОГРАФИЙ","ПОПРОСИТЬ МИЛОСТЫНИ")
            await message.answer( text = "Я у аппарата, начнем работу", reply_markup=keyboard)
        
    else:
        await message.answer("Пасхалко найдено")
@user_router.message(F.text.lower().contains("проверка фотографий"))
async def moneytalks(message: types.Message):
        if message.from_user.id == ADMIN_ID:
            try:
                name, base64_str, pid = await get_image()

                str_message = f'Пользователь {name} прислал вам изображение, он хочет ликвидировать место {pid}. Одобряем?'
                
                header, base64_data = base64_str.split(',', 1)
                decode = base64.b64decode(base64_data)
                img = Image.open(io.BytesIO(decode))

                with tempfile.NamedTemporaryFile(delete=False, suffix='.png') as tmp_file:
                    img.save(tmp_file, format='PNG')
                    tmp_file.seek(0) 
                    photo = types.FSInputFile(tmp_file.name)

                buttons = {
                    "ДА": "yes_callback", 
                    "НЕТ": "no_callback",   
                }
                keyboard = get_callback_btns(btns=buttons)
                await bot.send_photo(chat_id= ADMIN_ID, photo=photo, caption=str_message, reply_markup=keyboard)
                
                os.remove(tmp_file.name)
            except Exception as e:
                print(e)
                await message.answer("Больше нет фотографий для проверки!")

@user_router.callback_query(lambda c: c.data in ["yes_callback", "no_callback"])
async def process_callback(callback_query: types.CallbackQuery):
    if callback_query.data == "yes_callback":
        await bot.answer_callback_query(callback_query.id) 
        res = await good_photo()
        
        id, name, h, p, tg = res
        if not(tg is None):
            await bot.send_message(chat_id=tg, text=f"Поздравляем, {name}! Ликвидация успешна")
        else:
            await bot.send_message(chat_id=ADMIN_ID, text=f"У дурика {name}! нет телеги")
    elif callback_query.data == "no_callback":
        await bot.answer_callback_query(callback_query.id)
        res = await bad_photo()
        mes = res[0]
        base64_str = res[1]
        tg = res[2][0]
        
        header, base64_data = base64_str.split(',', 1)
        decode = base64.b64decode(base64_data)
        img = Image.open(io.BytesIO(decode))

        with tempfile.NamedTemporaryFile(delete=False, suffix='.png') as tmp_file:
            img.save(tmp_file, format='PNG')
            tmp_file.seek(0) 
            photo = types.FSInputFile(tmp_file.name)
        if(not(tg is None)):
            await bot.send_photo(photo = photo, chat_id= tg, caption=mes)
        else:
            await bot.send_photo(chat_id= ADMIN_ID, caption="У дурика нет ТГ, еще и фотки херовые присылает, может его того?")
