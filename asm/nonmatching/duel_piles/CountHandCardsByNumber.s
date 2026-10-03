	thumb_func_start CountHandCardsByNumber
CountHandCardsByNumber: @ 0x0800A2A8
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _0800A2F0 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _0800A2F4 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #2]
	cmp r3, r2
	bge _0800A2E8
	ldr r6, _0800A2F8 @ =0x00000684
	add r0, r4, r6
	add r1, r1, r0
	ldr r6, _0800A2FC @ =0x000007FF
	ldr r4, _0800A300 @ =0x08622AB4
_0800A2CC:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r5
	bne _0800A2E0
	add r3, #1
_0800A2E0:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _0800A2CC
_0800A2E8:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0800A2F0: .4byte 0x020192E4
_0800A2F4: .4byte 0x00000D64
_0800A2F8: .4byte 0x00000684
_0800A2FC: .4byte 0x000007FF
_0800A300: .4byte gCardIdToNumber
	thumb_func_end CountHandCardsByNumber

