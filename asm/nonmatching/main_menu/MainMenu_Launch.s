	thumb_func_start MainMenu_Launch
MainMenu_Launch: @ 0x08003A58
	push {lr}
	bl MainMenu_DrawItems
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003A88
	ldr r1, _08003A90 @ =0x03000040
	ldr r0, _08003A94 @ =0x0000488A
	add r1, r1, r0
	ldr r0, _08003A98 @ =0xFFFFF00F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r1, _08003A9C @ =0x081984D8
	ldr r0, _08003AA0 @ =0x02015ED8
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r0, [r0]
	bl SetMainCallback
_08003A88:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08003A90: .4byte 0x03000040
_08003A94: .4byte 0x0000488A
_08003A98: .4byte 0xFFFFF00F
_08003A9C: .4byte gMainMenuTable
_08003AA0: .4byte 0x02015ED8
	thumb_func_end MainMenu_Launch

