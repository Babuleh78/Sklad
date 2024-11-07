const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 
const cors = require('cors');
const { closeConnectionDB, getDataFromDB } = require('./readfromsql');
app.use(cors());
app.use(express.json());
const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'project',
    port: 8888
});
app.get('/RID', async (req, res) => {
    try {
        const data = await getDataFromDB(); 
        res.json(data); 
    } catch (error) {
        console.error('Ошибка:', error);
        res.status(500).json({ error: 'Ошибка при получении данных' });
    } 
});


app.post('/visit', (req, res) => {
    const { userId, placeId } = req.body;

    const query = 'INSERT INTO visits (iduser, idplace) VALUES (?, ?)';
    connection.query(query, [userId, placeId], (error, results) => {
        if (error) {
            if (error.code === 'ER_DUP_ENTRY') {
                return res.json({ success: false, message: 'Уже было' });
            }
            return res.status(500).json({ success: false, message: 'Опять хуйня произошла' });
        }
        res.json({ success: true });
    });
});
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});