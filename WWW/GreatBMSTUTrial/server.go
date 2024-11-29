package main

import (
	"net/http"
)

func main() {
	http.Handle("/", http.FileServer(http.Dir(".")))
	http.HandleFunc("/LogIn", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "LogIn.html")
	})
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		panic(err)
	}
}
