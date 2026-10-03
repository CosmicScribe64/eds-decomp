	thumb_func_start IsCardProhibited
IsCardProhibited: @ 0x0800966C
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r5, #0
	b _080096D2
_08009676:
	lsl r4, r5, #1
	ldr r1, _080096C4 @ =0x00001AF0
	add r0, r6, r1
	add r0, r4, r0
	ldrh r0, [r0]
	add r1, r7, #0
	bl IsSameCardName
	cmp r0, #0
	beq _080096D0
	ldr r1, _080096C8 @ =0x00001AD0
	add r0, r6, r1
	add r0, r4, r0
	ldrh r1, [r0]
	lsr r2, r1, #8
	mov r1, #1
	ldrb r0, [r0]
	and r1, r0
	mov r0, #0x94
	mul r2, r0
	ldr r0, _080096CC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	add r0, r6, #0
	add r0, #0x2C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080096D0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080096D0
	mov r0, #1
	b _080096E4
	.align 2, 0
_080096C4: .4byte 0x00001AF0
_080096C8: .4byte 0x00001AD0
_080096CC: .4byte 0x00000D64
_080096D0:
	add r5, #1
_080096D2:
	ldr r6, _080096EC @ =0x020192E0
	ldr r1, _080096F0 @ =0x00001ACC
	add r0, r6, r1
	ldr r0, [r0]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	cmp r5, r0
	blt _08009676
	mov r0, #0
_080096E4:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080096EC: .4byte 0x020192E0
_080096F0: .4byte 0x00001ACC
	thumb_func_end IsCardProhibited

