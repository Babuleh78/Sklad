import { useEffect } from "react";

export default function Timer({ time, setTime, timerActive }) {
    useEffect(() => {
        let interval = null;
        if (timerActive) {
            interval = setInterval(() => {
                setTime((prevTime) => prevTime + 1);
            }, 1000);
        } else {
            clearInterval(interval);
        }
        return () => {
            clearInterval(interval);
        };
    }, [timerActive]); 

    return <p>Time: {time}s</p>;
}