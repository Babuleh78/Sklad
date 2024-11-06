const mysql = require('mysql');
const express = require('express');
const path = require('path');
const app = express();
const PORT = 8080;
const connection = mysql.createConnection({
  host: 'localhost', // адрес сервера, по умолчанию localhost
  user: 'root', // имя пользователя
  password: 'root', // пароль
  database: 'project', // имя базы данных
  port: 8888 // порт, на котором работает MySQL
});
connection.connect(err => {
    if (err) {
      console.error('Ошибка подключения:', err);
      return;
    }
    console.log('Соединение с базой данных установлено!');
  });
  connection.query('SELECT * FROM user', (err, results, fields) => {
    if (err) {
      console.error('Ошибка запроса:', err);
      return;
    }
    console.log('Результаты:', results);
  });
app.use(express.static(path.join(__dirname, 'public')));

app.get('/LogIn', (req, res) => {
    res.sendFile(path.join(__dirname, 'LogIn.html'));
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    
});