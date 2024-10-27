import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-app.js';
  import { getAuth, createUserWithEmailAndPassword } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-auth.js';

  // Ваш код инициализации Firebase
  
  const firebaseConfig = {
      apiKey: "AIzaSyCRBI8odC_xkOQWvm3RQByJsqQ1XsrL3WA",
      authDomain: "nightofthelongknives-b66e2.firebaseapp.com",
      projectId: "nightofthelongknives-b66e2",
      storageBucket: "nightofthelongknives-b66e2.appspot.com",
      messagingSenderId: "1085351862604",
      appId: "1:1085351862604:web:c28fbd08e96b9026006fe9",
      measurementId: "G-TRCG7E5HPC"
  };

  // Инициализация приложения Firebase
  const app = initializeApp(firebaseConfig);
  const auth = getAuth(app); 

  document.getElementById('button_reg').addEventListener('click', function(event) {
      event.preventDefault();

      const email = document.getElementById('username_reg').value + "@example.com"; 
      const password = document.getElementById('password_reg').value;

      // Создание пользователя
      createUserWithEmailAndPassword(auth, email, password)
          .then((userCredential) => {
              const user = userCredential.user;
              console.log('Пользователь зарегистрирован:', user);
              alert('Регистрация успешна!');
          })
          .catch((error) => {
              const errorCode = error.code;
              const errorMessage = error.message;
              console.error('Ошибка регистрации:', errorCode, errorMessage);
              alert('Ошибка: ' + errorMessage);
          });
  });