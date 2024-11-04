const buttons = document.querySelectorAll('.image-button');
const container = document.getElementById("container");
var taps = 16;
buttons.forEach(button => {
    button.addEventListener('click', () => {
        if (button.classList.contains('active')) {
            button.classList.remove('active');
            taps-=1;
        } else {
            button.classList.add('active'); 
            taps+=1;
        }
    });
});
const go_button = document.getElementById("go-button");
go_button.addEventListener('click', ()=>{
    if(taps == 16){
        container.style.visibility = 'hidden';
    } else{
        console.log(taps);
    }
});