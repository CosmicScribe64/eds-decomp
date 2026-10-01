	thumb_func_start sub_08008FDC
sub_08008FDC: @ 0x08008FDC
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _08009020 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_08008FEC:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08009024 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08009028
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08009028
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800CAF0
	cmp r0, #1
	blt _08009028
	cmp r0, #2
	ble _0800901C
	cmp r0, #6
	bne _08009028
_0800901C:
	mov r0, #0
	b _08009030
_08009020: .4byte 0x00000D64
_08009024: .4byte 0x0201930C
_08009028:
	add r4, #1
	cmp r4, #4
	ble _08008FEC
	mov r0, #1
_08009030:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08008FDC
	.align 2, 0

