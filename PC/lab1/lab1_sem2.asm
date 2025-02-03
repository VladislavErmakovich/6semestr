org 100h

targ:
    mov dx, 0xfff9h
    in al, dx
    cmp al, 128
    je act
    test al, 128
    je dis
    
act:
    cmp bl,1
    je exit
    mov cx, 3
    mov al, 10000000b
    mov dx, 0xfff8h 
    out dx, al
    
loop1:
    mov dx, 0xfff8h
    shr al, 1
    or al, 10000000b
    out dx, al
    call delay1
    loop loop1
    
    mov cx, 4 

loop2:
    mov dx, 0xfff8h
    shl al, 1
    or al, 00010001b
    out dx, al
    call delay1
    loop loop2
    
    mov cl, al
    mov bl, 1
    
exit:
    jmp targ
    
dis:
    cmp bl, 0
    je exit
    mov al, cl
    mov cx, 4
    
loop3:
    mov dx, 0xfff8h
    shr al, 1
    out dx, al
    call delay2
    loop loop3 
    
    mov cx, 4
    
loop4:
    mov dx, 0xfff8h
    shl al,1
    xor al, 00010000b
    out dx, al
    call delay2
    loop loop4
    
    mov bl,0
    jmp exit
    
proc delay1
    push cx
    mov cx, 250
    
    cyc1:
    mov dx, cx
    mov cx, 0xffffh
    cyc2:
    loop cyc2
    mov cx, dx
    loop cyc1
    pop cx
    xor ch, ch
    ret 
endp delay1

proc delay2
    push cx
    mov cx, 500
    
    cyc3:
    mov dx, cx
    mov cx, 0xffffh
    cyc4:
    loop cyc4
    mov cx, dx
    loop cyc3
    pop cx
    xor ch, ch
    ret 
endp delay2   
    
    
    
    
    

