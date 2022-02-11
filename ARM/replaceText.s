          AREA QUESTION, CODE, READONLY
          ENTRY
                                              ; Initialize and point registers
          ADR r0, STRING1                     ; - Point to original string
          ADR r1, STRING2                     ; - Point to where to store the final string
          MOV r2, #-1                         ; - Initialize pointer incrementer
		                                      
                                              ; Read and increment characters
Read      ADD r2, #1                          ; - Increase pointer incremeter by 1
          MOV r3, r2                          ; - Clone pointer r2 into r3
          LDRB r4, [r0, r2]                   ; - Load character into r4
          MOV r5, r4                          ; - Clone character r4 into r5
		  
                                              ; Load character and check for any form of "the"
Check     CMP r4, #t                          ; - Check for 't'
          SUBEQ r3, #1                        ; - Decrement pointer for space (0x20)
          LDRBEQ r4, [r0, r3]                 ; - Load that character
          CMPEQ r4, #spc                      ; - Check for the space
          ADDEQ r3, #2                        ; - Increment pointer twice for 'h' (we are going backwards to check space, then forwards for the rest)
          LDRBEQ r4, [r0, r3]                 ; - Load next character
          CMPEQ r4, #h                        ; - Check 'h'
          ADDEQ r3, #1                        ; - Increment pointer for 'e'
          LDRBEQ r4, [r0, r3]                 ; - Load next character
          CMPEQ r4, #e                        ; - Check 'e'
          ADDEQ r3, #1                        ; - Increment pointer for space
          LDRBEQ r4, [r0, r3]                 ; - Load next character
          CMPEQ r4, #spc                      ; - Check space
          ADDEQ r2, #2                        ; - Skip characters if "the" matches
          BEQ Read                            ; - Go back to Read to read next characters
		  
          CMPNE r4, #null                     ; Compare if null character  
          STRBNE r5, [r1], #1                 ; Store cloned character (r5) if not any of "the" cases and not null character
          BNE Read                            ; Go back to Read if not the case

Loop      B Loop                              ; Finishing loop

                                              ; Constants
t         EQU 0x74                            ; - Character 't'
h         EQU 0x68                            ; - Character 'h'
e         EQU 0x65                            ; - Character 'e'
spc       EQU 0x20                            ; - Character space
null      EQU 0x0                             ; - Character null
          
FIRST     DCB 0x20                            ; Define a space
STRING1   DCB "and the man said they must go" ; String1
EoS       DCB 0x00                            ; end of string1
STRING2   SPACE 0x7F                          ; just allocating 127 bytes

          END
