<?php
// Параметры подключения к базе данных
$db_host = 'localhost'; // Хост
$db_user = 'root'; // Имя пользователя
$db_password = 'root'; // Пароль
$db_name = 'project'; // Имя базы данных

// Подключение к базе данных
$conn = new mysqli($db_host, $db_user, $db_password, $db_name);

// Проверка соединения
if ($conn->connect_error) {
    die("Ошибка подключения: " . $conn->connect_error);
}

// Проверка, была ли отправлена форма
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    $password = password_hash($_POST['password'], PASSWORD_DEFAULT); // Хеширование пароля

    // Подготовка и выполнение SQL-запроса
    $stmt = $conn->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    $stmt->bind_param("ss", $username, $password);

    if ($stmt->execute()) {
        echo "Регистрация прошла успешно!";
    } else {
        echo "Ошибка: " . $stmt->error;
    }

    // Закрытие подготовленного выражения и соединения
    $stmt->close();
}

$conn->close();
?>