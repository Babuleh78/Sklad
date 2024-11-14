
const TextContainer = document.getElementById('entries');
const PhotoContainer = document.getElementById('photo');
const journalForm = document.getElementById('journal_form');
const namej = document.getElementById('namej');
let avatar_mas = ["avatars/BAZA.jpeg", "avatars/BEGEMOT85.jpg","avatars/FUCK.png", "avatars/ZVEZDA.png", "avatars/PIVO.png", 
    "avatars/SLON.png", "avatars/OCHKI.jpg", "avatars/KPTCH.jpg"
];
async function updateDisplayJ() {
    TextContainer.innerHTML = '';
    PhotoContainer.innerHTML = ''; 
    const all = await get_all_j();//noteid, text, user_avatar, user_nick
    if (all === -1) {
        TextContainer.innerHTML = '<p>Ошибка при получении записей</p>'; 
        return; 
    }
    if (all.length === 0) {
        TextContainer.innerHTML = '<p>Нет записей</p>'; 
    } else {
        for(let i = 0; i<all.length; i++){
            const Element = all[i];
            
            const entryContainer = document.createElement('div');
            entryContainer.className = "entry_container";
            const TextElement = document.createElement('p');
            TextElement.className = "journal_element";
            TextElement.textContent = Element.text;
            const PhotoElement = document.createElement('img');
            PhotoElement.className = "avatar_journal";
            const avid = Element.user_avatar;
            PhotoElement.src = avatar_mas[avid];
            entryContainer.appendChild(PhotoElement);
            entryContainer.appendChild(TextElement);
            TextContainer.appendChild(entryContainer); 
        }
    }
}
async function get_all_j() {
    try {
        const response = await fetch(`http://localhost:3000/getNote`);
        if (!response.ok) {
            throw new Error(`Ошибка: ${response.status}`);
        }
        const data = await response.json();
        return data.all;
    } catch (error) {
        console.error(error.message);
        return -1; 
    }
}

updateDisplayJ();
//Теперь чтоб могли вращать 

let isMouseDown = false;
let startY;
let scrollTop;
journalForm.addEventListener('mousedown', (e) => {
    isMouseDown = true;
    e.preventDefault(); 
    startY = e.pageY - journalForm.offsetTop;
    scrollTop = journalForm.scrollTop;
});
journalForm.addEventListener('mouseleave', (e) => {
    
        e.preventDefault(); 
        isMouseDown = false;
});

journalForm.addEventListener('mouseup', (e) => {
    
        e.preventDefault(); 
        isMouseDown = false;
});
//Не ну сам бы я до этого не додумался
journalForm.addEventListener('mousemove', (e) => {
    if (!isMouseDown) return; 
        e.preventDefault(); 
        const y = e.pageY - journalForm.offsetTop;
        const walk = (y - startY) * 1; 
        journalForm.scrollTop = scrollTop - walk; 
});