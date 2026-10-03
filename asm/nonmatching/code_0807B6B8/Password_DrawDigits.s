	thumb_func_start Password_DrawDigits
Password_DrawDigits: @ 0x0807BF68
	push {r4, r5, lr}
	mov r4, #0
	mov r5, #0x18
_0807BF6E:
	mov r0, #0x80
	lsl r0, r0, #0xC
	orr r0, r5
	ldr r1, _0807BF98 @ =0x0201F7B0
	add r1, r4, r1
	ldrb r1, [r1]
	mov r3, #0x84
	lsl r3, r3, #5
	add r2, r1, r3
	mov r1, #0x80
	lsl r1, r1, #8
	bl AddSprite
	add r5, #8
	add r4, #1
	cmp r4, #7
	ble _0807BF6E
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807BF98: .4byte 0x0201F7B0
	thumb_func_end Password_DrawDigits

