// var express = require("express");
// var mysql = require("mysql");
// var app = express();

// app.get('/', function(req, res){
//     res.send(
//         'Шашура'
//     )
// });

// app.listen(3000, function(){
//     console.log(
//         'port 3000'
//     )
// });
var mysql = require("mysql");

var connection = mysql.createConnection({
    host: 'localhost',
    database: 'project',
    user: 'root',
    password: 'root'
});

module.exports = connection;