
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
let count = -1;
async function get_hse_count() {
  try {
    const response = await fetch('http://localhost:3000/get_hse_count');
    if (!response.ok) {
        throw new Error(`Ошибка Хуяшибка: ${response.status}`);
    }
    const data = await response.json();
    console.log(data);
    count = data.hse_count;
    return count;
} catch (error) {
    console.error("Error fetching data:", error.message); 
  }
}(async () => {
  count = await get_hse_count();
})();
console.log("получили", count);
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
      for (let i = 0; i < count; i++) {
        const Element = data[i];
     
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
            let isVisit = 0;
            let count = -1;
            const userName = document.getElementById("name").textContent;
            const placeId = parseInt(ZButton.id[ZButton.id.length-1])+1;
            fetch('http://localhost:3000/check_visit', {
              method: 'POST',
              headers: {
                  'Content-Type': 'application/json'
              },
              body: JSON.stringify({username: userName, placeId: placeId })
            })
            .then(response => {
              if (!response.ok) {
                  throw new Error('Хуйня с чеком');
              }
              return response.json();
            })
            .then(data => {
              if (data.success) {
                console.log(data);
                isVisit = data.is_visit;
              } else {
                  console.error(data.message); 
              }
              })
              .catch(error => {
              console.error('Ошибка:', error);
              
              });
              async function get_visit_count() {
                try {
                  const response = await fetch(`http://localhost:3000/get_visit_count?placeId=${encodeURIComponent(i)}`);
                  if (!response.ok) {
                      throw new Error(`Ошибка: ${response.status}`);
                  }
                  const data = await response.json();
                  console.log(data);
                  count = data.visit_count;
                  return count;
              } catch (error) {
                  console.error("Error fetching data:", error.message); 
                }
              }(async () => {
                count = await get_visit_count();
              })();
            infospan.textContent = placeinfo; 
      
            ZButton.addEventListener("click", function() {
              
              
              if(isVisit == 0){
                  isVisit = 1;  
                  fetch('http://localhost:3000/visit', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({ username: userName, placeId: placeId })
                })
                .then(response => {
                    if (!response.ok) {
                        throw new Error('Хуйня с чеком');
                    }
                    return response.json();
                })
                .then(data => {
                    if (data.success) {
                      count+=1;
                      visitorCount.textContent = count;
                  } else {
                      console.error(data.message); 
                  }
                })
                .catch(error => {
                    console.error('Ошибка:', error);              
                });
              }
                
          });
        });
        
      }
  } catch (error) {
      console.error('Ошибка при получении данных:', error);
  }
};

fetchData();