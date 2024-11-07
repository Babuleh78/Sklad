const key = 'F3Eh4Wsa3QzZb0pMVkSZ';
const map = L.map('map').setView([55.75476845750829,37.621749677246086], 11); //starting position
let visitors = 0;
class Hamster{
  constructor(is_tap, all_count){
    this.is_tap = is_tap;
    this.all_count = all_count;
  }
}
class information{
  constructor(info_, number_){
      this.info = info_;
      this.number = number_;
  }
}
const myballoonTemplate = `
    <div class="balloon" id="balloon">
        <div class="balloon_info"> 
          <span id="info">текст</span>
        </div>
        <div class="balloon_num">Число посетителей
          <span id="visitorCount">0</span>
        </div>
        <div class="difficult">
            <div class="star-box">
                Сложность
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star"></i>
                <i class="fas fa-star"></i>
            </div>
        </div>
        <button id="ZButton" class="ZButton">
            <div class="ZButton_text">ЛИКВИДИРОВАТЬ</div>
        </button>
    </div>
`;

// Создаем массив с уникальными balloon-ами
let balloons = [myballoonTemplate, myballoonTemplate, myballoonTemplate];
let coor = [[55.7558, 37.6173], [55.8033638275589,37.409635388619094], [55.754137466243,37.64899523354594]];
let count = coor.length; 
let info = ["Раковая опухоль на теле Строгино. Таллинская 34. Прознав о нахождении в Строгино штаб-квартиры агента, ВШивые студенты возвели напротив \
    это убожество архитектуры под названием МИЭМ",
    "Да-да, она существует, военная кафедра вшэ, что сказать, служу России",
    "еще какая-нибудь хуйня"
];    
L.tileLayer(`https://api.maptiler.com/maps/streets-v2/{z}/{x}/{y}.png?key=${key}`,{ //style URL
  tileSize: 512,
  zoomOffset: -1,
  minZoom: 1,
  attribution: "\u003ca href=\"https://www.maptiler.com/copyright/\" target=\"_blank\"\u003e\u0026copy; MapTiler\u003c/a\u003e \u003ca href=\"https://www.openstreetmap.org/copyright\" target=\"_blank\"\u003e\u0026copy; OpenStreetMap contributors\u003c/a\u003e",
  crossOrigin: true
}).addTo(map);

for (let i = 0; i < count; i++) {
  // Создаем уникальные идентификаторы для каждого маркера
  const uniqueId = `popup-${i}`;
  const uniqueZButtonId = `ZButton-${i}`;
  const uniqueVisitorCountId = `visitorCount-${i}`;
  const uniqueInfoId = `info-${i}`;

  const balloonHTML = `
    <div class="balloon" id="${uniqueId}">
        <div class="balloon_info"> 
          <span id="${uniqueInfoId}">текст</span>
        </div>
        <div class="balloon_num">Число посетителей
          <span id="${uniqueVisitorCountId}">0</span>
        </div>
        <div class="difficult">
            <div class="star-box">
                Сложность
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star checked"></i>
                <i class="fas fa-star"></i>
                <i class="fas fa-star"></i>
            </div>
        </div>
        <button id="${uniqueZButtonId}" class="ZButton">
            <div class="ZButton_text">ЛИКВИДИРОВАТЬ</div>
        </button>
    </div>
`;

  const marker = L.marker(coor[i])
      .addTo(map)
      .bindPopup(balloonHTML); // Добавление всплывающего окна

  marker.on('popupopen', function() {
      const ZButton = document.getElementById(uniqueZButtonId);
      const visitorCount = document.getElementById(uniqueVisitorCountId);
      const infospan = document.getElementById(uniqueInfoId);

      // Устанавливаем уникальный текст для каждого всплывающего окна
      infospan.textContent = info[i]; // Убедитесь, что info[i] существует

      ZButton.addEventListener("click", function() {
          visitors++;
          visitorCount.textContent = visitors;
      });
  });
  
}
const fetchData = async () => {
  try {
      const response = await fetch('http://localhost:3000/RID'); // Запрос к серверу
      if (!response.ok) {
          throw new Error('Сеть ответила с ошибкой: ' + response.status);
      }
      const data = await response.json(); // Преобразуем ответ в JSON
      console.log('Полученные данные:', data); // Выводим данные в консоль

  } catch (error) {
      console.error('Ошибка при получении данных:', error);
  }
};

fetchData();