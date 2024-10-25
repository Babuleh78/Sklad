
let center = [[55.80312651648279,37.40965577123039],
  [55.81580120650133,37.52485751573193]
];


let info = ["Раковая опухоль на теле Строгино. Таллинская 34. Прознав о нахождении в Строгино штаб-квартиры агента, ВШивые студенты возвели напротив \
этого убожество архитектуры под названием МИЭМ",
"Да-да, она существует, военная кафедра вшэ, что сказать, служу России"


];
let visitors = 0;
const count = 2;
function init() {
    let map = new ymaps.Map('map', {
      center: center[0],
      zoom: 11
    });
    let myballoon  = new ymaps.Placemark(center[0], {
      balloonContent: `
      <div class="balloon">
        <div class="balloon_address"> ${info[0]}</div>
        <div class="balloon_num">Число посетителей
          <span id="visitorCount">0</span>
        </div>
            <div class = "rating-box">
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
    },{
      iconLayout: 'default#image',
      iconImageHref: 'govno.png',
      iconImageSoze: [20, 20],
      iconImageOffset: [-10, -10]
    });
    myballoon.events.add('balloonopen', function () {
      console.log("Пиздец");
      const incrementButton = document.getElementById('ZButton');
      const visitorCountElement = document.getElementById('visitorCount');
      incrementButton.addEventListener('click', function () {
        visitors++;
        visitorCountElement.textContent = visitors;
      });
    });
    map.geoObjects.add(myballoon);
  }

ymaps.ready(init);
