	thumb_func_start sub_08027C58
sub_08027C58: @ 0x08027C58
	push {r4, lr}
	ldr r1, _08027C80 @ =0x08199DFC
	ldr r4, _08027C84 @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08027C88
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08027C7A
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_08027C7A:
	mov r0, #0
	b _08027C8A
	.align 2, 0
_08027C80: .4byte gUnk_08199DFC
_08027C84: .4byte 0x02017A30
_08027C88:
	mov r0, #1
_08027C8A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08027C58

