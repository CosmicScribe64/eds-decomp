	thumb_func_start sub_080436B8
sub_080436B8: @ 0x080436B8
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	add r6, r0, #0
	mov r7, #0
	mov r5, #0
	mov r3, #1
	and r3, r6
	ldr r0, _08043718 @ =0x000007FF
	add r2, r0, #0
_080436CA:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _0804371C @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08043720 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r6, #0
	add r1, r5, #0
	str r2, [sp, #0]
	str r3, [sp, #4]
	bl sub_08008A6C
	ldr r2, [sp, #0]
	ldr r3, [sp, #4]
	cmp r0, #0
	beq _08043744
	add r0, r4, #0
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08043724 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043730
	cmp r0, #0x17
	ble _08043728
	cmp r0, #0x18
	beq _0804372C
	b _08043730
	.align 2, 0
_08043718: .4byte 0x000007FF
_0804371C: .4byte 0x00000D64
_08043720: .4byte 0x0201930C
_08043724: .4byte gUnk_08621DE0
_08043728:
	mov r0, #0
	b _08043742
_0804372C:
	mov r0, #0xA
	b _08043742
_08043730:
	and r4, r2
	lsl r0, r4, #2
	ldr r1, _08043754 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043742:
	add r7, r0, r7
_08043744:
	add r5, #1
	cmp r5, #4
	ble _080436CA
	add r0, r7, #0
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08043754: .4byte gUnk_08621DE0
	thumb_func_end sub_080436B8

