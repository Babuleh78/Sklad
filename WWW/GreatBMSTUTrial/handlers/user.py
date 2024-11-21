import pymysql
from PIL import Image
import io
import os
import base64
from aiogram import F, types, Router, Bot
from aiogram.types import InputFile  
from aiogram.filters import CommandStart, Command, or_f
from klava import reply, inline
from klava.reply import get_keyboard
user_router = Router()
ADMIN_ID = os.getenv('ADMIN')
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
            id, name, url = result[0]
            data = [name, url]
            return data
    except Exception as e:
        print("Ошибка выполнения SQL-запроса:", e)
        return []  
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

@user_router.message(F.text.lower().contains("а"))
async def info_panel(message: types.Message):
    await message.answer("Будем считать, что ты админ")
    data = await get_image()  
    name, url = data
    header, base64_data = url.split(',', 1)
    image_data = base64.b64decode(base64_data)
    image = Image.open(io.BytesIO(image_data))
    output_file_path = f"{name}.jpg"
    image.save(output_file_path, format="JPEG", quality=95)  # Вы можете изменить значение качества
   
    await message.answer_photo(photo = "https://cdn1.ozone.ru/s3/multimedia-1-j/7033464451.jpg", caption= f"Пользователь {name}" )
@user_router.message(F.text.lower().contains("ш"))
async def info_panel(message: types.Message):
    await message.answer("ЧЕГО ЧЕГО НАХУЙ, ЭТО ЧЕ, ШАШУРА?")