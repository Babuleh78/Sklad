const express = require('express');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 

// Middleware для парсинга данных формы
app.use(express.urlencoded({ extended: true })); // Для парсинга application/x-www-form-urlencoded
app.use(express.json()); // Для парсинга application/json

// Создание подключения к базе данных
const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'project',
    port: 8888
});

// Обработка POST-запроса
app.post('/add', (req, res) => {
    console.log("POST работает");
    
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});