	thumb_func_start CB_Record
CB_Record: @ 0x08003E94
	push {r4, lr}
	ldr r1, _08003EC0 @ =0x08198588
	ldr r0, _08003EC4 @ =0x03000040
	ldr r2, _08003EC8 @ =0x0000485B
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08003ECC
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003EBA
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08003EBA:
	mov r0, #0
	b _08003ECE
	.align 2, 0
_08003EC0: .4byte gRecordSteps
_08003EC4: .4byte 0x03000040
_08003EC8: .4byte 0x0000485B
_08003ECC:
	mov r0, #1
_08003ECE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end CB_Record

