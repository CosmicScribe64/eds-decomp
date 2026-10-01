	thumb_func_start sub_08018C3C
sub_08018C3C: @ 0x08018C3C
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08018C84 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08018C88 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r6, #0
	beq _08018D3C
	ldr r0, _08018C8C @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08018C90 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _08018C94 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _08018C98
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018544
	b _08018CAC
	.align 2, 0
_08018C84: .4byte 0x00000D64
_08018C88: .4byte 0x0201930C
_08018C8C: .4byte 0x000007FF
_08018C90: .4byte gUnk_08622AB4
_08018C94: .4byte 0xFFFFF880
_08018C98:
	mov r0, #0x81
	cmp r4, #0
	beq _08018CA0
	ldr r0, _08018D44 @ =0x00008081
_08018CA0:
	lsl r1, r5, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08018CAC:
	cmp r5, #4
	bgt _08018CBA
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl sub_08017DE0
_08018CBA:
	cmp r5, #0xA
	bne _08018CEC
	mov r0, #1
	and r0, r4
	ldr r1, _08018D48 @ =0x00000D64
	mul r1, r0
	mov r0, #0xB9
	lsl r0, r0, #3
	add r1, r1, r0
	ldr r0, _08018D4C @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08018CEC
	mov r0, #0x11
	cmp r4, #0
	beq _08018CE2
	ldr r0, _08018D50 @ =0x00008011
_08018CE2:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08018CEC:
	ldr r0, _08018D54 @ =0x000007FF
	and r6, r0
	lsl r0, r6, #1
	ldr r1, _08018D58 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08018D5C @ =0x00000447
	ldrh r0, [r0]
	cmp r0, r1
	bne _08018D3C
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800CD68
	lsl r6, r0, #0x10
	lsr r3, r6, #0x10
	ldr r0, _08018D60 @ =0x0000FFFF
	cmp r3, r0
	beq _08018D3C
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08018D48 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08018D4C @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08018D3C
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	lsr r1, r6, #0x18
	mov r2, #1
	bl sub_08018544
_08018D3C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018D44: .4byte 0x00008081
_08018D48: .4byte 0x00000D64
_08018D4C: .4byte 0x0201930C
_08018D50: .4byte 0x00008011
_08018D54: .4byte 0x000007FF
_08018D58: .4byte gUnk_08622AB4
_08018D5C: .4byte 0x00000447
_08018D60: .4byte 0x0000FFFF
	thumb_func_end sub_08018C3C

