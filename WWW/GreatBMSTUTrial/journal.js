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
            Element.textContent = `Запись ${index + 1}: ${entry}`;
            Container.appendChild(Element);
        });
    }
}

addBtn.addEventListener('click', () => {
    const newEntry = `Запись ${notes.length + 1} создана`;
    notes.push(newEntry); 
    updateDisplay(); 
});

updateDisplay();
