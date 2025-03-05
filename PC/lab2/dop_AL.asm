.org 100h

PORT_KEYBOARD_STATUS      EQU 0xffe7
PORT_KEYBOARD_DATA        EQU 0xffe8
PORT_LED_DISPLAY          EQU 0xfff8
PORT_INDICATOR            EQU 0xfff9

CMD_READ_KEYBOARD         EQU 0x40
CMD_WRITE_DISPLAY         EQU 0x90

ASCII_ZERO                EQU 0x30
ASCII_ONE                 EQU 0x31
ASCII_NINE                EQU 0x39
ASCII_A                   EQU 0x41
ASCII_MINUS               EQU 0x2D
ASCII_SLASH               EQU 0x2F

PLUS_INDICATOR_POSITION   EQU 1
MINUS_INDICATOR_POSITION  EQU 2 

SCREEN_ELEMENTS_COUNT     EQU 8

start:
    xor bx, bx

choose_operation:
    mov dx, PORT_INDICATOR
    in al, dx

    cmp al, PLUS_INDICATOR_POSITION
    jne check_subtraction
    add bh, bl
    mov al, bh
    jmp display_result

check_subtraction:
    cmp al, MINUS_INDICATOR_POSITION
    jne get_input
    sub bh, bl
    mov al, bh
    jmp display_result

get_input:
    mov dx, PORT_KEYBOARD_STATUS
    mov al, CMD_READ_KEYBOARD
    out dx, al

wait_for_input:
    in al, dx
    and al, 0x0f
    jnz wait_for_input

    mov dx, PORT_KEYBOARD_DATA
    in al, dx

    cmp al, ASCII_ZERO
    jae store_input
    mov cl, SCREEN_ELEMENTS_COUNT
    shl bx, cl
    mov bl, al

store_input:
    cmp al, 0x0f
    jbe  display_result
    jmp choose_operation

display_result:
    mov dx, PORT_LED_DISPLAY
    out dx, al
    mov ah, al

    mov dx, PORT_KEYBOARD_STATUS
    mov al, CMD_WRITE_DISPLAY
    out dx, al

    mov dx, PORT_KEYBOARD_DATA
    mov al, ASCII_SLASH
    out dx, al
    out dx, al

    mov dx, PORT_KEYBOARD_STATUS
    mov al, CMD_WRITE_DISPLAY
    out dx, al

    mov dx, PORT_KEYBOARD_DATA

    cmp ah, 0
    jl handle_negative
    cmp ah, 0x10
    jae handle_big_number
    jmp display_digit

handle_negative:
    mov al, ASCII_MINUS
    out dx, al
    neg ah
    jmp display_digit

handle_big_number:
    mov al, ASCII_ONE
    out dx, al
    sub ah, 0x10

display_digit:
    mov cl, ASCII_ZERO
    cmp ah, 9
    jbe  convert_to_ascii
    mov cl, ASCII_A

convert_to_ascii:
    add ah, cl
    mov al, ah
    out dx, al

    mov dx, PORT_INDICATOR

wait_for_exit:
    in al, dx
    cmp al, 0
    jne wait_for_exit

    jmp choose_operation