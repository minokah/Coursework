             AREA VerifyUPC, CODE, READONLY ; Declare region to run assembly
             ENTRY                          ; Entry point
			 
             MOV r4, #zero                  ; clear register for odd sum counter
             MOV r5, #zero                  ; clear register for even sum counter
             MOV r3, #loopAmount            ; clear register for odd-even checker/decrementer
             ADR r1, UPC                    ; point r1 to the location of the UPC string in memory
			 
                                            ; Parsing String, add integer to odd or even sums								
addSum       LDRB r2, [r1], #1              ; - load the digit (in ASCII) of the UPC string from memory (and shift position)
             SUB r2, #asciiDiff             ; - subtract 0x30 from that digit to get the integer value (explained in constants area)
             TST r3, #oddBit                ; - check if odd/even checker is 0
             ADDNE r4, r2                   ; - if checker is 0, move odd digit(first, third, ...) into r4 (odd sum)
             ADDEQ r5, r2                   ; - otherwise, move even digit (second, fourth, ...) into r5 (even sum)
             SUBS r3, #1                    ; - decrement odd-even checker by 1
             BPL addSum                     ; - if counter still positive (we know it hasn't ran 12 times yet) then branch back to addSum to keep adding
			 
                                            ; Finished Adding, add totals
finish       ADD r4, r4, LSL#1              ; - multiply the odd sum by 3 (odd + odd * 2) 
             ADD r5, r4                     ; - add the odd sum to the even sum
			 
                                            ; Modulo Loop
modulo       CMP r5, #0                     ; - check if even sum is larger than 0
             SUBGTS r5, #moduloAmount       ; - if so, subtract 10
             BGT modulo                     ; - if it's still larger than 0, branch back to modulo to keep subtracting
			 
                                            ; Final Result
result       MOVPL r0, #upcValid            ; - if it is, then it's a valid UPC (and r0 is 1)
             MOVMI r0, #upcInvalid          ; - if not, it's an invalid UPC (r0 is 2)
			 
loop         B loop                         ; Infinite loop

                                            ; UPC Strings
UPC          DCB "013800150738"             ; - correct UPC string
UPC2         DCB "060383755577"             ; - correct UPC string
UPC3         DCB "065633454712"             ; - correct UPC string

                                            ; Constants
zero         EQU 0                          ; - Litreally zero
asciiDiff    EQU 0x30                       ; - ASCII and real integer difference (ex. 0x35 - 0x30 = 0x5, which equals 5)
upcValid     EQU 1                          ; - Use 1 to signify valid UPC
upcInvalid   EQU 2                          ; - Use 2 to signify invalid UPC
loopAmount   EQU 11                         ; - Amount of times loop will run (11 - 0 for 12 digits)
moduloAmount EQU 10                         ; - Modulo amount (what to keep subtracing by)
oddBit       EQU 0x1                        ; - Bit to test for (if it's an odd number or not)

             END                            ; End program