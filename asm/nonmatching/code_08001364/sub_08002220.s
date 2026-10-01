	thumb_func_start sub_08002220
sub_08002220: @ 0x08002220
	push {r4, r5, lr}
	sub sp, #4
	ldr r4, _08002268 @ =0x0201F7D0
	ldrb r1, [r4, #8]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x1D
	lsl r0, r0, #5
	add r0, #0xD
	ldrh r1, [r4, #8]
	lsl r2, r1, #0x16
	lsr r2, r2, #0x1D
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #0x13
	mov r2, #0x94
	lsl r2, r2, #0xE
	add r1, r1, r2
	orr r0, r1
	mov r2, #0xBC
	lsl r2, r2, #1
	mov r1, #0x80
	bl sub_080762D0
	mov r0, #4
	ldrb r4, [r4, #8]
	and r0, r4
	cmp r0, #0
	beq _08002270
	ldr r0, _0800226C @ =0x00080068
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0xC8
	lsl r2, r2, #1
	bl sub_080762D0
	b _0800227E
_08002268: .4byte 0x0201F7D0
_0800226C: .4byte 0x00080068
_08002270:
	ldr r0, _080022D4 @ =0x00700068
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0xCA
	lsl r2, r2, #1
	bl sub_080762D0
_0800227E:
	mov r4, #0
	mov r5, #0x10
_08002282:
	mov r0, #0xC8
	lsl r0, r0, #0xD
	orr r0, r5
	lsl r2, r4, #0x12
	ldr r1, _080022D8 @ =0x82A00000
	add r2, r2, r1
	lsr r2, r2, #0x10
	mov r1, #0x81
	lsl r1, r1, #7
	bl sub_080761F0
	add r5, #0x20
	add r4, #1
	cmp r4, #6
	ble _08002282
	ldr r4, _080022DC @ =0x0201F7D0
	ldrh r1, [r4, #2]
	mov r0, sp
	bl sub_080047F4
	ldr r0, [sp, #0]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #3
	bl __modsi3
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r5, r1, #1
	ldrb r2, [r4, #8]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _080022E6
	cmp r0, #1
	bgt _080022E0
	cmp r0, #0
	beq _0800230E
	b _08002334
	.align 2, 0
_080022D4: .4byte 0x00700068
_080022D8: .4byte 0x82A00000
_080022DC: .4byte 0x0201F7D0
_080022E0:
	cmp r0, #2
	beq _0800230E
	b _08002334
_080022E6:
	ldrh r1, [r4, #2]
	mov r0, sp
	bl sub_080047F4
	ldr r0, [sp, #0]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #3
	bl __divsi3
	add r1, r0, #0
	ldr r0, _08002328 @ =0x06014C00
	lsl r1, r1, #0xB
	ldr r2, _0800232C @ =0x087F5DF8
	add r1, r1, r2
	mov r2, #0x80
	lsl r2, r2, #4
	bl sub_08075294
_0800230E:
	ldr r3, _08002330 @ =0x0201F7D0
	ldrb r2, [r3, #8]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	add r1, #1
	mov r0, #3
	and r1, r0
	mov r0, #4
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #8]
	b _08002368
_08002328: .4byte 0x06014C00
_0800232C: .4byte gUnk_087F5DF8
_08002330: .4byte 0x0201F7D0
_08002334:
	ldr r0, _08002370 @ =0x00080010
	mov r4, #0x81
	lsl r4, r4, #7
	ldr r1, _08002374 @ =0x00007260
	add r2, r5, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r1, r4, #0
	bl sub_080761F0
	ldr r0, _08002378 @ =0x00080030
	ldr r1, _0800237C @ =0x00007264
	add r2, r5, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r1, r4, #0
	bl sub_080761F0
	ldr r0, _08002380 @ =0x00080050
	ldr r1, _08002384 @ =0x00007268
	add r2, r5, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x40
	bl sub_080761F0
_08002368:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
_08002370: .4byte 0x00080010
_08002374: .4byte 0x00007260
_08002378: .4byte 0x00080030
_0800237C: .4byte 0x00007264
_08002380: .4byte 0x00080050
_08002384: .4byte 0x00007268
	thumb_func_end sub_08002220

