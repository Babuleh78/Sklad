
const Container = document.getElementById('entries');


async function updateDisplay() {
    console.log("вызвали апдейт");
    Container.innerHTML = '';
    const notes = await get_notes(); 
    console.log(notes);

    if (notes === -1) {
        Container.innerHTML = '<p>Ошибка при получении записей</p>'; 
        return; 
    }

    if (notes.length === 0) {
        Container.innerHTML = '<p>Нет записей</p>'; 
    } else {
        notes.forEach((entry) => {
            const Element = document.createElement('p');
            Element.textContent = entry.text; 
            Container.appendChild(Element);
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
        console.log(data);
        return data.text;
    } catch (error) {
        console.error(error.message);
        return -1; 
    }
}
updateDisplay();