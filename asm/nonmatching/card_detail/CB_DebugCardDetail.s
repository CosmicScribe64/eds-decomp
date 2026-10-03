	thumb_func_start CB_DebugCardDetail
CB_DebugCardDetail: @ 0x0800736C
	push {r4, r5, lr}
	ldr r1, _080073A4 @ =0x08198D50
	ldr r5, _080073A8 @ =0x03000040
	ldr r0, _080073AC @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080073B4
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080073A0
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _080073B0 @ =0x0000485A
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_080073A0:
	mov r0, #0
	b _080073B6
_080073A4: .4byte gDebugCardDetailSteps
_080073A8: .4byte 0x03000040
_080073AC: .4byte 0x00004859
_080073B0: .4byte 0x0000485A
_080073B4:
	mov r0, #1
_080073B6:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CB_DebugCardDetail

