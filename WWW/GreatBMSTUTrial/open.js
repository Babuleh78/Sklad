const modalUser  = document.getElementById("user_info");
const modalJournal = document.getElementById("user_journal");
const modalinfo = document.getElementById("info");
const modalach = document.getElementById("achivements");
const userBtn = document.getElementById("open_user");
const journalBtn = document.getElementById("open_journal");
const infoBtn = document.getElementById("open_info");
const achBtn = document.getElementById("open_achivements");
function openModal(modal) {
        modal.style.display = "block";
}
function closeModal(modal) {
    modal.style.display = "none";
}
userBtn.onclick = function() {
    if (modalUser.style.display === "block") {
        modalUser.style.display = "none";
    } else {
        closeAll();
        openModal(modalUser);
    }
}

journalBtn.onclick = function() {
    
    updateDisplay();    
    if (modalJournal.style.display === "block") {
        modalJournal.style.display = "none";
    } else {
        closeAll();
        openModal(modalJournal);
    }
}
infoBtn.onclick = function(){
    if (modalinfo.style.display === "block") {
        modalinfo.style.display = "none";
    } else {
        closeAll();
        openModal(modalinfo);
    }
}
achBtn.onclick = function(){
    if (modalach.style.display === "block") {
        modalach.style.display = "none";
    } else {
        closeAll();
        openModal(modalach);
    }
}


window.onclick = function(event) {
    switch(event.target){
        case modalUser:
            closeModal(modalUser);
        case modalJournal:
            closeModal(modalJournal);
        case modalinfo:
            closeModal(modalinfo);
        case modalach:
            closeModal(modalach);
        break;
    }
}

closeAll = function(){
    closeModal(modalinfo);
    closeModal(modalJournal);
    closeModal(modalUser);
    closeModal(modalach);
}
closeAll();