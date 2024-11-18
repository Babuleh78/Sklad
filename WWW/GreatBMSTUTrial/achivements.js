const TextContainerAch = document.getElementById("ach_form");
const AchForm = document.getElementById('ach_form_scroll');
let DRAWCHECK = false;
const nicknameach = document.getElementById("name");
let user_id = -10;
async function DrawDisplayAch() {
  if(DRAWCHECK){
    return;
  }
  DRAWCHECK = true;
  if(user_id === -10){
    user_id = await getId(nicknameach.textContent);
  } else if(user_id === -1){
    console.log("Полная пизда");
  }
    TextContainerAch.innerHTML = '';
    const all = await get_all_ach(); //id_ach, url, title, text
    const all_open = await get_all_ach_open();
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
            entryContainer.id = "entry_" + (i+1 ) ; 
            if(all_open[i].is_open){
              entryContainer.className = "entry_container_ach";
            } else{
              entryContainer.className = "entry_container_ach_unactive";
            }
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
async function updateDisplayAch(i) {
  const ach = document.getElementById("entry_"+i);
  if(ach!==null){
    ach.className = "entry_container_ach";
    console.log("Обновили");
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
async function get_all_ach_open() {
  try {
    const response = await fetch(`http://localhost:3000/get_ach_open`);
    if (!response.ok) {
        throw new Error(`Ошибка: ${response.status}`);
    }
    const data = await response.json();
    return data.ach_mas;
} catch (error) {
    console.error(error.message);
    return -1; 
}
}
function set_ach_comp(id) {
    const current_entry = document.getElementById("entry_" + (id+1));
    current_entry.classNmae = "entry_container_ach";
}


async function getId(userName) {
  const response = await fetch(`http://localhost:3000/get_id_from_name?userName=${encodeURIComponent(userName)}`);
  if (!response.ok) {
      throw new Error(`Ошибка: ${response.status}`);
  }
  const data = await response.json();
  return await data.id;
}



let isMouseDownA = false;
let startYA;
let scrollTopA;

AchForm.addEventListener('mousedown', (e) => {
  isMouseDownA = true;
  e.preventDefault(); 
  startYA = e.pageY - AchForm.offsetTop;
  scrollTopA = AchForm.scrollTop;
});

AchForm.addEventListener('mouseleave', (e) => {
  e.preventDefault(); 
  isMouseDownA = false;
});

AchForm.addEventListener('mouseup', (e) => {
  e.preventDefault(); 
  isMouseDownA = false;
});

AchForm.addEventListener('mousemove', (e) => {
  if (!isMouseDownA) return; 
  e.preventDefault(); 
  const y = e.pageY - AchForm.offsetTop;
  if (y !== startYA) {
    const walk = (y - startYA) * 1; 
    AchForm.scrollTop = scrollTopA - walk;
  }
});