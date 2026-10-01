	thumb_func_start sub_0801EA9C
sub_0801EA9C: @ 0x0801EA9C
	push {r4, lr}
	ldr r4, _0801EAB8 @ =0x02017A30
	ldrb r1, [r4, #0xB]
	cmp r1, #0
	beq _0801EABC
	cmp r1, #1
	beq _0801EAC4
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0801EAD0
	.align 2, 0
_0801EAB8: .4byte 0x02017A30
_0801EABC:
	mov r0, #0x80
	lsl r0, r0, #0x13
	strh r1, [r0]
	b _0801EAC8
_0801EAC4:
	bl sub_080609C4
_0801EAC8:
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
	mov r0, #0
_0801EAD0:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0801EA9C
	.align 2, 0

