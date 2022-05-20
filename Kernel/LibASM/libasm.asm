GLOBAL cpuVendor
GLOBAL getRTC
GLOBAL kbScanCode
GLOBAL kbDetection
GLOBAL asmGetByte
GLOBAL get_regs
GLOBAL initProcess
GLOBAL forceScheduler
GLOBAL xchg


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


initProcess: ; en rdi recibo la posicion del stackbase y en rsi recibo el puntero a la funcion del proceso
	push rbp
	push rsp
	mov rax, rsp ; me guardo en rax el rsp actual
	
	mov rsp, rdi ; en rdi recibo el stackbase
	sub rdi, 160 ; deja el rsp al final del "stackframe de int", para que luego se haga popstate + iretq y se popee en el orden correcto (160 = 20x8, un qword son 8 bytes)
	mov rbp, rdi

	; <-rsp (los primeros registros son para restaurarlos con el iretq)
	push qword 0x0   ; ss
	push qword rsp   ; rsp
	push qword 0x202 ; rflags
	push qword 0x8   ; cs
	push qword rsi   ; rip
	; registros generales que se restauran con el popState
	push qword 0x1
	push qword 0x2
	push qword 0x3
	push qword 0x4
	push qword 0x5
	push qword rdx ; argc (queda en rdi al hacer pop)
	push qword rcx ; argv (queda en rsi al hacer pop)
	push qword 0x6
	push qword 0x7
	push qword 0x8
	push qword 0x9
	push qword 0xA
	push qword 0xB
	push qword 0xC
	push qword 0xD

	mov rsp, rax ; restauro el rsp previo todo: revisar si esta demas
	mov rax, rbp ; en rax debe quedar el nuevo stack pointer desde el cual se restaura el contexto luego
	pop rsp 
	pop rbp
	ret

forceScheduler:	
	sti
	int 20h

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



xchg:
	mov rax, rsi 
	xchg [rdi], eax
	ret


section .bss
    regMem: resb 17*8