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

const idTokenLog = localStorage.getItem('firebaseIdTokenLog');
const idTokenReg = localStorage.getItem('firebaseIdTokenReg');

if (idTokenReg || idTokenLog) {
    let payload;
    if(idTokenReg && idTokenReg.trim() !== '') {
        payload = idTokenReg.split('.')[1];
    } else if (idTokenLog && idTokenLog.trim() !== '') {
        payload = idTokenLog.split('.')[1];
    }
    const decodedPayload = JSON.parse(decodeURIComponent(escape(atob(payload.replace(/-/g, '+').replace(/_/g, '/')))));
    const userId = decodedPayload.user_id; 
    const email = decodedPayload.email;
    const parts = email.split('@');
    const username = parts[0];
    nickname.textContent = username;
    localStorage.setItem('UID', userId);
    if(idTokenReg){
        fetch('http://localhost:3000/addUserReg', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ username: username })
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Ошибка при добавлении пользователя');
            }
            return response.text();
        })
        .then(data => {
            console.log(data); // Успешное добавление
        })
        .catch(error => {
            console.error('Ошибка:', error);
        });
    }
}
