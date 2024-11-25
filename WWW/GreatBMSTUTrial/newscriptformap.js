
const key = 'F3Eh4Wsa3QzZb0pMVkSZ';
const map = L.map('map').setView([55.75476845750829,37.621749677246086], 11); //starting position
let visitors = 0;
let stars_mas = [];
async function get_stars_count() {
  try{
    const response = await fetch(`http://localhost:3000/get_stars_count`);
    if(!response.ok){
      throw new Error(`Ошибка: ${response.status}`);
    }
    const data = await response.json();
    return await data.stars;
  } catch (error) {
    console.error("Error fetching data:", error.message); 
  }
  }(async () => {
    stars_mas = await get_stars_count();
  })();
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
        const uniqueZButtonTextId = `ZButtonText-${i}`;
        const uniqueVisitorCountId = `visitorCount-${i}`;
        const uniqueInfoId = `info-${i}`;
        const uniqueZagrId = `zagr-${i}`;
        const starCount = stars_mas[i].stars;
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
                      ${getStarsHTML(starCount)}
                  </div>
              </div>
              <button id="${uniqueZButtonId}" class="ZButton">
                  <div id = "${uniqueZButtonTextId}" class="ZButton_text">ЛИКВИДИРОВАТЬ</div>
              </button>
              
                <input id="${uniqueZagrId}"  name="myFile" type="file" accept="image/*" style= "display:none;">
              
          <script src = "send.js"> </script>
          
    
          </div>
          </div>
      `;
        const marker = L.marker([coor_x, coor_y]) //изменили
            .addTo(map)
            .bindPopup(balloonHTML);
      
            marker.on('popupopen', async function() {
              const ZButton = document.getElementById(uniqueZButtonId);
              const visitorCount = document.getElementById(uniqueVisitorCountId);
              const infospan = document.getElementById(uniqueInfoId);
              const userName = document.getElementById("name").textContent;
              const uid = await getId(userName);
              
              const placeId = Number(ZButton.id.split('-')[1])+1;
              try {
                  const visitData = await checkVisit(userName, placeId);
                  isVisit = visitData.success ? visitData.is_visit : 0;
                  if(isVisit === 1){

                    
                    const hse_text = document.getElementById(uniqueZButtonTextId);
                    hse_text.textContent = "ЛИКВИДИРОВАНА";
                    hse_text.style.color = "black"; 
                    hse_text.style.fontSize = "24px"; 
                    hse_text.style.textDecoration = "line-through";
                    ZButton.style.background = "red";
                   
                  } else if(isVisit === -1){//На проверке
                    const hse_text = document.getElementById(uniqueZButtonTextId);
                    hse_text.textContent = "Отправлено"
                    hse_text.style.fontSize = "24px";
                    hse_text.style.background = "blue";
                    ZButton.style.background = "white";
                  } else if(isVisit === 78){//Прошло проверку, необходимо внести всю информацию на сервер
                         isVisit = 1; 
                  const count_ach = await (get_count_for_ach(uid))+1;
                  
                  if(Number(stars.textContent) === 0){ //Достижение Первые шаги
                   
                    await set_ach(uid, 2);
                    await updateDisplayAch(2);
                  }
                  if(count_ach >= 5){//Достижение В яблочко
                    await set_ach(uid, 3);
                    await updateDisplayAch(3);
                  }
                  if(count_ach >= 10){//Достижение Подержи мое пиво
                    await set_ach(uid, 4);
                    
                    await updateDisplayAch(4);
                  }
                  
                  if(ZButton.id == "ZButton-6"){//Достижение Наш район
                    await set_ach(uid, 1); 
                    await updateDisplayAch(1);
                }
                  await setVisit(userName, placeId);
                  await addNote(userName, placeId);
                  visitorCount.textContent = Number(visitorCount.textContent) +1;
                  const hse_text = document.getElementById(uniqueZButtonTextId);
                  hse_text.textContent = "ЛИКВИДИРОВАНА";
                  hse_text.style.color = "black"; 
                  hse_text.style.fontSize = "24px"; 
                  hse_text.style.textDecoration = "line-through";
                  ZButton.style.background = "red";
                  
                    async function get_hse_count_for_user(userName) {
                    try {
                        const response = await fetch(`http://localhost:3000/get_hse_count_for_user?userName=${encodeURIComponent(userName)}`);
                        if (!response.ok) {
                            throw new Error(`Ошибка: ${response.status}`);
                        }
                        const data = await response.json();
                        count = data.hse_count_user; 
                        stars.textContent = count;
                        console.log(count);
                        return count; 
                    } catch (error) {
                        console.error(error.message);
                        return -1; 
                    }
                }
                 
                  await get_hse_count_for_user(userName); 
                  if(Number(stars.textContent) >= 25){//Достижение звездный лорд
                    await set_ach(uid, 6);
                    await updateDisplayAch(6);
                  }  
                  }
                  counthse = await getVisitCount(placeId);
                  visitorCount.textContent = counthse;
              } catch (error) {
                  console.error('Ошибка:', error);
              }
            infospan.textContent = placeinfo; 
      
            ZButton.addEventListener("click", async function() {
              const window = document.getElementById(uniqueZagrId);
              if(isVisit === 0 && window.style.display !=="flex" ){
                    window.style.display = "flex";
                      const hse_text = document.getElementById(uniqueZButtonTextId);
                    
                    window.addEventListener('change',async function(event) {
                      await visit_on_check(userName, placeId)
                      const file = event.target.files[0]; 
                      hse_text.textContent = "Отправлено"
                      hse_text.style.fontSize = "24px";
                      hse_text.style.background = "blue";
                      hse_text.style.color = "white";
                      if (file) {
                          const reader = new FileReader(); 
                  
                          reader.onload = async  function(e) {
                              const img = new Image();
                              img.src = e.target.result;
                  
                              img.onload = async function() {
                                  const canvas = document.createElement('canvas');
                                  const ctx = canvas.getContext('2d');
                                  const MAX_WIDTH = 800; 
                                  const scaleFactor = Math.min(MAX_WIDTH / img.width, 1); // Сохраняем пропорции
                                  const newWidth = img.width * scaleFactor;
                                  const newHeight = img.height * scaleFactor;
                  
                                  canvas.width = newWidth;
                                  canvas.height = newHeight;
                                  ctx.drawImage(img, 0, 0, newWidth, newHeight);
              
                                  const base64String = canvas.toDataURL('image/jpeg', 0.1); //похуй, сжимаем
                                  await set_image(userName, base64String, uid, placeId);
                              };
                          };
                          
                          reader.readAsDataURL(file); 
                      }
                  });  
                
              } 
                
          });
        });
        
      }
  } catch (error) {
      console.error('Ошибка при получении данных:', error);
  }
};

function getStarsHTML(count) {
  let starsHTML = '';
  for (let i = 0; i < 5; i++) {
      if (i < count) {
          starsHTML += '<i class="fas fa-star checked"></i>';
      } else {
          starsHTML += '<i class="fas fa-star"></i>';
      }
  }
  return starsHTML;
}



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
async function set_ach(uid, id) {
  const response = await fetch('http://localhost:3000/set_ach', {
    method: 'POST',
    headers: {
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ uid, id })
  });
  if (!response.ok) {
      throw new Error('Ошибка при постановке ачивки');
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

async function visit_on_check(username, placeId) {
  const response = await fetch('http://localhost:3000/visit_on_check', {
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
  return await data.count;
}

async function get_count_for_ach(userId) {
  const response = await fetch(`http://localhost:3000/get_count_for_ach?id=${encodeURIComponent(userId)}`);
  if (!response.ok) {
      throw new Error(`Ошибка: ${response.status}`);
  }
  const data = await response.json();
  return await data.count;
}


async function set_image(user_name, imageURL, user_id, place_id) {
  const response = await fetch('http://localhost:3000/set_image', {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json'
      },
      body: JSON.stringify({ user_name, imageURL, user_id, place_id })
  });
  if (!response.ok) {
      throw new Error('Ошибка при фотке');
  }

  return await response.json();
}

fetchData();