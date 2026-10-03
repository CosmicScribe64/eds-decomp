	thumb_func_start FindHandCardByNumber
FindHandCardByNumber: @ 0x0800A304
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _0800A340 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _0800A344 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #2]
	cmp r3, r2
	bge _0800A35C
	ldr r6, _0800A348 @ =0x00000684
	add r0, r4, r6
	add r1, r1, r0
	ldr r6, _0800A34C @ =0x000007FF
	ldr r4, _0800A350 @ =0x08622AB4
_0800A328:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r5
	bne _0800A354
	add r0, r3, #0
	b _0800A360
	.align 2, 0
_0800A340: .4byte 0x020192E4
_0800A344: .4byte 0x00000D64
_0800A348: .4byte 0x00000684
_0800A34C: .4byte 0x000007FF
_0800A350: .4byte gCardIdToNumber
_0800A354:
	add r1, #4
	add r3, #1
	cmp r3, r2
	blt _0800A328
_0800A35C:
	mov r0, #1
	neg r0, r0
_0800A360:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end FindHandCardByNumber
	.align 2, 0

