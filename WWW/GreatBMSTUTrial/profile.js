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
//Да, мне лень делать отдельную сущность для изображений, связывать ее с пользователем и тд, сами этим занимайтесь. 
let avatar_mas = ["url('avatars/BAZA.jpeg')", "url('avatars/BEGEMOT85.jpg')","url('avatars/pain.jpg')", "url('avatars/ZVEZDA.png')", "url('avatars/PIVO.png')", 
    "url('avatars/SLON.png')", "url('avatars/OCHKI.jpg')", "url('avatars/KPTCH.jpg')"
];
let username = "ОШИБКА";
const app = initializeApp(firebaseConfig);
const auth = getAuth(app); 
const currentUser = auth.currentUser;
const nickname = document.getElementById("name");
const nicknamej = document.getElementById("namej");
const stars = document.getElementById("stars");
const avatar = document.getElementById("avatar");
const pick_avatar = document.getElementById("pick_avatar");
const idTokenLog = localStorage.getItem('firebaseIdTokenLog');
const idTokenReg = localStorage.getItem('firebaseIdTokenReg');
// console.log(idTokenLog );
// console.log(idTokenReg);
if (idTokenReg || idTokenLog) {
    let payload;
    if(idTokenReg && idTokenReg.trim() !== '') {
        payload = idTokenReg.split('.')[1];
    } else if (idTokenLog && idTokenLog.trim() !== '') {
        payload = idTokenLog.split('.')[1];
    }
    const decodedPayload = JSON.parse(decodeURIComponent(escape(atob(payload.replace(/-/g, '+').replace(/_/g, '/')))));
    const email = decodedPayload.email;
    const parts = email.split('@');
    username = parts[0];
    nickname.textContent = username;
    nicknamej.textContent = username;
    
    if (idTokenReg) { //Называл себя бы максимально скромно - гений, запустил бы пафосную смену поколение, Prodigy, Chemical Brothers, Дядя Fatboy и Slim, пришла Эра 2R2R-а Сим-Селявим!
        console.log("Рег?");
        count = await get_hse_count();
        const addUserReg = async (username, count) => {
            try {
                const response = await fetch('http://localhost:3000/addUserReg', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({ username: username, count: count })
                });
        
                if (!response.ok) {
                    throw new Error('Ошибка при добавлении пользователя');
                }
                stars.textContent = 0;
            } catch (error) {
                
                console.error('Ошибка:', error);
            }
        };
        (async () => {
            await addUserReg(username, count); 
            
        })();
    } 
        async function get_hse_count_for_user(username) {
            try {
                const response = await fetch(`http://localhost:3000/get_hse_count_for_user?userName=${encodeURIComponent(username)}`);
                if (!response.ok) {
                    throw new Error(`Ошибка: ${response.status}`);
                }
                const data = await response.json();
                count = data.hse_count_user; 
                stars.textContent = count;
                return count; 
            } catch (error) {
                console.error(error.message);
                return -1; 
            }
        }
        await get_hse_count_for_user(username); 
        async function get_user_avatar(username) {
            try {
                const response = await fetch(`http://localhost:3000/GetAvatar?userName=${encodeURIComponent(username)}`);
                if (!response.ok) {
                    throw new Error(`Ошибка: ${response.status}`);
                }
                const data = await response.json();
                console.log(data);
                return data.avatar;
            } catch (error) {
                console.error(error.message);
                return -1; 
            }
        }
        const av = await get_user_avatar(username); 
        avatar.style.backgroundImage = avatar_mas[av];
        
    
}

pick_avatar.style.display = "none"
avatar.onclick = function(){
    if (pick_avatar.style.display === "block") {
        
        pick_avatar.style.display = "none";
    } else {
        pick_avatar.style.display = "block";
    }
}