import asyncio
from mysql.connector import Error
import os
from dotenv import find_dotenv, load_dotenv
from aiogram import Bot, Dispatcher, types
from aiogram.filters import CommandStart
load_dotenv(find_dotenv())

from handlers.user import user_router, bot
from common.bot_cmds_list import private

ALLOWED_UPDATES = ['message', 'edited_message', 'photo']

dp = Dispatcher() 
dp.include_router(user_router)

async def main() -> None:
    await bot.delete_webhook(drop_pending_updates=True)
    try:
        await bot.set_my_commands(commands=private, scope=types.BotCommandScopeAllPrivateChats())
    except Exception as e:
        print(f"An error occurred: {e}")
    await dp.start_polling(bot, allowed_updates= dp.resolve_used_update_types() ) #запуск бота

asyncio.run(main())