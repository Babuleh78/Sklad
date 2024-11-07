const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 
const cors = require('cors');
app.use(cors());
app.use(express.json());
// Создание подключения к базе данных
const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'project',
    port: 8888
});

app.post('/addUser', (req, res) => {
    console.log("Обработка");
    const username = req.body.username; // Получаем username из тела запроса

    // const sql = 'INSERT INTO users (username) VALUES (?)';
    // connection.query(sql, [username], (error, results) => {
    //     if (error) {
    //         console.error('Ошибка при вставке пользователя:', error);
    //         return res.status(500).send('Ошибка при вставке пользователя');
    //     }
    //     console.log('Пользователь добавлен с ID:', results.insertId);
    //     res.status(200).send('Пользователь успешно добавлен');
    // });
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});