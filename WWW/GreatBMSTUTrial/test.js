

connection.query("SELECT * FROM user", null, (error, results)=>{
            if (error) {
                console.error("Ошибка при выполнении запроса", error);
               
            }
            if (results.length > 0) {
               console.log(results);
            }
        });