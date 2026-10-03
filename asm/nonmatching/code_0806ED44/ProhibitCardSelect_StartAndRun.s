	thumb_func_start ProhibitCardSelect_StartAndRun
ProhibitCardSelect_StartAndRun: @ 0x0806F0BC
	push {lr}
	ldr r0, _0806F0D4 @ =0x03000040
	ldr r1, _0806F0D8 @ =0x0000485B
	add r2, r0, r1
	ldrb r1, [r2]
	cmp r1, #0
	beq _0806F0DC
	bl ProhibitCardSelect_Run
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0806F0EC
_0806F0D4: .4byte 0x03000040
_0806F0D8: .4byte 0x0000485B
_0806F0DC:
	ldr r0, _0806F0F0 @ =0x02017A40
	ldr r3, _0806F0F4 @ =0x000003E6
	add r0, r0, r3
	strb r1, [r0]
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	mov r0, #0
_0806F0EC:
	pop {r1}
	bx r1
_0806F0F0: .4byte 0x02017A40
_0806F0F4: .4byte 0x000003E6
	thumb_func_end ProhibitCardSelect_StartAndRun

