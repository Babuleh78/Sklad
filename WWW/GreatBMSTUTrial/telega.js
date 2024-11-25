const telega = document.getElementById("telega");
const closeModalButton = document.getElementById("closeTelega");
const go_btn = document.getElementById("go_to_telega");
go_btn.addEventListener("click", function(){
    window.open("https://t.me/ForGreatBMSTUTrailBot?start");
});


closeModalButton.addEventListener("click", function() {
    const again_name = document.getElementById("name");
    console.log(again_name.textContent);
    telega.style.display = "none";
});

window.addEventListener("click", function(event) {
    if (event.target === telega) {
        const again_name = document.getElementById("name");
        console.log(again_name.textContent);
        telega.style.display = "none";
    }
});

async function get_telega(name) {
    const response = await fetch(`http://localhost:3000/get_telega?name=${encodeURIComponent(name)}`);
    if (!response.ok) {
        throw new Error(`Ошибка: ${response.status}`);
    }
    const data = await response.json();
    return await data.tg;
  }
    