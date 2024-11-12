let notes = [];
const Container = document.getElementById('entries');
const addBtn = document.getElementById('addEntryButton');

function updateDisplay() {
    Container.innerHTML = '';

    if (notes.length === 0) {
        Container.innerHTML = '<p>Нет записей</p>'; 
    } else {
        notes.forEach((entry, index) => {
            const Element = document.createElement('p');
            Element.textContent = `${entry}`;
            Container.appendChild(Element);
        });
    }
}

addBtn.addEventListener('click', async () => {//ДОПИЛИТЬ
    const username = document.getElementById("namej").textContent;
    const placeId = 2;

    try {
        const res = await fetch('http://localhost:3000/addNote', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ username, placeId })
        });

        if (!res.ok) {
            throw new Error('Ошибка при добавлении');
        }
        const newEntry = await res.json(); 
        console.log(newEntry);
        notes.push(newEntry.string); 
        updateDisplay(); 
    } catch (error) {
        console.error('Ошибка:', error);
    }
});
updateDisplay();
