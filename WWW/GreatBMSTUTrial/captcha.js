const buttons = document.querySelectorAll('.image-button');
console.log("Скрипт Капча");
buttons.forEach(button => {
    button.addEventListener('click', () => {
        console.log("Нажатие обработано");
        if (button.classList.contains('active')) {
            button.classList.remove('active');
            console.log("Убрали");
        } else {
            button.classList.add('active'); 
            console.log("Добавили");
        }
    });
});

console.log("Сработал");
