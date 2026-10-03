	thumb_func_start DeckEdit_RunListFilter
DeckEdit_RunListFilter: @ 0x0806A92C
	push {r4, lr}
	ldr r1, _0806A958 @ =0x081A723C
	ldr r0, _0806A95C @ =0x03000040
	ldr r2, _0806A960 @ =0x0000485A
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806A964
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806A952
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806A952:
	mov r0, #0
	b _0806A966
	.align 2, 0
_0806A958: .4byte gListFilterSteps
_0806A95C: .4byte 0x03000040
_0806A960: .4byte 0x0000485A
_0806A964:
	mov r0, #1
_0806A966:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DeckEdit_RunListFilter

