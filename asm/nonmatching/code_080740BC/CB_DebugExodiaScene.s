	thumb_func_start CB_DebugExodiaScene
CB_DebugExodiaScene: @ 0x080743A4
	push {r4, r5, lr}
	ldr r1, _080743BC @ =0x03000040
	ldr r0, _080743C0 @ =0x00004859
	add r4, r1, r0
	ldrb r2, [r4]
	cmp r2, #0
	beq _080743C4
	cmp r2, #1
	beq _080743D8
	mov r0, #1
	b _08074426
	.align 2, 0
_080743BC: .4byte 0x03000040
_080743C0: .4byte 0x00004859
_080743C4:
	ldr r3, _080743D0 @ =0x0000485A
	add r0, r1, r3
	strb r2, [r0]
	ldr r0, _080743D4 @ =0x02017A30
	strb r2, [r0, #0xB]
	b _0807441E
_080743D0: .4byte 0x0000485A
_080743D4: .4byte 0x02017A30
_080743D8:
	ldr r5, _080743F8 @ =0x0000485A
	add r3, r1, r5
	ldrb r0, [r3]
	add r0, #1
	strb r0, [r3]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #7
	bhi _08074404
	ldr r0, _080743FC @ =0x0000040E
	add r1, r1, r0
	ldr r0, _08074400 @ =0x0000FFFE
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	b _08074424
_080743F8: .4byte 0x0000485A
_080743FC: .4byte 0x0000040E
_08074400: .4byte 0x0000FFFE
_08074404:
	ldr r5, _0807442C @ =0x0000040E
	add r1, r1, r5
	mov r0, #1
	mov r2, #0
	ldrh r5, [r1]
	orr r0, r5
	strh r0, [r1]
	strb r2, [r3]
	bl ExodiaScene_Run
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08074424
_0807441E:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08074424:
	mov r0, #0
_08074426:
	pop {r4, r5}
	pop {r1}
	bx r1
_0807442C: .4byte 0x0000040E
	thumb_func_end CB_DebugExodiaScene

