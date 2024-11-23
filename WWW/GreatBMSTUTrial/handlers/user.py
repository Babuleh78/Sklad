import pymysql
from PIL import Image
import tempfile
import os
import io
import base64

from aiogram import F, types, Router, Bot
from aiogram.types import InputFile  
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
            print("Зашли")
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
            query = "SELECT* FROM images ORDER BY image_id limit 1"
            cursor.execute(query)
            result = cursor.fetchone() 
            if not result:
                return
            id, name, url, uid, pid = result
            query1 = "UPDATE visits SET is_visit = %s WHERE user_id = %s AND place_id = %s"
            cursor.execute(query1, (1, uid, pid))  
            connection.commit()  
            return
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return



@user_router.message(or_f(Command('top'), (F.text.lower().contains("топ"))))
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
@user_router.message(or_f(Command('user'), (F.text.lower().contains("польз"))))
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
@user_router.message(Command('start'))
async def send_welcome(message: types.Message):
    keyboard = get_keyboard("Вход", "Топ")
    await message.answer("Здравствуй боец, выбери, что ты хочешь сделать", reply_markup = keyboard)
@user_router.message(F.text.lower().contains("а"))
async def info_panel(message: types.Message):
    print(message.from_user.id)
    if message.from_user.id == ADMIN_ID:
        await message.answer("Будем считать, что ты админ")
        
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
        await message.answer_photo(photo=photo, caption=str_message, reply_markup=keyboard)

        os.remove(tmp_file.name)
    else:
        await message.answer("Соси хуй")

@user_router.callback_query(lambda c: c.data in ["yes_callback", "no_callback"])
async def process_callback(callback_query: types.CallbackQuery):
    if callback_query.data == "yes_callback":
        await bot.answer_callback_query(callback_query.id) 
        await good_photo()
        await bot.send_message(callback_query.from_user.id, "Ликвидация успешна")
    elif callback_query.data == "no_callback":
        await bot.answer_callback_query(callback_query.id)  # Подтверждаем нажатие
        await bot.send_message(callback_query.from_user.id, "Вы выбрали: НЕТ")

@user_router.message(F.text.lower().contains("ш"))
async def info_panel(message: types.Message):
    await message.answer("ЧЕГО ЧЕГО НАХУЙ, ЭТО ЧЕ, ШАШУРА?")