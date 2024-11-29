async function get_all_j() {
    try {
        const response = await fetch(`http://localhost:3000/getNote`);
        if (!response.ok) {
            throw new Error(`Ошибка: ${response.status}`);
        }
        const data = await response.json();
        console.log(data);
        return data.all;
    } catch (error) {
        console.error(error.message);
        return -1; 
    }
}
get_all_j();