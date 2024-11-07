const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 

// Создание подключения к базе данных
const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'project'
});

app.post('/add', (req, res) => {
    console.log("POST работает");
    // Здесь вы можете добавить логику для работы с базой данных
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});