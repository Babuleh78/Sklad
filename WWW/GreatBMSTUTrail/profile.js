import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-app.js';
  import { getAuth, createUserWithEmailAndPassword } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-auth.js';


  const firebaseConfig = {
    apiKey: "AIzaSyCRBI8odC_xkOQWvm3RQByJsqQ1XsrL3WA",
    authDomain: "nightofthelongknives-b66e2.firebaseapp.com",
    projectId: "nightofthelongknives-b66e2",
    storageBucket: "nightofthelongknives-b66e2.appspot.com",
    messagingSenderId: "1085351862604",
    appId: "1:1085351862604:web:c28fbd08e96b9026006fe9",
    measurementId: "G-TRCG7E5HPC"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth(app); 
const currentUser = auth.currentUser;
const nickname = document.getElementById("name");

const idToken = localStorage.getItem('firebaseIdToken');
if (idToken) {
    // JWT состоит из трех частей: заголовок, полезная нагрузка и подпись
    const payload = idToken.split('.')[1]; // Получаем часть, содержащую полезную нагрузку

    // Декодируем полезную нагрузку из Base64URL
    const decodedPayload = JSON.parse(decodeURIComponent(escape(atob(payload.replace(/-/g, '+').replace(/_/g, '/')))));

    console.log("Decoded Token:", decodedPayload);

    // Получаем информацию о пользователе
    const userId = decodedPayload.user_id; // Идентификатор пользователя
    const email = decodedPayload.email; // Электронная почта пользователя
    const name = decodedPayload.name; // Имя пользователя (если указано)

    console.log(`User ID: ${userId}`);
    console.log(`Email: ${email}`);
    console.log(`Name: ${name}`);
}
