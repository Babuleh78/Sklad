const modalUser  = document.getElementById("user_info");
const modalJournal = document.getElementById("user_journal");
const modalinfo = document.getElementById("info");
const userBtn = document.getElementById("open_user");
const journalBtn = document.getElementById("open_journal");
const infoBtn = document.getElementById("open_info");
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


window.onclick = function(event) {
    switch(event.target){
        case modalUser:
            closeModal(modalUser);
        case modalJournal:
            closeModal(modalJournal);
        case modalinfo:
            closeModal(modalinfo);

        break;
    }
}

closeAll = function(){
    closeModal(modalinfo);
    closeModal(modalJournal);
    closeModal(modalUser);
}
closeAll();