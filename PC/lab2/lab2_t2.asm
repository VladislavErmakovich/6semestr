org 100h 


main:
    mov dx, 0xffe7h
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