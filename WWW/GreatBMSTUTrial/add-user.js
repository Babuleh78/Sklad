// const express = require('express');
// const mysql = require('mysql');
// const bodyParser = require('body-parser');

// const app = express();
// const PORT = 3000;

// app.use(bodyParser.json()); // Для парсинга JSON

// // Настройка подключения к MySQL
// const db = mysql.createConnection({
//     host: 'localhost',
//     port: 8889,
//     user: 'root',
//     password: 'root',
//     database: 'project'
// });

// // Подключение к базе данных
// db.connect(err => {
//     if (err) {
//         console.error('Ошибка подключения к базе данных:', err);
//         return;
//     }
//     console.log('Подключение к базе данных успешно!');
// });

// // Эндпоинт для добавления пользователя
// app.post('/add-user', (req, res) => {
//     const newUser  = req.body; // Получаем данные из тела запроса

//     const sql = 'INSERT INTO users SET ?';
//     db.query(sql, newUser , (err, result) => {
//         if (err) {
//             return res.status(500).send('Ошибка при добавлении пользователя');
//         }
//         res.send(`Пользователь добавлен с ID: ${result.insertId}`);
//     });
// });

// // Запуск сервера
// app.listen(PORT, () => {
//     console.log(`Сервер запущен на http://localhost:${PORT}`);
// });