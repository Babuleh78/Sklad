const express = require('express');
const path = require('path');
const mysql = require('mysql2'); // Не забудьте подключить mysql2
const app = express();
const PORT = 3000; 
const cors = require('cors');
const { getDataFromDB } = require('./readfromsql');
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
//СЧИТЫВАНИЕ ДАННЫХ
app.get('/RID', async (req, res) => {
    try {
        const data = await getDataFromDB(); 
        res.json(data); 
    } catch (error) {
        console.error('Ошибка:', error);
        res.status(500).json({ error: 'Ошибка при получении данных' });
    } 
});
//ПОЛУЧИТЬ СКОЛЬКО ПОСЕТИЛИ КАЖДОЕ МЕСТО ДЛЯ ЗАГРУЗКИ КАРТЫ
app.get('/get_visit_count', (req, res) => {
    const { placeId } = req.query;
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
//ПРОВЕРИТЬ БЫЛ ЛИ ПОЛЬЗОВАТЕЛЬ
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
//ПОЛУЧИТЬ КОЛИЧЕСТВО ВСЕХ ВЫШЕК
app.get('/get_hse_count', (req, res) => {
    const query1 = 'SELECT COUNT(idplace) AS hse_count FROM place'; 
    connection.query(query1, null, (error, results) => {
        if (error) {
            console.error("Ошибка", error);
            return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
        }
        if (results.length > 0) {
            const hse_count = results[0].hse_count;
            res.json({ success: true, hse_count });
        } else {
            res.json({ success: true, hse_count: 0 }); 
        }
    });
});
//ПОСЕТИТЬ КОЛИЧЕСТВО ДЛЯ КОНКРЕТНОГО ПОЛЬЗОВАТЕЛЯ
    app.get('/get_hse_count_for_user', (req, res) => {
        const { userName } = req.query;
        
        const query1 = 'SELECT stars AS hse_count_user FROM user WHERE usertoken = ?'; 
        connection.query(query1, [userName], (error, results) => {
            if (error) {
                console.error("Ошибка", error);
                return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
            }
            if (results.length > 0) {
                const hse_count_user = results[0].hse_count_user;
                res.json({ success: true, hse_count_user});
            } else {
                res.json({ success: true, hse_count_user: -1 }); 
            }
        });
    });
//ПОСЕТИТЬ (КАК ЖЕ ОНО УБОГО НАПИСАНО) уже не убого, жаль, что мне лень все на такие же рельсы переводить
app.post('/visit', async (req, res) => {
    const { username, placeId } = req.body;
    try {
        const [userResults] = await query('SELECT iduser FROM user WHERE usertoken = ?', [username]);
        if (userResults.length === 0) {
            return res.json({ success: false, message: 'Пользователь не найден' });
        }
        const idUser = userResults.iduser;
        await query('UPDATE place SET visit_count = visit_count + 1 WHERE idplace = ?', [placeId]);
        const [visitResults] = await query('SELECT is_visit FROM visits WHERE user_id = ? AND place_id = ?', [idUser , placeId]);
        const isVisit = visitResults.is_visit;
        if (!isVisit) {
            await query('UPDATE visits SET is_visit = 1 WHERE user_id = ? AND place_id = ?', [idUser , placeId]);
            await query('UPDATE user SET stars = stars + 1 WHERE iduser = ?', [idUser ]);
            const results = await query('SELECT visit_count FROM place WHERE idplace = ?', [placeId]);

            if (results.length > 0) {
                const count = results[0].visit_count; 
                return res.json({ success: true, count: count }); 
            } else {
            
            return res.json({ success: false, message: 'Этого не может быть, промежуток должен быть' });
        }
        } else {
            return res.json({ success: false, message: 'Посещение не найдено' });
        }
    } catch (error) {
        console.error("Ошибка", error);
        return res.status(500).json({ success: false, error: 'Ошибка при выполнении запроса' });
    }
});

//ДАВНО ПОРА БЫЛО
function query(sql, params) {
    return new Promise((resolve, reject) => {
        connection.query(sql, params, (error, results) => {
            if (error) {
                return reject(error);
            }
            resolve(results);
        });
    });
}
//ДОБАВИТЬ ПОЛЬЗОВАТЕЛЯ
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
            const achQuery = 'INSERT INTO open_ach (user_id, ach_id, is_open) VALUES (?, ?, ?)';
            const visitPromises = [];
            const achPromises = [];
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
            for (let i = 1; i <= 7; i++) { // МНЕ ЛЕЕЕЕЕЕНЬ
                achPromises.push(new Promise((resolve, reject) => {
                    connection.query(achQuery, [userId, i, 0], (error, results) => {
                        if (error) {
                            console.error('Ошибка при выполнении запроса:', error);
                            return reject(error);
                        }
                        resolve(results);
                    });
                }));
            }
            Promise.all([...visitPromises, ...achPromises])
                .then(() => {
                    res.status(201).send('Пользователь успешно добавлен, посещения и достижения зарегистрированы');
                })
                .catch(err => {
                    console.error('Ошибка при добавлении:', err);
                    res.status(500).send('Ошибка при добавлении данных');
                });
        });
    });
});
//ПОЛУЧИТЬ НОМЕР АВЫ
app.get('/GetAvatar', (req, res)=>{
    const {userName} = req.query;
    const query1 = 'SELECT picid FROM user WHERE usertoken = ?';
    connection.query(query1, [userName], (error, results)=>{
        if (error) {
            console.error('Ошибка при получении аватарки:', error);
            return res.status(500).send('Ошибка при получении аватарки');
        }
        else{
            const picid = results[0].picid;
            return res.json({success: true, avatar: picid});
        }
        
    });
});
//ЗАПИСАТЬ НОМЕР АВЫ
app.post('/SetAvatar', (req, res)=>{
    const {userName, picId} = req.body;
    const query1 = 'UPDATE user SET picid = ? WHERE (usertoken = ?);';
    connection.query(query1, [picId, userName], (error, results)=>{
        if (error) {
            console.error('Ошибка при изменении аватарки', error);
            return res.status(500).send('Ошибка при изменении аватарки');
        }
        else{
            return res.json({success: true});
        }
        
    });
});
//ДОБАВИТЬ ЗАПИСЬ О ПОСЕЩЕНИИ
app.post('/addNote', async (req, res) => {
    const { username, placeId } = req.body;
    const query0 = 'SELECT picid FROM user WHERE usertoken = ?';
    try {
        const [userResults] = await connection.promise().query(query0, [username]);
        
        if (userResults.length === 0) {
            return res.status(404).json({ success: false, message: 'Пользователь не найден' });
        }
        const picid = userResults[0].picid;
        const query1 = 'SELECT note FROM place WHERE idplace = ?;';
        const [placeResults] = await connection.promise().query(query1, [placeId]);

        if (placeResults.length === 0) {
            return res.status(404).json({ success: false, message: 'Запись не найдена' });
        }
        const info = placeResults[0].note;
        const UPusername = username.toUpperCase();
        const str = `${UPusername} ЛИКВИДИРОВАЛ ${info}`;
        const query2 = 'INSERT INTO notes (text, user_avatar) VALUES (?, ?);';
        await connection.promise().query(query2, [str, picid]);
        return res.json({ success: true, note: str, avatar: picid });
    } catch (error) {
        console.error('Ошибка при обработке запроса', error);
        return res.status(500).json({ success: false, message: 'Ошибка при обработке запроса' });
    }
});
//ЕСЛИ БОЛЬШЕ 7-ми записей, удаляем. Вообще, как-то тяжко получилось
app.post('/check_journal_count', async (req, res) => {
    const query1 = 'SELECT COUNT(noteid) AS note_count FROM notes'; 
    connection.query(query1, null, (error, results) => {
        if (error) {
            console.error('Ошибка при проверки количества (КАК)', error);
            return res.status(500).send('Ошибка при проверке количества');
        } else {
            const count_note = results[0].note_count; 
            if (count_note >= 7) {
                const query2 = `SELECT noteid FROM notes ORDER BY noteid LIMIT 1`;
                connection.query(query2, (error, results) => {
                  if (error) {
                    console.error('Ошибка при получении noteid', error);
                    return res.status(500).send('Ошибка при получении noteid');
                  } else {
                    const noteid = results[0].noteid;
                    const query3 = `DELETE FROM notes WHERE noteid = ?`;
                    connection.query(query3, noteid, (error, results) => {
                      if (error) {
                        console.error('Ошибка при удалении', error);
                        return res.status(500).send('Ошибка при удалении');
                      } else {
                        return res.json({ success: true });
                      }
                    });
                  }
            
                });
            } else {
                return res.json({ success: false, message: 'Количество записей меньше или равно 7' }); 
            }
        }
    });
});
//ПОЛУЧИТЬ ЗАПИСИ
app.get('/getNote', (req, res)=>{
    const query1 = 'SELECT * FROM notes';
    connection.query(query1, null, (error, results)=>{
        if (error) {
            console.error('Ошибка при получении записей', error);
            return res.json({success: false, all: -1});
        }
        else{
            return res.json({success: true, all: results});
        }
    })
});
//ПОЛУЧИТЬ ДОСТИЖЕНИЯ
app.get('/getAch', (req, res)=>{
    const query1 = 'SELECT * FROM achivements';
    connection.query(query1, null, (error, results)=>{
        if (error) {
            console.error('Ошибка при получении записей', error);
            return res.json({success: false, all: -1});
        }
        else{
            return res.json({success: true, all: results});
        }
    })
});
//ПОЛУЧИТЬ КОЛИЧЕСТВО ЗВЕЗД
app.get('/get_stars_count', (req, res)=>{
    const query1 = 'SELECT stars FROM place';
    connection.query(query1, null, (error, results)=>{
        if (error) {
            console.error('Ошибка при получении количества звезд у мест', error);
            return res.json({success: false, stars: -1});
        }
        else{
            return res.json({success: true, stars: results});
        }
    })


});
//СЛУШАТЬ
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
    connection.connect(function(err) {
        if (err) throw err;
        console.log("Подключение к базе данных успешно!");
    });
});