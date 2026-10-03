	thumb_func_start OpponentSelect_SnapCursor
OpponentSelect_SnapCursor: @ 0x0800323C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r5, _08003288 @ =0x0201F7E0
	ldr r1, _0800328C @ =0x081983AC
	lsl r0, r4, #2
	add r2, r0, r1
	add r1, r5, #4
	mov r3, #7
_0800324C:
	ldrh r0, [r2]
	add r0, #0x10
	strh r0, [r1]
	ldrh r0, [r2, #2]
	strh r0, [r1, #0x10]
	add r1, #2
	sub r3, #1
	cmp r3, #0
	bge _0800324C
	mov r0, #7
	and r4, r0
	lsl r1, r4, #6
	ldr r0, _08003290 @ =0xFFFFFE3F
	ldrh r2, [r5]
	and r0, r2
	orr r0, r1
	strh r0, [r5]
	mov r0, #0x1F
	neg r0, r0
	ldrb r1, [r5, #1]
	and r0, r1
	strb r0, [r5, #1]
	ldr r0, [r5]
	ldr r1, _08003294 @ =0xFFFE1FFF
	and r0, r1
	str r0, [r5]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08003288: .4byte 0x0201F7E0
_0800328C: .4byte gOpponentSelectSlotPos
_08003290: .4byte 0xFFFFFE3F
_08003294: .4byte 0xFFFE1FFF
	thumb_func_end OpponentSelect_SnapCursor

