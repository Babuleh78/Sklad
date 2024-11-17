const TextContainerAch = document.getElementById("ach_form");
const AchForm = document.getElementById('ach_form_scroll');

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
            entryContainer.className = "entry_container_ach";
            const TextElement = document.createElement('p');
            TextElement.className = "ach_text_element";
            TextElement.textContent = Element.text;
            const PhotoElement = document.createElement('img');
            PhotoElement.className = "ach_image";
            PhotoElement.src = Element.url; 
            const TitleElement = document.createElement('p');
            TitleElement.textContent = Element.title;
            TitleElement.className = "ach_title_element";
            entryContainer.appendChild(PhotoElement);
            entryContainer.appendChild(TextElement);
            entryContainer.appendChild(TitleElement);
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

let isMouseDownA = false;
let startYA;
let scrollTopA;
AchForm.addEventListener('mousedown', (e) => {
    console.log("down");
    isMouseDownA = true;
    e.preventDefault(); 
    startYA = e.pageY - AchForm.offsetTop;
    scrollTopA = AchForm.scrollTopA;
});
AchForm.addEventListener('mouseleave', (e) => {
    console.log("leave");
        e.preventDefault(); 
        isMouseDownA = false;
});

AchForm.addEventListener('mouseup', (e) => {
    console.log("up");
        e.preventDefault(); 
        isMouseDownA = false;
});
AchForm.addEventListener('mousemove', (e) => {
    console.log("move");
    if (!isMouseDownA) return; 
        e.preventDefault(); 
        const y = e.pageY - AchForm.offsetTopA;
        const walk = (y - startYA) * 1; 
        AchForm.scrollTopA = scrollTopA - walk; 
});