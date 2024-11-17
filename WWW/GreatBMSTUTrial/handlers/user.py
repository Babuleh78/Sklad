import pymysql
import os
from aiogram import F, types, Router, Bot
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

async def get_top_items():
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
            print("тут")
            query = "SELECT iduser FROM user WHERE usertoken = %s"
            cursor.execute(query, (nickname,))
            result = cursor.fetchall()
            if not result:
                return []
            
            id = result[0][0]
            print(id)
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
@user_router.message(or_f(Command('top'), (F.text.lower().contains("топ"))))
async def process_callback_button(message: types.Message):
    await message.answer("Топ 5 лучших участников")
    top_items = await get_top_items()
    ans_messages = []
    for i in range(min(len(top_items), 5)):
        data = top_items[i]  
        id, role, count, status = data  
        ans_message = f"{i + 1}. Ник: {role}, Звезды: {count}"
        ans_messages.append(ans_message)
    
    final_message = "\n".join(ans_messages)
    await message.answer(final_message)
@user_router.message(or_f(Command('top'), (F.text.lower().contains("топ"))))
async def process_callback_button(message: types.Message):
    data = await ach_users("Админ")
    print(data)
    await message.answer(data[0])
@user_router.message(F.text.lower().contains("ш"))
async def info_panel(message: types.Message):
    await message.answer("ЧЕГО ЧЕГО НАХУЙ, ЭТО ЧЕ, ШАШУРА?")