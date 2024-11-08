const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 
const cors = require('cors');
const { closeConnectionDB, getDataFromDB } = require('./readfromsql');
const { UserRecord } = require('firebase-admin/auth');
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
    const { username, placeId } = req.body;
    console.log(username);

    // Первый запрос для получения iduser
    const query1 = 'SELECT iduser FROM user WHERE usertoken = ?'; 
    connection.query(query1, [username], (error, results) => {
        if (error) {
            console.error("Ошибка", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        console.log(results);
        if (results.length > 0) {
            const idUser  = results[0].iduser; 
            console.log("ID пользователя:", idUser );

            const query2 = 'SELECT is_visit FROM visits WHERE user_id = ? AND place_id = ?'; 
            connection.query(query2, [idUser , placeId], (error, results) => {
                console.log("Места", placeId);

                if (error) {
                    console.error("Ошибка", error);
                    return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
                }
                console.log(results);
                if (results.length > 0) {
                    const isVisit = results[0].is_visit;
                    res.json({ success: true, isVisit });
                } else {
                    res.json({ success: false, message: 'Посещение не найдено' });
                }
            });
        } else {    
            res.json({ success: false, message: 'Пользователь не найден' });
        }
    });
});

app.post('/addUserReg', (req, res) => {
    console.log("ОбработкаРегистрации");
    const username = req.body.username;

    const sql = 'INSERT INTO user (usertoken, stars) VALUES (?, ?)';
    const values = [username, 0];

    connection.query(sql, values, (error, results) => {
        if (error) {
            console.error('Ошибка при выполнении запроса:', error);
            return res.status(500).send('Ошибка при добавлении пользователя');
        }
        console.log('Пользователь добавлен:', results);
        res.status(201).send('Пользователь успешно добавлен');
    });

    
});

app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});