	thumb_func_start sub_0806EE34
sub_0806EE34: @ 0x0806EE34
	push {r4, lr}
	ldr r2, _0806EE5C @ =0x03000040
	ldr r0, _0806EE60 @ =0x0000485A
	add r1, r2, r0
	mov r0, #0
	strb r0, [r1]
	ldr r1, _0806EE64 @ =0x0201DB20
	ldr r3, _0806EE68 @ =0x00001C48
	add r0, r1, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1C
	add r3, r1, #0
	cmp r0, #4
	bhi _0806EEFC
	lsl r0, r0, #2
	ldr r1, _0806EE6C @ =0x0806EE70
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0806EE5C: .4byte 0x03000040
_0806EE60: .4byte 0x0000485A
_0806EE64: .4byte 0x0201DB20
_0806EE68: .4byte 0x00001C48
_0806EE6C: .4byte 0x0806EE70
_0806EE70:
	.4byte _0806EEFC
	.4byte _0806EE84
	.4byte _0806EE90
	.4byte _0806EEE0
	.4byte _0806EEEC
_0806EE84:
	ldr r4, _0806EE8C @ =0x0000485B
	add r1, r2, r4
	mov r0, #5
	b _0806EEF2
_0806EE8C: .4byte 0x0000485B
_0806EE90:
	ldr r0, _0806EED4 @ =0x0000485B
	add r1, r2, r0
	mov r0, #0xD
	strb r0, [r1]
	ldr r1, _0806EED8 @ =0x02013D90
	mov r0, #1
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r4, _0806EEDC @ =0x00001C1C
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
	b _0806EEFE
	.align 2, 0
_0806EED4: .4byte 0x0000485B
_0806EED8: .4byte 0x02013D90
_0806EEDC: .4byte 0x00001C1C
_0806EEE0:
	ldr r0, _0806EEE8 @ =0x0000485B
	add r1, r2, r0
	mov r0, #9
	b _0806EEF2
_0806EEE8: .4byte 0x0000485B
_0806EEEC:
	ldr r3, _0806EEF8 @ =0x0000485B
	add r1, r2, r3
	mov r0, #1
_0806EEF2:
	strb r0, [r1]
	mov r0, #0
	b _0806EEFE
_0806EEF8: .4byte 0x0000485B
_0806EEFC:
	mov r0, #1
_0806EEFE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0806EE34

