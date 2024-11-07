const key = 'F3Eh4Wsa3QzZb0pMVkSZ';
const map = L.map('map').setView([55.75476845750829,37.621749677246086], 11); //starting position
let visitors = 0;


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
let balloons = [myballoonTemplate, myballoonTemplate, myballoonTemplate];
let count = 1;
L.tileLayer(`https://api.maptiler.com/maps/streets-v2/{z}/{x}/{y}.png?key=${key}`,{ //style URL
  tileSize: 512,
  zoomOffset: -1,
  minZoom: 1,
  attribution: "\u003ca href=\"https://www.maptiler.com/copyright/\" target=\"_blank\"\u003e\u0026copy; MapTiler\u003c/a\u003e \u003ca href=\"https://www.openstreetmap.org/copyright\" target=\"_blank\"\u003e\u0026copy; OpenStreetMap contributors\u003c/a\u003e",
  crossOrigin: true
}).addTo(map);

const fetchData = async () => {
  try {
      const response = await fetch('http://localhost:3000/RID'); // Запрос к серверу
      if (!response.ok) {
          throw new Error('Сеть ответила с ошибкой: ' + response.status);
      }
      const data = await response.json(); // Преобразуем ответ в JSON
      console.log('Полученные данные:', data); // Выводим данные в консоль
      for (let i = 0; i < 1; i++) {
        const Element = data[0];
     
        const placeinfo = Element.placeinfo;   
        const coor_x = Element.coor_x; 
        const coor_y = Element.coor_y;  
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
        const marker = L.marker([coor_x, coor_y]) //изменили
            .addTo(map)
            .bindPopup(balloonHTML); // Добавление всплывающего окна
      
        marker.on('popupopen', function() {
            const ZButton = document.getElementById(uniqueZButtonId);
            const visitorCount = document.getElementById(uniqueVisitorCountId);
            const infospan = document.getElementById(uniqueInfoId);
      
            // Устанавливаем уникальный текст для каждого всплывающего окна
            infospan.textContent = placeinfo; //изменили 
      
            ZButton.addEventListener("click", function() {
              visitors++;
              visitorCount.textContent = visitors;
          });
        });
        
      }
  } catch (error) {
      console.error('Ошибка при получении данных:', error);
  }
};

fetchData();