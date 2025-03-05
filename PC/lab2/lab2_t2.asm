org 100h 

;mov dx, 0xffeah
;mov al, 00h
;out dx, al  

main:
    mov dx, 0xffe7h ;0xffeah
    mov al, 0x0090h
    out dx, al
    mov cx, 0x0008h
    mov bx, 0x0200h 
    mov dx, 0xffe8h
m:
    mov al, [bx]
    out dx, al
    inc bx
    loop m