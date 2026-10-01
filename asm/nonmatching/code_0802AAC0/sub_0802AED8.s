	thumb_func_start sub_0802AED8
sub_0802AED8: @ 0x0802AED8
	push {r4, r5, lr}
	ldr r4, _0802AF1C @ =0x0201D810
	ldrb r1, [r4]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0802AF2C
	ldr r5, _0802AF20 @ =0x0819A7B8
	ldrb r2, [r4, #1]
	lsl r0, r2, #2
	add r0, r0, r5
	ldr r0, [r0]
	cmp r0, #0
	beq _0802AF24
	bl sub_0802AB0C
	ldrb r1, [r4, #1]
	lsl r0, r1, #2
	add r0, r0, r5
	ldr r0, [r0]
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802AF18
	mov r0, #0
	strb r0, [r4, #2]
	strb r0, [r4, #3]
	strb r0, [r4, #4]
	ldrb r0, [r4, #1]
	add r0, #1
	strb r0, [r4, #1]
_0802AF18:
	mov r0, #1
	b _0802AF2E
_0802AF1C: .4byte 0x0201D810
_0802AF20: .4byte gUnk_0819A7B8
_0802AF24:
	mov r0, #2
	neg r0, r0
	and r0, r1
	strb r0, [r4]
_0802AF2C:
	mov r0, #0
_0802AF2E:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802AED8

