	thumb_func_start ProhibitCardSelect_RunListFilter
ProhibitCardSelect_RunListFilter: @ 0x0806A96C
	push {r4, lr}
	ldr r1, _0806A998 @ =0x081A723C
	ldr r0, _0806A99C @ =0x02017A40
	ldr r2, _0806A9A0 @ =0x000003E7
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806A9A4
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806A992
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806A992:
	mov r0, #0
	b _0806A9A6
	.align 2, 0
_0806A998: .4byte gListFilterSteps
_0806A99C: .4byte 0x02017A40
_0806A9A0: .4byte 0x000003E7
_0806A9A4:
	mov r0, #1
_0806A9A6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end ProhibitCardSelect_RunListFilter

