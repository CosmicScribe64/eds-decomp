	thumb_func_start sub_0802AAC0
sub_0802AAC0: @ 0x0802AAC0
	push {r4, lr}
	ldr r4, _0802AAD8 @ =0x0201D810
	ldrb r0, [r4, #2]
	cmp r0, #0
	beq _0802AADC
	cmp r0, #1
	beq _0802AAF4
	bl sub_08060B2C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802AB04
_0802AAD8: .4byte 0x0201D810
_0802AADC:
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802AB02
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	b _0802AAFC
_0802AAF4:
	bl sub_080609C4
	bl sub_0805F96C
_0802AAFC:
	ldrb r0, [r4, #2]
	add r0, #1
	strb r0, [r4, #2]
_0802AB02:
	mov r0, #0
_0802AB04:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802AAC0
	.align 2, 0

