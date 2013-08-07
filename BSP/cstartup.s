;
;********************************************************************************************************
;                                    EXCEPTION VECTORS & STARTUP CODE
;
; File      : cstartup.s
; For       : ARM7 or ARM9
; Toolchain : IAR EWARM V5.10 and higher
;********************************************************************************************************
;
	
;********************************************************************************************************
;                                           MACROS AND DEFINIITIONS
;********************************************************************************************************

                                ; Mode, correspords to bits 0-5 in CPSR
MODE_BITS	DEFINE	0x1F		; Bit mask for mode bits in CPSR
USR_MODE	DEFINE	0x10		; User mode
FIQ_MODE	DEFINE	0x11		; Fast Interrupt Request mode
IRQ_MODE	DEFINE	0x12		; Interrupt Request mode
SVC_MODE	DEFINE	0x13		; Supervisor mode
ABT_MODE	DEFINE	0x17		; Abort mode
UND_MODE	DEFINE	0x1B		; Undefined Instruction mode
SYS_MODE	DEFINE	0x1F		; System mode
	
                                ; MMU C1
MMU_C1_M    DEFINE  0x0001
MMU_C1_A    DEFINE  0x0002
MMU_C1_C    DEFINE  0x0004
MMU_C1_W    DEFINE  0x0008
MMU_C1_S    DEFINE  0x0100
MMU_C1_R    DEFINE  0x0200
MMU_C1_I    DEFINE  0x1000
MMU_C1_RR   DEFINE  0x4000

;********************************************************************************************************
;                                            ARM EXCEPTION VECTORS
;********************************************************************************************************

    SECTION .intvec:CODE:NOROOT(2)
    PUBLIC  __vector
    PUBLIC  __iar_program_start

    IMPORT  OS_CPU_ARM_ExceptUndefInstrHndlr
    IMPORT  OS_CPU_ARM_ExceptSwiHndlr
    IMPORT  OS_CPU_ARM_ExceptPrefetchAbortHndlr
    IMPORT  OS_CPU_ARM_ExceptDataAbortHndlr
    IMPORT  OS_CPU_ARM_ExceptIrqHndlr
    IMPORT  OS_CPU_ARM_ExceptFiqHndlr

    ARM
__vector:
    LDR	    PC, [PC,#24]    ; Absolute jump can reach 4 GByte
    LDR     PC, [PC,#24]    ; Branch to undef_handler
    LDR     PC, [PC,#24]    ; Branch to swi_handler
    LDR     PC, [PC,#24]    ; Branch to prefetch_handler
    LDR     PC, [PC,#24]    ; Branch to data_handler
__vector_0x14:
    DC32    0               ; Reserved
    LDR	    PC, [PC,#24]    ; Branch to irq_handler
    LDR	    PC, [PC,#24]    ; Branch to fiq_handler


    DC32    __iar_program_start
    DC32    OS_CPU_ARM_ExceptUndefInstrHndlr
    DC32    OS_CPU_ARM_ExceptSwiHndlr
    DC32    OS_CPU_ARM_ExceptPrefetchAbortHndlr
    DC32    OS_CPU_ARM_ExceptDataAbortHndlr
    DC32    0
    DC32    OS_CPU_ARM_ExceptIrqHndlr
    DC32    OS_CPU_ARM_ExceptFiqHndlr

;********************************************************************************************************
;                                   LOW-LEVEL INITIALIZATION
;********************************************************************************************************
    SECTION FIQ_STACK:DATA:NOROOT(3)
    SECTION IRQ_STACK:DATA:NOROOT(3)
    SECTION SVC_STACK:DATA:NOROOT(3)
    SECTION ABT_STACK:DATA:NOROOT(3)
    SECTION UND_STACK:DATA:NOROOT(3)
    SECTION CSTACK:DATA:NOROOT(3)
    SECTION .text:CODE:NOROOT(2)
    REQUIRE __vector
    EXTERN  ?main
    EXTERN  LowLevelInit
    PUBLIC  __iar_program_start
    ARM
__iar_program_start:

;********************************************************************************************************
;                                    STACK POINTER INITIALIZATION
;********************************************************************************************************

    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#FIQ_MODE                     ; Set FIQ mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK
    
    MRS     r0,cpsr                             ; Original PSR value
    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#SVC_MODE                     ; Set SVC mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#ABT_MODE                     ; Set ABT mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(ABT_STACK)                  ; End of ABT_STACK
    
    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#UND_MODE                     ; Set UND mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(UND_STACK)                  ; End of UND_STACK
    
    BIC     r0,r0,#MODE_BITS                    ; Clear the mode bits
    ORR     r0,r0,#SYS_MODE                     ; Set System mode bits
    MSR     cpsr_c,r0                           ; Change the mode
    LDR     sp,=SFE(CSTACK)                     ; End of SYS_STACK

    LDR     r0, =LowLevelInit
    BLX     r0
     
    ;BIC r0, r0, #0xF0000000
    ;ADD r0, r0, r2
    ;ADD r0, r0, r3
    ;MOV r0, #0x21000000
    ;MOV r1, #0x00000009
    ;MOV r2, #0x00000008
    ;MOV r3, #0x00000007
    ;STMIB r0, {r1-r3}
    ;MOV r1, #1
    ;MOV r2, #2
    ;MOV r3, #3
    ;MOV r0, #0x21000000
    ;LDMIB r0!, {r1-r3}
;********************************************************************************************************
;                                   ADDITIONAL INITIALIZATION
;********************************************************************************************************

                                                ; ------------------- ENABLE THE I-CACHE ------------------ ;
    MRC     P15,  0, R0, C1, C0, 0              ; Read  MMU C1
    BIC     R0,  R0, #MMU_C1_M                  ; Disable MMU
    ORR     R0,  R0, #MMU_C1_I                  ; Enable I-Cache
    MCR     P15,  0, R0, C1, C0, 0              ; Write MMU C1

;********************************************************************************************************
;                           CONTINUE TO ?main FOR ADDITIONAL INITIALIZATION
;********************************************************************************************************

    LDR     r0,=?main
    BX      r0

    END
