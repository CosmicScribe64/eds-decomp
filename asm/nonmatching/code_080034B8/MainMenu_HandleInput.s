	thumb_func_start MainMenu_HandleInput
MainMenu_HandleInput: @ 0x080039E0
	push {r4, r5, lr}
	bl MainMenu_DrawItems
	ldr r5, _08003A3C @ =0x03000040
	mov r0, #0x40
	ldrh r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	beq _08003A0A
	ldr r4, _08003A40 @ =0x02015ED8
	ldrh r0, [r4]
	add r0, #6
	strh r0, [r4]
	ldrh r0, [r4]
	mov r1, #7
	bl __umodsi3
	strh r0, [r4]
	mov r0, #0
	bl PlaySE
_08003A0A:
	mov r0, #0x80
	ldrh r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	beq _08003A2C
	ldr r4, _08003A40 @ =0x02015ED8
	ldrh r0, [r4]
	add r0, #8
	strh r0, [r4]
	ldrh r0, [r4]
	mov r1, #7
	bl __umodsi3
	strh r0, [r4]
	mov r0, #0
	bl PlaySE
_08003A2C:
	mov r0, #1
	ldrh r5, [r5, #6]
	and r0, r5
	cmp r0, #0
	bne _08003A44
	mov r0, #0
	b _08003A50
	.align 2, 0
_08003A3C: .4byte 0x03000040
_08003A40: .4byte 0x02015ED8
_08003A44:
	mov r0, #1
	bl PlaySE
	bl FadeOutBGM
	mov r0, #1
_08003A50:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end MainMenu_HandleInput
	.align 2, 0

