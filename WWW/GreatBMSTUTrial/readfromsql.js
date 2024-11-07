const mysql = require('mysql2');

const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root', 
    password: 'root', 
    database: 'project',
    port: 8888
});

const getDataFromDB = () => {
    return new Promise((resolve, reject) => {
        connection.query('SELECT * FROM place', (error, results) => {
            if (error) {
                return reject(error);
            }
            resolve(results);
        });
    });
};

const closeConnectionDB = () => {
    connection.end();
};

module.exports = { getDataFromDB, closeConnectionDB };