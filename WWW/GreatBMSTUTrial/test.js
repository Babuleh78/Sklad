var express = require("express");
var mysql = require("mysql");
var app = express();

app.get('/', function(req, res){
    res.send(
        'Шашура'
    )
});

app.listen(3000, function(){
    console.log(
        'port 3000'
    )
});