const telega = document.getElementById("telega");
const closeModalButton = document.getElementById("closeTelega");
const go_btn = document.getElementById("go_to_telega");
const go_btn2 = document.getElementById("go_to_telega_btn");
go_btn.addEventListener("click", function(){
    window.open("https://t.me/ForGreatBMSTUTrailBot?start");
});
go_btn2.addEventListener("click", function(){
    window.open("https://t.me/ForGreatBMSTUTrailBot?start");
});



closeModalButton.addEventListener("click", function() {
   
    telega.style.display = "none";
});

window.addEventListener("click", function(event) {
    if (event.target === telega) {
        telega.style.display = "none";
    }
});

async function get_telega(name) {
    const response = await fetch(`https://109.252.15.235:3000/get_telega?name=${encodeURIComponent(name)}`);
    if (!response.ok) {
        throw new Error(`Ошибка: ${response.status}`);
    }
    const data = await response.json();
    return await data.tg;
  }
function open_telega(){
    telega.style.display = "flex";
}