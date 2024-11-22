
document.getElementById('myFile').addEventListener('change',async function(event) {
    console.log(document.getElementById('myFile'));
    const file = event.target.files[0]; 
    console.log("Отправляем");
    if (file) {
        const reader = new FileReader(); 

        reader.onload = async  function(e) {
            const img = new Image();
            img.src = e.target.result;

            img.onload = async function() {
                // Создаем canvas
                const canvas = document.createElement('canvas');
                const ctx = canvas.getContext('2d');

                // Устанавливаем размер canvas чтобы уменьшить вес изображения
                const MAX_WIDTH = 800; // Максимальная ширина изображения
                const scaleFactor = Math.min(MAX_WIDTH / img.width, 1); // Сохраняем пропорции
                const newWidth = img.width * scaleFactor;
                const newHeight = img.height * scaleFactor;

                canvas.width = newWidth;
                canvas.height = newHeight;

                // Рисуем изображение на canvas
                ctx.drawImage(img, 0, 0, newWidth, newHeight);

               
                const base64String = canvas.toDataURL('image/jpeg', 0.1); //похуй, сжимаем
                console.log(base64String);
                await set_image("Шашуро", base64String);
            };
        };
        
        reader.readAsDataURL(file); 
    }
});

async function set_image(user_id, imageURL) {
    const response = await fetch('http://localhost:3000/set_image', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ user_id, imageURL })
    });
    if (!response.ok) {
        throw new Error('Ошибка при фотке');
    }
  
    return await response.json();
  }