	thumb_func_start sub_08059954
sub_08059954: @ 0x08059954
	push {r4, r5, r6, lr}
	sub sp, #0x14
	ldr r4, _0805996C @ =0x02015EF0
	ldrb r0, [r4, #6]
	cmp r0, #0x64
	beq _08059990
	cmp r0, #0x64
	bgt _08059970
	cmp r0, #0
	beq _08059976
	b _08059A24
	.align 2, 0
_0805996C: .4byte 0x02015EF0
_08059970:
	cmp r0, #0xC8
	beq _080599D4
	b _08059A24
_08059976:
	ldr r0, _0805998C @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _08059A24
	mov r0, #0x64
	strb r0, [r4, #6]
_08059988:
	mov r0, #0
	b _08059A26
_0805998C: .4byte 0x02015EE8
_08059990:
	ldr r0, _080599CC @ =0x000005A7
	bl sub_08059374
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08059988
	mov r2, sp
	ldrb r0, [r2, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r2, #2]
	mov r0, sp
	mov r1, #0
	mov r2, #0
	bl sub_0802EE18
	cmp r0, #0
	beq _08059A24
	bl sub_08057854
	cmp r0, #0
	ble _08059A24
	ldr r0, _080599D0 @ =0x0000047B
	bl sub_08059374
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059A24
	b _08059988
	.align 2, 0
_080599CC: .4byte 0x000005A7
_080599D0: .4byte 0x0000047B
_080599D4:
	ldrb r2, [r4, #0xB]
	mov r6, #0x94
	add r1, r2, #0
	mul r1, r6
	ldr r5, _08059A1C @ =0x0201A070
	add r1, r1, r5
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080599F4
	mov r0, #1
	add r1, r2, #0
	mov r2, #0
	bl sub_08018DC8
_080599F4:
	ldrb r2, [r4, #0xB]
	mov r1, #0x1F
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #0x10
	add r1, r2, #0
	mul r1, r6
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _08059A20 @ =0x80200000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl sub_0801FBCC
	mov r0, #0
	strb r0, [r4, #0xA]
	b _08059A26
_08059A1C: .4byte 0x0201A070
_08059A20: .4byte 0x80200000
_08059A24:
	mov r0, #1
_08059A26:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08059954
	.align 2, 0

