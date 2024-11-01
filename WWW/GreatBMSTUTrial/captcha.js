const buttons = document.querySelectorAll('.image-button');
const container = document.getElementById("container");
var taps = 0;
buttons.forEach(button => {
    button.addEventListener('click', () => {
        console.log("Нажатие обработано");
        if (button.classList.contains('active')) {
            button.classList.remove('active');
            taps-=1;
            console.log("Убрали");
        } else {
            button.classList.add('active'); 
            console.log("Добавили");
            taps+=1;
        }
    });
});
const go_button = document.getElementById("go-button");
go_button.addEventListener('click', ()=>{
    if(taps == 16){
        container.style.visibility = 'hidden';
    }
});