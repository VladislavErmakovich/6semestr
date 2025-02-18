org 100h
main:
    mov dx, 0xffe7h
    mov al, 0x0040h
    out dx, al
m:
    in al, dx
    and al, 0x000fh
    jnz m  
b: 
    mov dx, 0xffe8h 
    in al, dx
    mov dx, 0xfff8h
    mov bl, 10
    sub bl, al
    cmp bl, 10
    jl b
    mov al, bl
    out dx, al
    mov dx, 0xffe7h
    jmp m