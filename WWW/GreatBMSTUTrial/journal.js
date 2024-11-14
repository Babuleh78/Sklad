
const TextContainer = document.getElementById('entries');
const PhotoContainer = document.getElementById('photo');
const journalForm = document.getElementById('journal_form');

async function updateDisplay() {
    // Очищаем контейнеры
    TextContainer.innerHTML = '';
    PhotoContainer.innerHTML = ''; 
    const notes = await get_notes();
    if (notes === -1) {
        TextContainer.innerHTML = '<p>Ошибка при получении записей</p>'; 
        return; 
    }
    if (notes.length === 0) {
        TextContainer.innerHTML = '<p>Нет записей</p>'; 
    } else {
        notes.forEach((entry) => {
            const entryContainer = document.createElement('div');
            entryContainer.className = "entry_container";
            const TextElement = document.createElement('p');
            TextElement.className = "journal_element";
            TextElement.textContent = entry.text; 
            const PhotoElement = document.createElement('img');
            PhotoElement.className = "avatar_journal";
            PhotoElement.src = "avatars/BAZA.jpeg"; 
            entryContainer.appendChild(PhotoElement);
            entryContainer.appendChild(TextElement);
            TextContainer.appendChild(entryContainer); 
        });
    }
}

async function get_notes() {
    try {
        const response = await fetch(`http://localhost:3000/getNote`);
        if (!response.ok) {
            throw new Error(`Ошибка: ${response.status}`);
        }
        const data = await response.json();
        return data.text;
    } catch (error) {
        console.error(error.message);
        return -1; 
    }
}
updateDisplay();


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