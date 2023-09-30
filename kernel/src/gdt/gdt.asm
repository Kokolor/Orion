section .data
align 4

gdt_start:
        dq 0x0000000000000000  ; NULL descriptor
gdt_code_ring0:
        dq 0x00af9a000000ffff  ; Code segment descriptor for ring 0
gdt_data_ring0:
        dq 0x008f92000000ffff  ; Data segment descriptor for ring 0
gdt_code_ring3:
        dq 0x00affa000000ffff  ; Code segment descriptor for ring 3
gdt_data_ring3:
        dq 0x008ff2000000ffff  ; Data segment descriptor for ring 3

gdt_tss:
        dw 0x67  ; Limit (TSS size)
        dw 0        ; Base address (low)
        db 0        ; Base address
        db 0x89  ; Type (TSS available)
        db 0        ; Limit (high)
        db 0        ; Base address (high)
        dd 0        ; Base address (higher)
        dd 0        ; Reserved
gdt_end:

align 4
tss:
        times 104 db 0
        dd 0
        dq 0
        times 40 db 0

section .text

global init_gdt
init_gdt:
        mov ax, gdt_end - gdt_start - 1
        mov [gdtr + 0], ax
        mov rax, gdt_start
        mov [gdtr + 2], rax

        lgdt [gdtr]

        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        mov ax, gdt_tss - gdt_start
        ltr ax

        mov rax, rsp
        mov [tss + 4], rax

        ret

section .data
align 4
gdtr:
        dw 0
        dq 0