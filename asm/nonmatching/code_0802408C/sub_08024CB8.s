	thumb_func_start sub_08024CB8
sub_08024CB8: @ 0x08024CB8
	push {r4, lr}
	ldr r1, _08024CE0 @ =0x08198FAC
	ldr r4, _08024CE4 @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08024CE8
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08024CDA
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_08024CDA:
	mov r0, #0
	b _08024CEA
	.align 2, 0
_08024CE0: .4byte gUnk_08198FAC
_08024CE4: .4byte 0x02017A30
_08024CE8:
	mov r0, #1
_08024CEA:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08024CB8

