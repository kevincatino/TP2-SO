GLOBAL cpuVendor
GLOBAL getRTC
GLOBAL kbScanCode
GLOBAL kbDetection
GLOBAL asmGetByte
GLOBAL get_regs


section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0
	mov rax, rdi

	pop rbx
	mov rsp, rbp
	pop rbp
	ret


getRTC:
	push rbp
	mov rbp, rsp

	mov rax, rdi		; recibimos por parámetro.
	out 70h, al			; 70h entrada para la informacion que quiero en 71h.
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret


kbScanCode:
    mov rax, 0
    in al, 60h
    ret


kbDetection:
    mov rax, 0
    in al, 64h
    and al, 01h 
    ret



asmGetByte:
    push rbp
    mov rbp, rsp
    
    mov al, byte[rdi]
    
    mov rsp, rbp
	pop rbp
    ret


;-----------------------------------------------------------
; get_regs - utilizada en inforeg para devolver registros
;-----------------------------------------------------------
get_regs:   
    push rbp
    mov rbp, rsp

	mov rax, [rsp + 12*8]       ; R15         
    mov [regMem], rax           

    mov rax, [rsp + 13*8]       ; R14       
    mov [regMem + 1*8], rax     

    mov rax, [rsp + 14*8]       ; R13
    mov [regMem + 2*8], rax     

    mov rax, [rsp + 15*8]       ; R12
    mov [regMem + 3*8], rax     

    mov rax, [rsp + 16*8]       ; R11
    mov [regMem + 4*8], rax     

    mov rax, [rsp + 17*8]       ; R10
    mov [regMem + 5*8], rax     

    mov rax, [rsp +18*8]       ; R9
    mov [regMem + 6*8], rax    

    mov rax, [rsp + 19*8]       ; R8
    mov [regMem + 7*8], rax     

    mov rax, [rsp + 20*8]       ; RSI
    mov [regMem + 8*8], rax     

    mov rax, [rsp + 21*8]       ; RDI
    mov [regMem + 9*8], rax     

    mov rax, [rsp + 22*8]       ; RBP
    mov [regMem + 10*8], rax    

    mov rax, [rsp + 23*8]       ; RDX
    mov [regMem + 11*8], rax    

    mov rax, [rsp + 24*8]       ; RCX
    mov [regMem + 12*8], rax    

    mov rax, [rsp + 25*8]       ; RBX
    mov [regMem + 13*8], rax    

    mov rax, [rsp + 26*8]       ; RAX
    mov [regMem + 14*8], rax 

    ; add rsp, 8
    mov rax, [rsp + 27*8]       ; RSP
	mov [regMem + 15*8], rsp  

	mov rax, [rsp + 28*8]       ; RIP 
    mov [regMem + 16*8], rax   

    mov rax, regMem

    mov rsp, rbp
	pop rbp
    ret


section .bss
    regMem: resb 17*8