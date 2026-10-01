	thumb_func_start sub_0804D298
sub_0804D298: @ 0x0804D298
	push {r4, r5, r6, lr}
	add r6, r0, #0
	ldr r5, _0804D310 @ =0x02018450
	mov r0, #4
	ldrb r1, [r5]
	and r0, r1
	cmp r0, #0
	beq _0804D308
	mov r0, #1
	sub r4, r0, r6
	ldrh r1, [r5, #2]
	add r0, r4, #0
	mov r2, #0
	bl sub_0802CFA0
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _0804D314 @ =0x000007FF
	ldrh r1, [r5, #2]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804D318 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0804D31C @ =0x000002FA
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804D2E0
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r5
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0804D2E0
	mov r2, #0
_0804D2E0:
	cmp r2, #0
	beq _0804D308
	mov r1, #1
	sub r0, r1, r6
	and r0, r1
	lsl r0, r0, #0x1F
	ldr r3, _0804D310 @ =0x02018450
	ldrb r2, [r3, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0x91
	lsl r2, r2, #0x16
	orr r1, r2
	orr r0, r1
	ldrh r3, [r3, #2]
	orr r0, r3
	mov r1, #0
	bl sub_0801FBCC
_0804D308:
	mov r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0804D310: .4byte 0x02018450
_0804D314: .4byte 0x000007FF
_0804D318: .4byte gUnk_08622AB4
_0804D31C: .4byte 0x000002FA
	thumb_func_end sub_0804D298

