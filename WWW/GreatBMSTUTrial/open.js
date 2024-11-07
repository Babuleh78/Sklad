const modal = document.getElementById("user_info");
const btn = document.getElementById("open_user");
const span = document.getElementById("close_user");
modal.style.display = "none";
btn.onclick = function() {
    modal.style.display = "block";
}

span.onclick = function() {
    modal.style.display = "none";
}

window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}