const TextContainerAch = document.getElementById("ach_form");
const AchForm = document.getElementById('ach_form');

async function updateDisplayAch() {
    TextContainerAch.innerHTML = '';
    const all = await get_all_ach(); //id_ach, url, title, text
    
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
            TextElement.className = "ach_element";
            TextElement.textContent = Element.text;
            const PhotoElement = document.createElement('img');
            PhotoElement.className = "ach_image";
            PhotoElement.src = Element.url;
            entryContainer.appendChild(TextElement);
            entryContainer.appendChild(PhotoElement);
            TextContainerAch.appendChild(entryContainer); 
        }
    }
}
async function get_all_ach() {
    try {
        const response = await fetch(`http://localhost:3000/getAch`);
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

updateDisplayAch(); 

// let isMouseDown = false;
// let startY;
// let scrollTop;
// journalForm.addEventListener('mousedown', (e) => {
//     isMouseDown = true;
//     e.preventDefault(); 
//     startY = e.pageY - journalForm.offsetTop;
//     scrollTop = journalForm.scrollTop;
// });
// journalForm.addEventListener('mouseleave', (e) => {
    
//         e.preventDefault(); 
//         isMouseDown = false;
// });

// journalForm.addEventListener('mouseup', (e) => {
    
//         e.preventDefault(); 
//         isMouseDown = false;
// });
// journalForm.addEventListener('mousemove', (e) => {
//     if (!isMouseDown) return; 
//         e.preventDefault(); 
//         const y = e.pageY - journalForm.offsetTop;
//         const walk = (y - startY) * 1; 
//         journalForm.scrollTop = scrollTop - walk; 
// });