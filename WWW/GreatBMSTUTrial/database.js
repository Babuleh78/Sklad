const mysql = require('mysql2');

// Создайте подключение к базе данных
const connection = mysql.createConnection({
  host: 'localhost', // 127.0.0.1
  user: 'root',     
  password: 'root',     
  database: 'project',
  port: 8888
});

connection.connect((err) => {
  if (err) {
    console.error('Ошибка подключения: ' + err.stack);
    return;
  }
  console.log('Подключено как ID ' + connection.threadId);
});

connection.query('SELECT * FROM user', (error, results) => {
  if (error) {
    throw error;
  }
  console.log(results);
});

connection.end();