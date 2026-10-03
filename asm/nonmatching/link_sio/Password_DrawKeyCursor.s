	thumb_func_start Password_DrawKeyCursor
Password_DrawKeyCursor: @ 0x0807BFE0
	push {r4, lr}
	mov r4, #0
_0807BFE4:
	ldr r1, _0807C018 @ =0x08087E24
	lsl r0, r4, #3
	add r3, r0, r1
	ldrh r1, [r3, #4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	add r2, r1, #0
	orr r2, r0
	ldr r0, _0807C01C @ =0x0201F7B0
	ldrb r0, [r0, #0x10]
	lsr r0, r0, #4
	cmp r4, r0
	bne _0807C036
	cmp r4, #9
	bgt _0807C020
	ldrb r1, [r3, #1]
	lsl r0, r1, #0x10
	ldrb r3, [r3]
	orr r0, r3
	mov r1, #0x40
	bl AddSprite
	b _0807C036
_0807C018: .4byte gPasswordKeypad
_0807C01C: .4byte 0x0201F7B0
_0807C020:
	ldr r0, _0807C044 @ =0x00880030
	ldr r1, _0807C048 @ =0x00004040
	ldr r2, _0807C04C @ =0x00001044
	bl AddSprite
	ldr r0, _0807C050 @ =0x00880050
	mov r1, #0x80
	lsl r1, r1, #7
	ldr r2, _0807C054 @ =0x00001048
	bl AddSprite
_0807C036:
	add r4, #1
	cmp r4, #0xA
	ble _0807BFE4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0807C044: .4byte 0x00880030
_0807C048: .4byte 0x00004040
_0807C04C: .4byte 0x00001044
_0807C050: .4byte 0x00880050
_0807C054: .4byte 0x00001048
	thumb_func_end Password_DrawKeyCursor

