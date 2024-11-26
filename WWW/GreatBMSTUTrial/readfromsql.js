// const mysql = require('mysql2');

// const connection = mysql.createConnection({
//     // host: '109.252.15.235',
//      host: '141.8.192.138',
//      //192.168.1.65
//      user: 'a1057017_babuleh',
//      password: 'LopastiNeGluposti',
//      database: 'a1057017_babuleh',
//      port: 3306
//  });

// const getDataFromDB = () => {
//     return new Promise((resolve, reject) => {
//         connection.query('SELECT * FROM user', (error, results) => {
//             if (error) {
//                 return reject(error);
//             }
//             resolve(results);
//         });
//     });
// };

// const closeConnectionDB = () => {
//     connection.end();
// };

// module.exports = { getDataFromDB, closeConnectionDB };