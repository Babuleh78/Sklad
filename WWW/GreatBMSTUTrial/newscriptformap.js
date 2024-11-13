
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
let counthse = -1;
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
L.tileLayer(`https://api.maptiler.com/maps/streets-v2/{z}/{x}/{y}.png?key=${key}`,{ 
  tileSize: 512,
  zoomOffset: -1,
  minZoom: 1,
  attribution: "\u003ca href=\"https://www.maptiler.com/copyright/\" target=\"_blank\"\u003e\u0026copy; MapTiler\u003c/a\u003e \u003ca href=\"https://www.openstreetmap.org/copyright\" target=\"_blank\"\u003e\u0026copy; OpenStreetMap contributors\u003c/a\u003e",
  crossOrigin: true
}).addTo(map);

const fetchData = async () => {
  try {
      const response = await fetch('http://localhost:3000/RID'); 
      if (!response.ok) {
          throw new Error('Сеть ответила с ошибкой: ' + response.status);
      }
      const data = await response.json(); 
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
      
            marker.on('popupopen', async function() {
              const ZButton = document.getElementById(uniqueZButtonId);
              const visitorCount = document.getElementById(uniqueVisitorCountId);
              const infospan = document.getElementById(uniqueInfoId);
              const userName = document.getElementById("name").textContent;
              console.log(userName);
              const placeId = parseInt(ZButton.id[ZButton.id.length - 1]) + 1;
              try {
                  const visitData = await checkVisit(userName, placeId);
                  isVisit = visitData.success ? visitData.is_visit : 0;
      
                  counthse = await getVisitCount(placeId);
                  visitorCount.textContent = counthse;
                  console.log(counthse);
              } catch (error) {
                  console.error('Ошибка:', error);
              }
            infospan.textContent = placeinfo; 
      
            ZButton.addEventListener("click", async function() {
              if(isVisit == 0){
                  isVisit = 1;  
                  await setVisit(userName, placeId);
                  await addNote(userName, placeId);
                 
              }
                
          });
        });
        
      }
  } catch (error) {
      console.error('Ошибка при получении данных:', error);
  }
};
async function addNote(username, placeId) {
    const response = await fetch('http://localhost:3000/addNote', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ username, placeId })
    });
    if (!response.ok) {
        throw new Error('Ошибка при проверке посещения');
    }
    return await response.json();
}
async function checkVisit(username, placeId) {
  const response = await fetch('http://localhost:3000/check_visit', {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json'
      },
      body: JSON.stringify({ username, placeId })
  });
  if (!response.ok) {
      throw new Error('Ошибка при проверке посещения');
  }

  return await response.json();
}

async function setVisit(username, placeId) {
  const response = await fetch('http://localhost:3000/visit', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({username: username, placeId: placeId})

  });
  if (!response.ok) {
    throw new Error('Ошибка setVisit');
  }

    return response.json();
  
}

async function getVisitCount(placeId) {
  const response = await fetch(`http://localhost:3000/get_visit_count?placeId=${encodeURIComponent(placeId)}`);
  if (!response.ok) {
      throw new Error(`Ошибка: ${response.status}`);
  }
  const data = await response.json();
  console.log(data);
  return await data.count;
}
fetchData();