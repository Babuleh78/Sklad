const modal = document.getElementById("user_info");
const btn = document.getElementById("open_user");
const span = document.getElementById("close_user");
modal.style.display = "none";
// Когда пользователь нажимает на кнопку, открываем модальное окно
btn.onclick = function() {
    modal.style.display = "block";
}

// Когда пользователь нажимает на "x", закрываем модальное окно
span.onclick = function() {
    modal.style.display = "none";
}

// Когда пользователь нажимает в любом месте вне модального окна, закрываем его
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}