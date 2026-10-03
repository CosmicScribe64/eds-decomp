	thumb_func_start Password_ShowUsed
Password_ShowUsed: @ 0x0807CB74
	push {r4, lr}
	ldr r1, _0807CC0C @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	ldr r3, _0807CC10 @ =0x0201F7B0
	cmp r0, #0
	beq _0807CB94
	mov r0, #0xF
	ldrh r1, [r3, #0x12]
	and r0, r1
	mov r2, #0x96
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	strh r0, [r3, #0x12]
_0807CB94:
	ldrh r0, [r3, #0x12]
	lsl r2, r0, #0x10
	lsr r1, r2, #0x14
	ldr r0, _0807CC14 @ =0x0000012B
	cmp r1, r0
	bhi _0807CC20
	add r0, r1, #0
	cmp r0, #0xB3
	bhi _0807CBCE
	ldrb r2, [r3, #0x11]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1A
	add r1, #1
	mov r0, #0x3F
	and r1, r0
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #0x11]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	mov r1, #0x1F
	and r1, r0
	cmp r1, #0x1F
	bne _0807CBCE
	mov r0, #0x28
	bl PlaySE
_0807CBCE:
	ldr r4, _0807CC10 @ =0x0201F7B0
	ldrb r2, [r4, #0x11]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1A
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _0807CBEA
	ldr r0, _0807CC18 @ =0x00080028
	mov r1, #0x81
	lsl r1, r1, #7
	ldr r2, _0807CC1C @ =0x0000110B
	bl AddSprite
_0807CBEA:
	ldr r1, _0807CC0C @ =0x03000040
	mov r0, #8
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0807CC20
	ldrh r2, [r4, #0x12]
	lsr r1, r2, #4
	add r1, #1
	lsl r1, r1, #4
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strh r0, [r4, #0x12]
	mov r0, #0
	b _0807CC22
	.align 2, 0
_0807CC0C: .4byte 0x03000040
_0807CC10: .4byte 0x0201F7B0
_0807CC14: .4byte 0x0000012B
_0807CC18: .4byte 0x00080028
_0807CC1C: .4byte 0x0000110B
_0807CC20:
	mov r0, #1
_0807CC22:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end Password_ShowUsed

