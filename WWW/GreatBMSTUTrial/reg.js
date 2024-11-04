import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-app.js';
  import { getAuth, createUserWithEmailAndPassword } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-auth.js';
  import { getFirestore, doc, setDoc} from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-firestore.js';
  
  const firebaseConfig = {
      apiKey: "AIzaSyCRBI8odC_xkOQWvm3RQByJsqQ1XsrL3WA",
      authDomain: "nightofthelongknives-b66e2.firebaseapp.com",
      projectId: "nightofthelongknives-b66e2",
      storageBucket: "nightofthelongknives-b66e2.appspot.com",
      messagingSenderId: "1085351862604",
      appId: "1:1085351862604:web:c28fbd08e96b9026006fe9",
      measurementId: "G-TRCG7E5HPC"
  };

  const app = initializeApp(firebaseConfig);
  const auth = getAuth(app);    
  const db = getFirestore(app);
  document.getElementById('button_reg').addEventListener('click', function(event) {
      event.preventDefault();
      console.log('Зашлт');
      const nickname = document.getElementById('username_reg').value + "@example.com"; 
      const password = document.getElementById('password_reg').value;
    
      createUserWithEmailAndPassword(auth, nickname, password)
          .then((userCredential) => {
              const user = userCredential.user;
              console.log('Пользователь зарегистрирован:', user);
              user.getIdToken(true).then((idToken) => {
                
                localStorage.clear();
                  localStorage.setItem('firebaseIdTokenReg', idToken);
                //   addFieldsToUser(user.uid);
                window.location.href = "OSM.html"; 
                });
              
          })
          .catch((error) => {
              const errorCode = error.code;
              const errorMessage = error.message;
              console.error('Ошибка регистрации:', errorCode, errorMessage);
              alert('Ошибка: ' + errorMessage);
          });
  });
    async function addFieldsToUser (userId) {
    const userRef = doc(db, "users", userId); // Ссылка на документ пользователя

    console.log("Попытка обновить документ для пользователя:", userId);

    try {
        await setDoc(userRef, {
            rating: 5, // Добавление поля "рейтинг"
            textValue: "Это текстовое значение" // Добавление текстового поля
        }, { merge: true }); // Используйте merge для объединения данных
        console.log("Поля успешно добавлены.");
        
        window.location.href = "OSM.html"; 
    } catch (e) {
        console.error("Ошибка при добавлении полей: ", e);
        alert("Ошибка при добавлении полей: " + e.message); // Уведомление пользователя об ошибке
    }
}