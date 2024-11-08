const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 
const cors = require('cors');
const { closeConnectionDB, getDataFromDB } = require('./readfromsql');
const { UserRecord } = require('firebase-admin/auth');
const { error } = require('console');
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
app.post('/get_count', (req, res) => {
    const { placeId } = req.body;
    if (!placeId) {
        return res.status(400).json({ success: false, message: 'placeId не указан' });
    }
    const query1 = 'SELECT visit_count FROM place WHERE idplace = ?'; 
    connection.query(query1, [placeId], (error, results) => {
        if (error) {
            console.error("Ошибка при выполнении запроса", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        if (results.length > 0) {
            const count = results[0].visit_count; 
            res.json({ success: true, count});
        } else {
            res.json({ success: false, message: 'Место не найдено' });
        }
    });
});
app.post('/check_visit', (req, res) => {
    const { username, placeId } = req.body;

    const query = 'SELECT iduser FROM user WHERE usertoken = ?'; 
    connection.query(query, [username], (error, results) => {
        if (error) {
            console.error("Ошибка", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        if (results.length > 0) {
            const userId = results[0].iduser; 
            const query1 = 'SELECT is_visit FROM visits WHERE place_id = ? AND user_id = ?'; 
            connection.query(query1, [placeId, userId], (error, results) => {
                if (error) {
                    console.error("Ошибка при выполнении запроса", error);
                    return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
                }

                if (results.length > 0) {
                    const isVisit = results[0].is_visit;
                    res.json({ success: true, is_visit: isVisit }); 
                } else {
                    res.json({ success: false, message: 'Запись о посещении не найдена' });
                }
            });
        } else {
            res.json({ success: false, message: 'Пользователь не найден' });
        }
    });
});
app.post('/visit', (req, res) => {
    const { username, placeId } = req.body;
    const query1 = 'SELECT iduser FROM user WHERE usertoken = ?'; 
    connection.query(query1, [username], (error, results) => {
        if (error) {
            console.error("Ошибка", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        if (results.length > 0) {
            const idUser  = results[0].iduser; 
            const query2 = 'UPDATE place SET visit_count = visit_count + 1 WHERE idplace = ?';
            connection.query(query2, [placeId], (error) => {
                if (error) {
                    console.error("Ошибка при обновлении", error);
                    return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
                }
                const query3 = 'SELECT is_visit FROM visits WHERE user_id = ? AND place_id = ?'; 
                connection.query(query3, [idUser , placeId], (error, results) => {
                    if (error) {
                        console.error("Ошибка при получении значения", error);
                        return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
                    }

                    if (results.length > 0) {
                        const isVisit = results[0].is_visit;
                        const query4 = 'UPDATE visits SET is_visit = 1 WHERE user_id = ? AND place_id = ?';
                        connection.query(query4, [idUser , placeId], (error) => {
                            if (error) {
                                console.error("Ошибка при обновлении статуса посещения", error);
                                return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
                            }
                            res.json({ success: true, isVisit });
                        });
                    } else {
                        res.json({ success: false, message: 'Посещение не найдено' });
                    }
                });
            });
        } else {    
            res.json({ success: false, message: 'Пользователь не найден' });
        }
    });
});
app.post('/addUserReg', (req, res) => {
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