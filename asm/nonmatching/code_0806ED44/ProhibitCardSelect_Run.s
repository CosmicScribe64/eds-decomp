	thumb_func_start ProhibitCardSelect_Run
ProhibitCardSelect_Run: @ 0x0806F05C
	push {r4, lr}
	ldr r0, _0806F0A0 @ =0x02017A40
	ldr r1, _0806F0A4 @ =0x000003E6
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _0806F07E
	ldr r0, _0806F0A8 @ =0x03000040
	ldr r2, _0806F0AC @ =0x00004874
	add r0, r0, r2
	mov r1, #4
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
_0806F07E:
	ldr r1, _0806F0B0 @ =0x081A7330
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806F0B4
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806F09C
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806F09C:
	mov r0, #0
	b _0806F0B6
_0806F0A0: .4byte 0x02017A40
_0806F0A4: .4byte 0x000003E6
_0806F0A8: .4byte 0x03000040
_0806F0AC: .4byte 0x00004874
_0806F0B0: .4byte gProhibitCardSelectSteps
_0806F0B4:
	mov r0, #1
_0806F0B6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end ProhibitCardSelect_Run

