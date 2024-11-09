const modalUser  = document.getElementById("user_info");
const modalJournal = document.getElementById("user_journal");
const userBtn = document.getElementById("open_user");
const journalBtn = document.getElementById("open_journal");
const closeUserBtn = document.getElementById("close_user");
const closeJournalBtn = document.getElementById("close_journal");

function openModal(modal) {
    modal.style.display = "block";
}

function closeModal(modal) {
    modal.style.display = "none";
}
userBtn.onclick = function() {
    openModal(modalUser);
}

journalBtn.onclick = function() {
    openModal(modalJournal);
}

closeUserBtn.onclick = function() {
    closeModal(modalUser );
}

closeJournalBtn.onclick = function() {
    closeModal(modalJournal);
}

window.onclick = function(event) {
    switch(event.target){
        case modalUser:
            closeModal(modalUser);
        case modalJournal:
            closeModal(modalJournal);
    }
}