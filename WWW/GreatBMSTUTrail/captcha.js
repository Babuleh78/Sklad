const buttons = document.querySelectorAll('.image-button');
console.log("Скрипт Капча");

buttons.forEach(button => {
    button.addEventListener('click', () => {
        button.classList.toggle('active');
    });
});