	thumb_func_start sub_0800D4C8
sub_0800D4C8: @ 0x0800D4C8
	push {r4, lr}
	ldr r3, _0800D514 @ =0x020185C0
	ldrh r0, [r3]
	lsr r1, r0, #0xF
	add r4, r1, #0
	ldrh r0, [r3, #2]
	cmp r0, #0
	beq _0800D4E8
	ldr r2, _0800D518 @ =0x020192E4
	ldr r0, _0800D51C @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #1
	ldrb r2, [r1, #8]
	orr r0, r2
	strb r0, [r1, #8]
_0800D4E8:
	ldrh r0, [r3, #4]
	cmp r0, #0
	beq _0800D500
	ldr r2, _0800D518 @ =0x020192E4
	ldr r0, _0800D51C @ =0x00000D64
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r2
	mov r0, #2
	ldrb r2, [r1, #8]
	orr r0, r2
	strb r0, [r1, #8]
_0800D500:
	ldr r0, _0800D520 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_0800D514: .4byte 0x020185C0
_0800D518: .4byte 0x020192E4
_0800D51C: .4byte 0x00000D64
_0800D520: .4byte 0x0000080D
	thumb_func_end sub_0800D4C8

