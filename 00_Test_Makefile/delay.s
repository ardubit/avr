; Delay routine 
; Input:    Number of iterations: 16-bit in R24 and R25
;           Each iteration takes 4us (0.004ms)
;           Return takes 4us
;           100ms = (25000)*0.004 ms

; global to make the function visible to the linker
.global delay 

delay: 
    subi    R24, 0x01   ; Subtract 1
    sbci    R25, 0x00   ; Subtract 0 with carry
    brne    .-6         ; Repeat until the result is zero
ret                     ; Return
