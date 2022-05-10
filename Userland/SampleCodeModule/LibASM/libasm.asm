GLOBAL sys_read 
GLOBAL sys_write 
GLOBAL sys_get_char
GLOBAL sys_get_time
GLOBAL sys_screen_divition
GLOBAL sys_screen_clear
GLOBAL sys_memory_dump
GLOBAL sys_print_user
GLOBAL sys_print_sudoku_numbers
GLOBAL sys_print_in_screen_position
GLOBAL sys_screen_mode
GLOBAL sys_get_milli_seconds
GLOBAL sys_print_regs
GLOBAL div_zero
GLOBAL throwInvalidOpcode
GLOBAL sys_alloc
GLOBAL sys_free
GLOBAL sys_create_process
GLOBAL sys_get_ticks
GLOBAL sys_change_process_priority
GLOBAL sys_change_process_state
GLOBAL sys_exit

;-----------------------------------------------------------
; sys_read - lee de file descriptor
;-----------------------------------------------------------
sys_read:
    push rbp           
    mov rbp, rsp     
    push rbx

    mov rax, 0       
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_write - escribe de file descriptor
;-----------------------------------------------------------
sys_write:              
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 1        
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

;-----------------------------------------------------------
; sys_get_char - retorna el char
;-----------------------------------------------------------
sys_get_char:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 2
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_get_time - devuelve la hora
;-----------------------------------------------------------
sys_get_time:
    push rbp
    mov rbp, rsp

    mov rax, 3
    int 80h

    mov rsp, rbp 
    pop rbp
    ret


;-----------------------------------------------------------
; sys_screen_divition - divide la pantalla
;-----------------------------------------------------------
sys_screen_divition:
    push rbp
    mov rbp, rsp

    mov rax, 4
    int 80h

    mov rsp, rbp 
    pop rbp
    ret


;-----------------------------------------------------------
; sys_screen_clear - limpia la pantalla
;-----------------------------------------------------------
sys_screen_clear:
    push rbp
    mov rbp, rsp

    mov rax, 5
    int 80h

    mov rsp, rbp 
    pop rbp
    ret

;-----------------------------------------------------------
;sys_memory_dump - Makes a 32 Bytes memory dump to 
; screen from the address passed by argument
;-----------------------------------------------------------
sys_memory_dump:
	push rbp
	mov rbp, rsp
	
    mov rax, 6
	int 80h 

	leave
	ret


;-----------------------------------------------------------
; sys_print_user - imprime el usuario
;-----------------------------------------------------------
sys_print_user:              
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 7      
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_print_sudoku_numbers - imprime los numeros del sudoku
;-----------------------------------------------------------
sys_print_sudoku_numbers:              
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 8   
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_print_in_screen_position - imprime en pantalla en la 
; posicion que se le indica            
;-----------------------------------------------------------
sys_print_in_screen_position:              
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 9   
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_screen_mode - me dice en que pantalla estoy
;-----------------------------------------------------------
sys_screen_mode:
	push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 10  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


;-----------------------------------------------------------
; sys_get_milli_seconds - retorna los milisegundos que pasron
;-----------------------------------------------------------
sys_get_milli_seconds:
	push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 11  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret


sys_print_regs:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 12  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_alloc:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 13  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_free:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 14  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_create_process:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 15  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_get_ticks:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 16  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_change_process_priority:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 17  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_change_process_state:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 18  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret

sys_exit:
    push rbp     
    mov rbp, rsp     
    push rbx

    mov rax, 19  
    int 80h

    pop rbx
    mov rsp, rbp     
    pop rbp     
    ret



;-----------------------------------------------------------
; div_zero - Excepcion de dividir por cero
;-----------------------------------------------------------
div_zero:
    xor rax, rax
    div rax
    ret


;-----------------------------------------------------------
; throwInvalidOpcode
;-----------------------------------------------------------
throwInvalidOpcode:
  ud2
  ret