
<?php
// Проверяем, был ли запрос методом POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Проверяем, заполнены ли необходимые поля
    if (isset($_POST["name"]) && isset($_POST["email"])) {
        // Выводим данные, отправленные через POST
        echo "Name: " . htmlspecialchars($_POST["name"]) . "<br>";
        echo "Email: " . htmlspecialchars($_POST["email"]) . "<br>";
    } else {
        echo "Поля name или email не заполнены!";
    }
} else {
    echo "Запрос не был отправлен методом POST.";
}
?>