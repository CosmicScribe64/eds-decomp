	thumb_func_start sub_08018ED8
sub_08018ED8: @ 0x08018ED8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	add r6, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	mov r0, #1
	mov r9, r0
	mov r5, r8
	and r5, r0
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08018F78 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08018F7C @ =0x0201930C
	add r7, r1, r0
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r6, #4
	ble _08018F18
	b _08019054
_08018F18:
	cmp r4, #0
	bne _08018F1E
	b _08019054
_08018F1E:
	mov r0, #0x7E
	mov r1, r8
	cmp r1, #0
	beq _08018F28
	ldr r0, _08018F80 @ =0x0000807E
_08018F28:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r2, sl
	mov r3, #0
	bl sub_0801EC58
	mov r0, r9
	ldrb r7, [r7, #6]
	and r0, r7
	cmp r0, #0
	beq _08018F8C
	ldr r0, _08018F84 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08018F88 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0x5E
	bne _08018FE0
	mov r0, r8
	add r1, r4, #0
	mov r2, #0
	bl sub_0802CFA0
	cmp r0, #0
	beq _08018FE0
	lsl r0, r5, #0x1F
	mov r1, #0x1F
	and r1, r6
	lsl r1, r1, #0x10
	mov r2, #0xA2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	orr r0, r4
	mov r1, #0
	bl sub_0801FBCC
	b _08018FE0
	.align 2, 0
_08018F78: .4byte 0x00000D64
_08018F7C: .4byte 0x0201930C
_08018F80: .4byte 0x0000807E
_08018F84: .4byte 0x000007FF
_08018F88: .4byte gUnk_08622AB4
_08018F8C:
	ldr r0, _08018FA8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08018FAC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0xA1
	beq _08018FB8
	cmp r1, #0xA1
	bgt _08018FB0
	cmp r1, #0x77
	beq _08018FB8
	b _08018FE0
	.align 2, 0
_08018FA8: .4byte 0x000007FF
_08018FAC: .4byte gUnk_08622AB4
_08018FB0:
	mov r0, #0xF8
	lsl r0, r0, #1
	cmp r1, r0
	bne _08018FE0
_08018FB8:
	mov r0, r8
	add r1, r4, #0
	mov r2, #0
	bl sub_0802CFA0
	cmp r0, #0
	beq _08018FE0
	mov r1, r8
	lsl r0, r1, #0x1F
	mov r1, #0x1F
	and r1, r6
	lsl r1, r1, #0x10
	mov r2, #0xA2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	orr r0, r4
	mov r1, #0
	bl sub_0801FBCC
_08018FE0:
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _08019054
	mov r1, sl
	cmp r1, #0
	beq _08019054
	mov r7, #1
	mov r0, r8
	and r7, r0
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08019064 @ =0x00000D64
	mul r0, r7
	add r1, r1, r0
	ldr r0, _08019068 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08019054
	ldr r0, _0801906C @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08019070 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	beq _08019054
	ldr r5, _08019074 @ =0x000005FA
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08019054
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08019054
	lsl r0, r7, #0x1F
	mov r1, #0x1F
	and r1, r6
	lsl r1, r1, #0x10
	mov r2, #0xA2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	orr r0, r4
	mov r1, #0
	bl sub_0801FBCC
_08019054:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019064: .4byte 0x00000D64
_08019068: .4byte 0x0201930C
_0801906C: .4byte 0x000007FF
_08019070: .4byte gUnk_08622AB4
_08019074: .4byte 0x000005FA
	thumb_func_end sub_08018ED8

