	thumb_func_start sub_08067908
sub_08067908: @ 0x08067908
	add r3, r0, #0
	ldrb r2, [r3, #1]
	mov r0, #7
	and r0, r2
	cmp r0, #0
	beq _0806799C
	ldrb r0, [r3]
	sub r0, #1
	strb r0, [r3]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _0806799C
	mov r0, #0
	strb r0, [r3]
	lsl r0, r2, #0x1D
	lsr r0, r0, #0x1D
	cmp r0, #1
	beq _08067934
	cmp r0, #2
	beq _08067964
	b _0806799C
_08067934:
	mov r0, #0x60
	and r0, r2
	cmp r0, #0x40
	beq _08067944
	lsl r0, r2, #0x19
	lsr r0, r0, #0x1E
	add r0, #2
	b _08067972
_08067944:
	mov r0, #8
	neg r0, r0
	and r0, r2
	strb r0, [r3, #1]
	ldr r1, _0806795C @ =0x0201DB20
	ldr r0, _08067960 @ =0x00001C48
	add r1, r1, r0
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	b _0806799A
	.align 2, 0
_0806795C: .4byte 0x0201DB20
_08067960: .4byte 0x00001C48
_08067964:
	mov r0, #0x60
	and r0, r2
	cmp r0, #0
	beq _08067984
	lsl r0, r2, #0x19
	lsr r0, r0, #0x1E
	sub r0, #2
_08067972:
	mov r1, #3
	and r0, r1
	lsl r0, r0, #5
	mov r1, #0x61
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3, #1]
	b _0806799C
_08067984:
	mov r0, #8
	neg r0, r0
	and r0, r2
	strb r0, [r3, #1]
	ldr r1, _080679A0 @ =0x0201DB20
	ldr r0, _080679A4 @ =0x00001C48
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0806799A:
	strb r0, [r1]
_0806799C:
	bx lr
	.align 2, 0
_080679A0: .4byte 0x0201DB20
_080679A4: .4byte 0x00001C48
	thumb_func_end sub_08067908

