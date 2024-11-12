import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-app.js';
  import { getAuth, signInWithEmailAndPassword} from 'https://www.gstatic.com/firebasejs/9.1.0/firebase-auth.js';
  

  
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

  document.getElementById('button_log').addEventListener('click', function(e) {
    e.preventDefault(); 
    const email = document.getElementById('username_log').value + "@example.com";
    const password = document.getElementById('password_log').value;

    signInWithEmailAndPassword(auth, email, password) 
        .then((userCredential) => {
            const user = userCredential.user;
            user.getIdToken(true).then((idToken) => {
                localStorage.clear();
                localStorage.setItem('firebaseIdTokenLog', idToken);
                window.location.href = "OSM.html"; 
              });
        })
        .catch((error) => {
            const errorMessage = error.message;
            console.log(errorMessage);
        });

  });