	thumb_func_start sub_08042B60
sub_08042B60: @ 0x08042B60
	push {r4, lr}
	ldr r0, _08042B8C @ =0x02017A40
	ldr r1, _08042B90 @ =0x00000491
	add r4, r0, r1
	mov r0, #0x40
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _08042B94
	bl sub_0804244C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08042B86
	mov r0, #0x41
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
_08042B86:
	mov r0, #1
	b _08042B96
	.align 2, 0
_08042B8C: .4byte 0x02017A40
_08042B90: .4byte 0x00000491
_08042B94:
	mov r0, #0
_08042B96:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08042B60

