	thumb_func_start sub_08014BAC
sub_08014BAC: @ 0x08014BAC
	push {r4, r5, lr}
	ldr r4, _08014BD4 @ =0x020185C0
	ldr r0, _08014BD8 @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08014BE0
	cmp r0, #1
	beq _08014C08
	ldr r2, _08014BDC @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _08014C28
	.align 2, 0
_08014BD4: .4byte 0x020185C0
_08014BD8: .4byte 0x0000080A
_08014BDC: .4byte 0x0000080D
_08014BE0:
	bl sub_08060B4C
	cmp r0, #0
	beq _08014C28
	ldr r1, _08014C04 @ =0x0201CFB0
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldrh r0, [r4, #2]
	mov r1, #0x96
	lsl r1, r1, #1
	mov r2, #0
	bl sub_0800688C
	b _08014C12
	.align 2, 0
_08014C04: .4byte 0x0201CFB0
_08014C08:
	bl sub_08006D08
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08014C28
_08014C12:
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
_08014C28:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08014BAC
	.align 2, 0

