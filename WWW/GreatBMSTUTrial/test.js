async function fetchData() {
    try {
        const response = await fetch('https://109.252.15.235:3000/RID'); 
        if (!response.ok) {
            throw new Error('Сеть ответила с ошибкой: ' + response.status);
        }
        
        const data = await response.json(); 
        console.log(data);
    }
    catch (error) {
        console.error('Произошла ошибка:', error);
    }
}

fetchData(); // Вызов функции
