	thumb_func_start sub_08008C94
sub_08008C94: @ 0x08008C94
	add r3, r0, #0
	lsl r1, r1, #0x15
	lsr r1, r1, #0x13
	ldr r0, _08008CBC @ =0x08621DE0
	add r1, r1, r0
	ldr r1, [r1]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08008CC0
	cmp r0, #0x15
	blt _08008CC0
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08008CC2
	.align 2, 0
_08008CBC: .4byte gUnk_08621DE0
_08008CC0:
	mov r0, #0
_08008CC2:
	cmp r0, #2
	bne _08008CCA
_08008CC6:
	mov r0, #1
	b _08008CF0
_08008CCA:
	mov r2, #5
	mov r0, #1
	and r0, r3
	ldr r1, _08008CF4 @ =0x00000D64
	mul r0, r1
	mov r3, #0xB9
	lsl r3, r3, #2
	add r1, r0, r3
	ldr r3, _08008CF8 @ =0x0201930C
_08008CDC:
	add r0, r1, r3
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08008CC6
	add r1, #0x94
	add r2, #1
	cmp r2, #9
	ble _08008CDC
	mov r0, #0
_08008CF0:
	bx lr
	.align 2, 0
_08008CF4: .4byte 0x00000D64
_08008CF8: .4byte 0x0201930C
	thumb_func_end sub_08008C94

