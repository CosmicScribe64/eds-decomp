	thumb_func_start CardDetail_DrawTextBox
CardDetail_DrawTextBox: @ 0x080059B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	str r0, [sp, #8]
	ldr r0, [sp, #0x34]
	mov r9, r0
	ldr r5, [sp, #0x38]
	ldr r4, [sp, #0x44]
	ldr r0, [sp, #0x48]
	mov r8, r0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r0, r2, #0x10
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	mov r3, r9
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov ip, r3
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	mov r3, r8
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	lsl r3, r1, #8
	lsr r3, r3, #0x18
	str r3, [sp, #0x10]
	lsr r1, r1, #0x18
	mov sl, r1
	lsl r1, r0, #0x18
	lsr r6, r1, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r1, [sp, #0x3C]
	ldr r2, [sp, #0x40]
	mov r3, ip
	bl CardDetail_RenderText
	lsl r0, r7, #5
	ldr r1, _08005A68 @ =0x06004000
	add r0, r0, r1
	mov r1, r8
	bl TextCanvasToTiles
	mov r0, #0
	cmp r0, r9
	bge _08005A56
	ldr r5, _08005A6C @ =0x0300045C
_08005A24:
	mov r3, #0
	add r4, r0, #1
	cmp r3, r6
	bge _08005A50
	add r0, sl
	lsl r0, r0, #5
	ldr r2, [sp, #0x10]
	add r0, r0, r2
	ldr r2, [sp, #8]
	lsl r1, r2, #0xB
	lsl r0, r0, #1
	add r0, r0, r1
	add r2, r0, r5
_08005A3E:
	add r1, r7, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	strh r1, [r2]
	add r2, #2
	add r3, #1
	cmp r3, r6
	blt _08005A3E
_08005A50:
	add r0, r4, #0
	cmp r0, r9
	blt _08005A24
_08005A56:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005A68: .4byte 0x06004000
_08005A6C: .4byte 0x0300045C
	thumb_func_end CardDetail_DrawTextBox

