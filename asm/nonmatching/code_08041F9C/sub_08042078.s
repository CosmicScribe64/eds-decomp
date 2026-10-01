	thumb_func_start sub_08042078
sub_08042078: @ 0x08042078
	push {r4, lr}
	mov r4, #1
	cmp r2, #5
	beq _0804208C
	cmp r2, #0xB
	bne _0804209A
	add r2, r3, #0
	bl sub_08041F9C
	b _08042092
_0804208C:
	add r2, r3, #5
	bl sub_08041DC4
_08042092:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804209A
	mov r4, #0x41
_0804209A:
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08042078
	.align 2, 0

