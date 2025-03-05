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
    mov bl, al
    
    mov dx, 0xffe7h
    mov al, 0x0090h
    out dx, al
    cmp bl, 1
    je output_a
    cmp bl, 2
    je output_b
    cmp bl, 3
    je output_c
    jmp m
            
            
output_a:
    mov bx, 0x200h
    mov dx, 0xffe8h 
    mov cx, 8
loop_a:  
    mov al, [bx]
    out dx, al
    inc bx
    loop loop_a
    jmp m
    
output_b:
    mov bx, 0x209h 
    mov dx, 0xffe8h 
    mov cx, 8
loop_b: 
    mov al, [bx]
    out dx, al
    inc bx
    loop loop_b
    jmp m
    
output_c:
    mov bx, 0x217h 
    mov dx, 0xffe8h 
    mov cx, 8
loop_c:
    mov al, [bx]
    out dx, al
    inc bx
    loop loop_c
    jmp m    
    
    
proc delay
    push cx
    mov cx, 100    
    cyc1:    
    mov dx, cx
    mov cx, 0xfffh
    cyc2:
    loop cyc2
    mov cx, dx
    loop cyc1
    pop cx
    xor ch, ch
    ret 
endp delay


