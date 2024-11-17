
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

@user_router.message(F.text.lower().contains("ш"))
async def info_panel(message: types.Message):
    await message.answer("ЧЕГО ЧЕГО НАХУЙ, ЭТО ЧЕ, ШАШУРА?")
@user_router.message(or_f(Command('top'), (F.text.lower().contains("топ"))))
async def process_callback_button(message: types.Message):
    
    await message.answer("Назовись, боец")