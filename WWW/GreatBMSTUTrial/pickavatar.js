const avatar = document.getElementById("avatar");
const av1 = document.getElementById("avatarpick1");
const av2 = document.getElementById("avatarpick2");
const av3 = document.getElementById("avatarpick3");
const av4 = document.getElementById("avatarpick4");
const av5 = document.getElementById("avatarpick5");
const av6 = document.getElementById("avatarpick6");
const av7 = document.getElementById("avatarpick7");
const av8 = document.getElementById("avatarpick8");
let userName  = document.getElementById("name").textContent;
av1.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/BAZA.jpeg')";
    fetchfunc(userName, 0);
}
av2.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/BEGEMOT85.jpg')";
    fetchfunc(userName, 1);
}
av3.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/pain.jpg')";
    fetchfunc(userName, 2);
}
av4.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/ZVEZDA.png')";
    fetchfunc(userName, 3);
}
av5.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/PIVO.png')";
    fetchfunc(userName, 4);
}
av6.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/SLON.png')";
    fetchfunc(userName, 5);
}
av7.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/OCHKI.jpg')";
    fetchfunc(userName, 6);
}
av8.onclick = function(){
    userName  = document.getElementById("name").textContent;
    avatar.style.backgroundImage = "url('avatars/KPTCH.jpg')";
    fetchfunc(userName, 7);
}
fetchfunc = function(userName, picId){
    console.log("Пытаемся");
    fetch('http://localhost:3000/SetAvatar', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ userName: userName, picId: picId })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Ошибка с установкой аватарки в БД');
        }
        return response.json();
    })
    .then(data => {
        if (data.success) {

            
    } else {
        console.error(data.message); 
    }
    })
    .catch(error => {
        console.error('Ошибка:', error);              
    });
}