import asyncio
import mysql.connector
from mysql.connector import Error
import os
from dotenv import find_dotenv, load_dotenv
from aiogram import Bot, Dispatcher, types
from aiogram.filters import CommandStart
load_dotenv(find_dotenv())

from handlers.user import user_router
from common.bot_cmds_list import private

ALLOWED_UPDATES = ['message', 'edited_message']

bot = Bot(token = os.getenv('TOKEN'))

dp = Dispatcher() 
dp.include_router(user_router)
config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'root',
    'database': 'project',
    'port': 8888,
    'raise_on_warnings': True,
}

try:
    with mysql.connector.connect(**config) as connection:
        print("Подключение успешно!")
        # Здесь можно выполнять запросы к базе данных
except Error as e:
    print(f"Ошибка подключения: {e}")

async def main() -> None:
    await bot.delete_webhook(drop_pending_updates=True)
    try:
        await bot.set_my_commands(commands=private, scope=types.BotCommandScopeAllPrivateChats())
    except Exception as e:
        print(f"An error occurred: {e}")
    await dp.start_polling(bot, allowed_updates= dp.resolve_used_update_types() ) #запуск бота

asyncio.run(main())