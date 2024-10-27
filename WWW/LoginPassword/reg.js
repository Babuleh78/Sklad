
const firebaseConfig = {
    apiKey: "AIzaSyCRBI8odC_xkOQWvm3RQByJsqQ1XsrL3WA",
    authDomain: "nightofthelongknives-b66e2.firebaseapp.com",
    projectId: "nightofthelongknives-b66e2",
    storageBucket: "nightofthelongknives-b66e2.appspot.com",
    messagingSenderId: "1085351862604",
    appId: "1:1085351862604:web:c28fbd08e96b9026006fe9",
    measurementId: "G-TRCG7E5HPC"
  };
const app = firebase.initializeApp(firebaseConfig);
const auth = firebase.auth();
document.getElementById('registrationForm').addEventListener('submit', function(event) {
    event.preventDefault(); // Предотвращаем перезагрузку страницы

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // Создание пользователя с произвольным логином
    auth.createUserWithEmailAndPassword(username + "@example.com", password) // Используем email как временный идентификатор
        .then((userCredential) => {
            // Успешная регистрация
            const user = userCredential.user;
            console.log('Пользователь зарегистрирован:', user);
            alert('Регистрация успешна!');
        })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;
            console.error('Ошибка регистрации:', errorCode, errorMessage);
            alert('Ошибка: ' + errorMessage);
        });
});
