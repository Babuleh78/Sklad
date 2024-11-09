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
app.post('/get_hse_count', (req, res) =>{
    const query1 = 'SELECT COUNT(idplace) AS hse_count FROM place'; 
    
    connection.query(query1,null, (error, results)=>{
        if (error) {
            console.error("Ошибка", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        if(results.length>0){
            const hse_count = results[0].hse_count;
            res.json({ success: true, hse_count });
        }
    })

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
    const count = req.body.count;
    const checkUserQuery = 'SELECT COUNT(*) AS count FROM user WHERE usertoken = ?';
    connection.query(checkUserQuery, [username], (error, results) => {
        if (error) {
            console.error('Ошибка при выполнении запроса:', error);
            return res.status(500).send('Ошибка при проверке пользователя');
        }
        if (results[0].count > 0) {
            return res.status(200).send('Пользователь с таким именем уже зарегистрирован');
        }
        const sql = 'INSERT INTO user (usertoken, stars) VALUES (?, ?)';
        const userValues = [username, 0];
        connection.query(sql, userValues, (error, results) => {
            if (error) {
                console.error('Ошибка при выполнении запроса:', error);
                return res.status(500).send('Ошибка при добавлении пользователя');
            }

            
            const userId = results.insertId; 
            const visitQuery = 'INSERT INTO visits (user_id, place_id, is_visit) VALUES (?, ?, ?)';
            const visitPromises = [];
            for (let i = 1; i <= count; i++) {
                visitPromises.push(new Promise((resolve, reject) => {
                    connection.query(visitQuery, [userId, i, 0], (error, results) => {
                        if (error) {
                            console.error('Ошибка при выполнении запроса:', error);
                            return reject(error);
                        }
                        resolve(results);
                    });
                }));
            }

            Promise.all(visitPromises)
                .then(() => {
                    res.status(201).send('Пользователь успешно добавлен и посещения зарегистрированы');
                })
                .catch(err => {
                    console.error('Ошибка при добавлении посещений:', err);
                    res.status(500).send('Ошибка при добавлении посещений');
                });
        });
    });
});
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});