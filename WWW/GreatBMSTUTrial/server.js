const express = require('express');
const path = require('path');
const app = express();
const PORT = 8080;

// Обработка маршрута /LogIn
app.get('/LogIn', (req, res) => {
    res.sendFile(path.join(__dirname, 'LogIn.html'));
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err){
        if(err) throw err;
        console.log("Connect");
    })
});