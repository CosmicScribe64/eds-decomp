	thumb_func_start sub_0806ED64
sub_0806ED64: @ 0x0806ED64
	push {r4, lr}
	ldr r2, _0806ED8C @ =0x03000040
	ldr r0, _0806ED90 @ =0x0000485A
	add r1, r2, r0
	mov r0, #0
	strb r0, [r1]
	ldr r1, _0806ED94 @ =0x0201DB20
	ldr r3, _0806ED98 @ =0x00001C48
	add r0, r1, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1C
	add r3, r1, #0
	cmp r0, #4
	bhi _0806EE2C
	lsl r0, r0, #2
	ldr r1, _0806ED9C @ =0x0806EDA0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0806ED8C: .4byte 0x03000040
_0806ED90: .4byte 0x0000485A
_0806ED94: .4byte 0x0201DB20
_0806ED98: .4byte 0x00001C48
_0806ED9C: .4byte 0x0806EDA0
_0806EDA0:
	.4byte _0806EE2C
	.4byte _0806EDB4
	.4byte _0806EDC0
	.4byte _0806EE10
	.4byte _0806EE1C
_0806EDB4:
	ldr r4, _0806EDBC @ =0x00004859
	add r1, r2, r4
	mov r0, #5
	b _0806EE22
_0806EDBC: .4byte 0x00004859
_0806EDC0:
	ldr r0, _0806EE04 @ =0x00004859
	add r1, r2, r0
	mov r0, #0xD
	strb r0, [r1]
	ldr r1, _0806EE08 @ =0x02013D90
	mov r0, #1
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r4, _0806EE0C @ =0x00001C1C
	add r0, r3, r4
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0
	mov r2, #0
	bl sub_0800688C
	mov r0, #0
	b _0806EE2E
	.align 2, 0
_0806EE04: .4byte 0x00004859
_0806EE08: .4byte 0x02013D90
_0806EE0C: .4byte 0x00001C1C
_0806EE10:
	ldr r0, _0806EE18 @ =0x00004859
	add r1, r2, r0
	mov r0, #9
	b _0806EE22
_0806EE18: .4byte 0x00004859
_0806EE1C:
	ldr r3, _0806EE28 @ =0x00004859
	add r1, r2, r3
	mov r0, #1
_0806EE22:
	strb r0, [r1]
	mov r0, #0
	b _0806EE2E
_0806EE28: .4byte 0x00004859
_0806EE2C:
	mov r0, #1
_0806EE2E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0806ED64

