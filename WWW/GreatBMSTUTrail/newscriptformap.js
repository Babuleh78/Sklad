const key = 'F3Eh4Wsa3QzZb0pMVkSZ';
const map = L.map('map').setView([55.75476845750829,37.621749677246086], 11); //starting position

let info = ["Раковая опухоль на теле Строгино. Таллинская 34. Прознав о нахождении в Строгино штаб-квартиры агента, ВШивые студенты возвели напротив \
    это убожество архитектуры под названием МИЭМ",
    "Да-да, она существует, военная кафедра вшэ, что сказать, служу России"
];    

L.tileLayer(`https://api.maptiler.com/maps/streets-v2/{z}/{x}/{y}.png?key=${key}`,{ //style URL
  tileSize: 512,
  zoomOffset: -1,
  minZoom: 1,
  attribution: "\u003ca href=\"https://www.maptiler.com/copyright/\" target=\"_blank\"\u003e\u0026copy; MapTiler\u003c/a\u003e \u003ca href=\"https://www.openstreetmap.org/copyright\" target=\"_blank\"\u003e\u0026copy; OpenStreetMap contributors\u003c/a\u003e",
  crossOrigin: true
}).addTo(map);
const testdrive =`
    <div class="balloon">
        <div class="balloon_info"> ${info[0]}</div>
        <div class="balloon_num">Число посетителей
          <span id="visitorCount">0</span>
        </div>
            <div class = "difficult">
              <div class = "star-box">
                Сложность
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star"></i>
                <i class="fas fa-star"></i>
              </div>
            </div>
            <button id = "ZButton" class="ZButton">
              <div class = "ZButton_text">ЛИКВИДИРОВАТЬ</div>
            </button>
        </div>

`
const marker = L.marker([55.7558, 37.6173]) // Широта и долгота маркера
.addTo(map)
.bindPopup(testdrive); // Добавление всплывающего окна

    