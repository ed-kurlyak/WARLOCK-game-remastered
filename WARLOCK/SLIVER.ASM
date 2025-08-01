;/////////////////////////////////////////////////////////////////////////////
;
; This function is an assembly language version of the C function that renders
; a single textured sliver into the double buffer. It uses the pre-computed
; look up table for the scale indices to speed things up, moreover it uses
; global variables instead of variables passed on the stack.
;
;////////////////////////////////////////////////////////////////////////////

.386
.MODEL FLAT, C             ; use flat memory model and C function names

.CODE                       ; begin code segment

EXTRN screen_width:DWORD
EXTRN p_video_buffer:DWORD ; the external double buffer
EXTRN sliver_texture:DWORD ; a pointer to the texture memory
EXTRN sliver_column:WORD   ; the current texture column
EXTRN sliver_top:WORD      ; the starting Y of the sliver
EXTRN sliver_scale:WORD    ; the over all height of the sliver
EXTRN sliver_ray:WORD      ; the current video column
EXTRN sliver_clip:WORD     ; how much of the texture is being clipped
EXTRN scale_row:DWORD      ; the pointer to the proper row of pre-computed scale indices

PUBLIC Render_Sliver_32_ASM    ; export the function to the linker

Render_Sliver_32_ASM PROC      ; this is a C function

    push esi                ; save registers we will use
    push edi
    push ebx

    mov edi, p_video_buffer ; point edi to double buffer
    movzx edx, sliver_column ; hold the column in edx

    mov esi, sliver_texture ; esi points to texture memory

    ; offset = (sprite->y * 320) + sprite->x
    movzx eax, sliver_top   ; multiply Y by 320 to get proper offset
    imul eax, screen_width
    
    movzx ecx, sliver_ray
    add eax, ecx     ; add X
    add edi, eax
    

    movzx ebx, sliver_clip  ; move important constants into registers
    movzx eax, sliver_scale
    add eax, ebx

    mov ecx, scale_row      ; get scale table pointer

Sliver_Loop:                 ; main loop
    ; double_buffer[offset] = work_sprite[work_offset+column]
    push ebx                ; save ebx since we need it for addressing
    movzx ebx, dx           ; use column value
    mov bl, BYTE PTR [esi + ebx] ; get texture pixel
    mov BYTE PTR [edi], bl  ; move it to screen
    pop ebx                 ; restore ebx

    ; row = scale_table[scale]
    push eax                ; save eax
    mov eax, ebx            ; current index
    shl eax, 1              ; multiply by 2 for word array
    add eax, ecx            ; add scale table base
    mov dx, WORD PTR [eax]  ; get scale index
    add dx, sliver_column  ; add column offset
    pop eax                 ; restore eax

    ; offset += Screen_Width
    add edi, screen_width            ; move down to next video line

    inc ebx                 ; increment counter
    cmp ebx, eax
    jne Sliver_Loop         ; are we done?

    pop ebx                 ; restore registers
    pop edi
    pop esi

    ret                     ; return

Render_Sliver_32_ASM ENDP

END