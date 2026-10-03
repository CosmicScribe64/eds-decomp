	thumb_func_start CB_DeckEdit
CB_DeckEdit: @ 0x0806EF04
	push {r4, lr}
	ldr r1, _0806EF54 @ =0x03000040
	ldr r0, _0806EF58 @ =0x00004859
	add r4, r1, r0
	ldrb r0, [r4]
	cmp r0, #0
	bne _0806EF20
	ldr r2, _0806EF5C @ =0x00004874
	add r1, r1, r2
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0806EF20:
	ldr r1, _0806EF60 @ =0x0201DB20
	ldr r0, _0806EF64 @ =0x00001C5A
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _0806EF68 @ =0x081A725C
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806EF6C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806EF4E
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806EF4E:
	mov r0, #0
	b _0806EF6E
	.align 2, 0
_0806EF54: .4byte 0x03000040
_0806EF58: .4byte 0x00004859
_0806EF5C: .4byte 0x00004874
_0806EF60: .4byte 0x0201DB20
_0806EF64: .4byte 0x00001C5A
_0806EF68: .4byte gDeckEditSteps
_0806EF6C:
	mov r0, #1
_0806EF6E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end CB_DeckEdit

