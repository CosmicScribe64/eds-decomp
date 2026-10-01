	thumb_func_start sub_08008CFC
sub_08008CFC: @ 0x08008CFC
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	mov r0, #1
	and r0, r4
	ldr r1, _08008D20 @ =0x00000D64
	mul r1, r0
	ldr r0, _08008D24 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r5
	add r0, r1, r0
	cmp r2, #0
	beq _08008D28
	bl sub_08009768
	b _08008D2C
_08008D20: .4byte 0x00000D64
_08008D24: .4byte 0x0201930C
_08008D28:
	bl sub_080096F4
_08008D2C:
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08008278
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08008CFC
	.align 2, 0

