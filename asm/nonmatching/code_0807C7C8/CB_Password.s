	thumb_func_start CB_Password
CB_Password: @ 0x0807CC28
	push {r4, r5, lr}
	ldr r1, _0807CC60 @ =0x081A7970
	ldr r5, _0807CC64 @ =0x03000040
	ldr r0, _0807CC68 @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0807CC70
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807CC5C
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _0807CC6C @ =0x0000485A
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_0807CC5C:
	mov r0, #0
	b _0807CC72
_0807CC60: .4byte gPasswordSteps
_0807CC64: .4byte 0x03000040
_0807CC68: .4byte 0x00004859
_0807CC6C: .4byte 0x0000485A
_0807CC70:
	mov r0, #1
_0807CC72:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CB_Password

