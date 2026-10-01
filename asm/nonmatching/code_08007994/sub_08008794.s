	thumb_func_start sub_08008794
sub_08008794: @ 0x08008794
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r3, #0
	mov r2, #0
	ldr r6, _080087DC @ =0x0201930C
	mov r1, #1
	and r1, r0
	ldr r0, _080087E0 @ =0x00000D64
	mul r1, r0
	ldr r5, _080087E4 @ =0x000007FF
_080087AA:
	mov r0, #0x94
	mul r0, r2
	add r0, r0, r1
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080087CC
	and r0, r5
	lsl r0, r0, #1
	ldr r7, _080087E8 @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, r4
	bne _080087CC
	add r3, #1
_080087CC:
	add r2, #1
	cmp r2, #4
	ble _080087AA
	add r0, r3, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080087DC: .4byte 0x0201930C
_080087E0: .4byte 0x00000D64
_080087E4: .4byte 0x000007FF
_080087E8: .4byte gUnk_08622AB4
	thumb_func_end sub_08008794

