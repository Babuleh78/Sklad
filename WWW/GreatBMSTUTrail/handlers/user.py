import asyncio
import os
import random
import sqlite3
from aiogram import F, types, Router, Bot
from aiogram.filters import CommandStart, Command, or_f
from klava import reply, inline
from klava.reply import get_keyboard
user_router = Router()
ADMIN_ID = os.getenv('ADMIN')
IS_ADMIN = False
IGNORE_CASE = False
bot = Bot(token = os.getenv('TOKEN'))