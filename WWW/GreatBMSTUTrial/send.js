
document.getElementById('myFile').addEventListener('change', function(event) {
    const file = event.target.files[0]; 
    if (file) {
        const reader = new FileReader(); 

        reader.onload = async function(e) {
            const base64String = e.target.result; 
            console.log(base64String); 
            await set_image("Шашуро", base64String);
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