	thumb_func_start sub_08034BA8
sub_08034BA8: @ 0x08034BA8
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08034BEC
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _08034BF4 @ =0x000007FF
	add r1, r4, #0
	ldrh r2, [r6]
	and r1, r2
	lsl r1, r1, #1
	ldr r5, _08034BF8 @ =0x08622AB4
	add r1, r1, r5
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldrh r6, [r6]
	and r4, r6
	lsl r4, r4, #1
	add r4, r4, r5
	ldrh r2, [r4]
	mov r3, #0
	bl sub_0802AF34
_08034BEC:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08034BF4: .4byte 0x000007FF
_08034BF8: .4byte gUnk_08622AB4
	thumb_func_end sub_08034BA8

