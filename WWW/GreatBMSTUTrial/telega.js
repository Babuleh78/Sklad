const telega = document.getElementById("telega");
const closeModalButton = document.getElementById("closeTelega");
const go_btn = document.getElementById("go_to_telega");
go_btn.addEventListener("click", function(){
    window.open("https://t.me/ForGreatBMSTUTrailBot?start");
});


closeModalButton.addEventListener("click", function() {
    telega.style.display = "none";
});

// Закрываем модальное окно при клике вне его
window.addEventListener("click", function(event) {
    if (event.target === telega) {
        telega.style.display = "none";
    }
});