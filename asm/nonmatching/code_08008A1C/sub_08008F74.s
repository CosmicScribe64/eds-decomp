	thumb_func_start sub_08008F74
sub_08008F74: @ 0x08008F74
	push {r4, r5, lr}
	mov r4, #0
	mov r1, #1
	and r1, r0
	ldr r0, _08008FBC @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
_08008F82:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _08008FC0 @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08008FCC
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08008FCC
	ldr r2, _08008FC4 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08008FC8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_0800756C
	cmp r0, #0
	beq _08008FCC
	mov r0, #1
	b _08008FD4
	.align 2, 0
_08008FBC: .4byte 0x00000D64
_08008FC0: .4byte 0x0201930C
_08008FC4: .4byte 0x000007FF
_08008FC8: .4byte gUnk_08622AB4
_08008FCC:
	add r4, #1
	cmp r4, #4
	ble _08008F82
	mov r0, #0
_08008FD4:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08008F74
	.align 2, 0

