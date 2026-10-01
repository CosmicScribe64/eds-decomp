	thumb_func_start sub_0802F90C
sub_0802F90C: @ 0x0802F90C
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802F954
	cmp r1, #0
	beq _0802F954
	ldr r2, _0802F944 @ =0x000007FF
	ldrh r1, [r1]
	and r2, r1
	lsl r0, r2, #2
	ldr r1, _0802F948 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802F954
	lsl r0, r2, #1
	ldr r1, _0802F94C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0802F950 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802F954
	mov r0, #1
	b _0802F956
	.align 2, 0
_0802F944: .4byte 0x000007FF
_0802F948: .4byte gUnk_08621DE0
_0802F94C: .4byte gUnk_08622AB4
_0802F950: .4byte 0x00000603
_0802F954:
	mov r0, #0
_0802F956:
	bx lr
	thumb_func_end sub_0802F90C

