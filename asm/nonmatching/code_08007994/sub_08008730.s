	thumb_func_start sub_08008730
sub_08008730: @ 0x08008730
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r4, #0
	ldr r7, _08008774 @ =0x0201930C
	mov r1, #1
	and r1, r0
	ldr r0, _08008778 @ =0x00000D64
	mul r1, r0
	ldr r6, _0800877C @ =0x000007FF
_08008744:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r1
	add r3, r0, r7
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08008784
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _08008784
	and r2, r6
	lsl r0, r2, #1
	ldr r2, _08008780 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r5
	bne _08008784
	add r0, r4, #0
	b _0800878E
	.align 2, 0
_08008774: .4byte 0x0201930C
_08008778: .4byte 0x00000D64
_0800877C: .4byte 0x000007FF
_08008780: .4byte gUnk_08622AB4
_08008784:
	add r4, #1
	cmp r4, #4
	ble _08008744
	mov r0, #1
	neg r0, r0
_0800878E:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08008730

