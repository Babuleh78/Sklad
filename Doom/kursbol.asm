.model tiny
.code
.386
org 100h

start:
    jmp init

; Данные программы
old_int09h dd ?         ; Для хранения старого обработчика прерывания клавиатуры
old_int08h dd ?         ; Для хранения старого обработчика таймера
old_int16h dd ?         ; Для хранения старого обработчика int 16h
f1_pressed db 0         ; Флаг нажатия F1
f7_pressed db 0         ; Флаг нажатия F7
timer_counter dw 0      ; Счетчик для таймера
unload_flag db 0        ; Флаг для выгрузки программы
student_info db 'Иванов Иван Иванович, Группа 1234, Вариант 5', 0

; Таблица замены русских букв на латинские
rus_to_lat db 32 dup(?) ; Пропускаем служебные символы
           db 'a', 'b', 'w', 'g', 'd', 'e', 'v', 'z', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
           db 'r', 's', 't', 'u', 'f', 'h', 'c', '~', 'x', 'y', '~'
           db 5 dup(?)  ; Остальные символы
           db 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 'a', 's'
           db 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 39, '`', 'z', 'x', 'c', 'v'
           db 'b', 'n', 'm', ',', '.', '/'

; Новый обработчик прерывания клавиатуры (int 09h)
new_int09h proc far
    pushf
    push ax
    push bx
    push ds
    
    mov ax, cs
    mov ds, ax
    
    in al, 60h          ; Получить скан-код нажатой клавиши
    
    ; Проверка на F1
    cmp al, 3Bh         ; Скан-код F1
    jne check_f7
    cmp f1_pressed, 0
    je set_f1
    mov f1_pressed, 0
    jmp check_unload
set_f1:
    mov f1_pressed, 1
    jmp check_unload

check_f7:
    ; Проверка на F7
    cmp al, 41h         ; Скан-код F7
    jne check_unload
    cmp f7_pressed, 0
    je set_f7
    mov f7_pressed, 0
    jmp check_unload
set_f7:
    mov f7_pressed, 1
    mov timer_counter, 0

check_unload:
    ; Проверка на Ctrl+U (выгрузка)
    in al, 61h          ; Получить состояние клавиатуры
    mov ah, al
    or al, 80h          ; Установить бит подтверждения
    out 61h, al
    xchg ah, al
    out 61h, al         ; Восстановить исходное состояние
    
    mov al, 0Eh         ; Скан-код Backspace (для проверки Ctrl)
    call get_key_state
    test al, 4          ; Проверить состояние Ctrl
    jz skip_unload_check
    
    mov al, 16h         ; Скан-код U
    call get_key_state
    test al, 128        ; Проверить, отпущена ли клавиша
    jnz skip_unload_check
    
    mov unload_flag, 1  ; Установить флаг выгрузки
    
skip_unload_check:
    pop ds
    pop bx
    pop ax
    popf
    jmp dword ptr cs:old_int09h ; Передать управление старому обработчику
new_int09h endp

; Функция для получения состояния клавиши
; Вход: AL - скан-код
; Выход: AL - состояние клавиши
get_key_state proc
    push es
    mov bx, 40h
    mov es, bx
    mov bx, 17h         ; Адрес состояния клавиш
    mov ah, al
    shr al, 3           ; Получить смещение байта
    and ah, 7           ; Получить номер бита
    add bl, al
    mov al, es:[bx]     ; Получить байт состояния
    mov cl, ah
    shr al, cl          ; Сдвинуть нужный бит в младший разряд
    and al, 1           ; Изолировать бит
    pop es
    ret
get_key_state endp

; Новый обработчик прерывания таймера (int 08h)
new_int08h proc far
    pushf
    push ax
    push ds
    
    mov ax, cs
    mov ds, ax
    
    ; Проверка флага выгрузки
    cmp unload_flag, 1
    je unload_program
    
    ; Обработка F7 (3-секундная задержка)
    cmp f7_pressed, 1
    jne skip_f7_processing
    inc timer_counter
    cmp timer_counter, 3*18 ; 3 секунды (18.2 тика/сек)
    jl skip_f7_processing
    
    ; Вывод информации о студенте
    call show_student_info
    mov f7_pressed, 0
    mov timer_counter, 0

skip_f7_processing:
    pop ds
    pop ax
    popf
    jmp dword ptr cs:old_int08h ; Передать управление старому обработчику

unload_program:
    ; Восстановление старых обработчиков прерываний
    cli
    mov dx, word ptr old_int09h
    mov ds, word ptr old_int09h+2
    mov ax, 2509h
    int 21h
    
    mov dx, word ptr old_int08h
    mov ds, word ptr old_int08h+2
    mov ax, 2508h
    int 21h
    
    mov dx, word ptr old_int16h
    mov ds, word ptr old_int16h+2
    mov ax, 2516h
    int 21h
    sti
    
    ; Освобождение памяти
    mov ax, 3100h
    mov dx, (init - start + 15) / 16 ; Размер резидентной части в параграфах
    int 21h
new_int08h endp

; Процедура для вывода информации о студенте
show_student_info proc
    push ax
    push bx
    push cx
    push dx
    push es
    push si
    push di
    
    mov ax, 0B800h      ; Сегмент видеопамяти
    mov es, ax
    
    ; Вычисляем позицию для вывода (низ экрана)
    mov di, (24*80)*2   ; 24 строка (0-based), 0 колонка
    
    lea si, student_info
    mov ah, 07h         ; Атрибут символа (серый на черном)
    
print_loop:
    lodsb               ; Загрузить символ из строки
    test al, al         ; Проверка на конец строки
    jz print_done
    
    stosw               ; Записать символ и атрибут в видеопамять
    jmp print_loop
    
print_done:
    pop di
    pop si
    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    ret
show_student_info endp

; Новый обработчик прерывания клавиатуры (int 16h)
new_int16h proc far
    cmp ah, 00h         ; Функция чтения символа
    je handle_read
    cmp ah, 01h         ; Функция проверки буфера
    je handle_check
    
    ; Для других функций передаем управление старому обработчику
    jmp dword ptr cs:old_int16h
    
handle_check:
    pushf
    call dword ptr cs:old_int16h
    jz no_key_available
    ; Если есть символ в буфере, проверить нужно ли его заменять
    test cs:f1_pressed, 1
    jz no_translation_needed
    ; Заменить русский символ на латинский
    push bx
    push ds
    push cs
    pop ds
    mov bx, offset rus_to_lat
    sub al, 80h         ; Русские символы начинаются с 80h
    xlatb
    pop ds
    pop bx
no_translation_needed:
    retf 2              ; Выход с сохранением флагов
no_key_available:
    retf 2              ; Выход с сохранением флагов
    
handle_read:
    pushf
    call dword ptr cs:old_int16h
    test cs:f1_pressed, 1
    jz read_done
    ; Заменить русский символ на латинский
    push bx
    push ds
    push cs
    pop ds
    mov bx, offset rus_to_lat
    sub al, 80h         ; Русские символы начинаются с 80h
    xlatb
    pop ds
    pop bx
read_done:
    iret
new_int16h endp

; Инициализация программы
init:
    ; Получить и сохранить старые обработчики прерываний
    mov ax, 3509h       ; Получить вектор прерывания 09h
    int 21h
    mov word ptr old_int09h, bx
    mov word ptr old_int09h+2, es
    
    mov ax, 3508h       ; Получить вектор прерывания 08h
    int 21h
    mov word ptr old_int08h, bx
    mov word ptr old_int08h+2, es
    
    mov ax, 3516h       ; Получить вектор прерывания 16h
    int 21h
    mov word ptr old_int16h, bx
    mov word ptr old_int16h+2, es
    
    ; Установить новые обработчики прерываний
    mov dx, offset new_int09h
    mov ax, 2509h
    int 21h
    
    mov dx, offset new_int08h
    mov ax, 2508h
    int 21h
    
    mov dx, offset new_int16h
    mov ax, 2516h
    int 21h
    
    ; Вывести сообщение о резидентной загрузке
    mov dx, offset init_msg
    mov ah, 09h
    int 21h
    
    ; Оставить резидентную часть и выйти
    mov dx, (init - start + 15) / 16 ; Размер резидентной части в параграфах
    mov ax, 3100h
    int 21h

init_msg db 'Резидентная программа загружена. Горячие клавиши:', 0Dh, 0Ah
         db 'F7 - вывести информацию через 3 секунды', 0Dh, 0Ah
         db 'F1 - переключить режим ввода рус/лат', 0Dh, 0Ah
         db 'Ctrl+U - выгрузить программу', 0Dh, 0Ah, '$'

end start
