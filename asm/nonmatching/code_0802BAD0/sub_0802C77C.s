	thumb_func_start sub_0802C77C
sub_0802C77C: @ 0x0802C77C
	push {r4, lr}
	add r4, r0, #0
	mov r1, #0
	ldr r0, _0802C7B4 @ =0x000007FF
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _0802C7B8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _0802C7BC @ =0x00000406
	cmp r2, r0
	beq _0802C810
	cmp r2, r0
	bgt _0802C7D8
	ldr r0, _0802C7C0 @ =0x000001F9
	cmp r2, r0
	beq _0802C816
	cmp r2, r0
	bgt _0802C7C4
	sub r0, #0x68
	cmp r2, r0
	beq _0802C810
	add r0, #0x12
	cmp r2, r0
	beq _0802C820
	b _0802C83E
	.align 2, 0
_0802C7B4: .4byte 0x000007FF
_0802C7B8: .4byte gUnk_08622AB4
_0802C7BC: .4byte 0x00000406
_0802C7C0: .4byte 0x000001F9
_0802C7C4:
	ldr r0, _0802C7D4 @ =0x000003F9
	cmp r2, r0
	beq _0802C804
	add r0, #0xB
	cmp r2, r0
	beq _0802C828
	b _0802C83E
	.align 2, 0
_0802C7D4: .4byte 0x000003F9
_0802C7D8:
	mov r0, #0x91
	lsl r0, r0, #3
	cmp r2, r0
	beq _0802C80A
	cmp r2, r0
	bgt _0802C7F2
	sub r0, #0x59
	cmp r2, r0
	bgt _0802C83E
	sub r0, #1
	cmp r2, r0
	blt _0802C83E
	b _0802C810
_0802C7F2:
	ldr r0, _0802C800 @ =0x00000489
	cmp r2, r0
	beq _0802C804
	add r0, #0x16
	cmp r2, r0
	beq _0802C810
	b _0802C83E
_0802C800: .4byte 0x00000489
_0802C804:
	mov r1, #0xFA
	lsl r1, r1, #1
	b _0802C842
_0802C80A:
	mov r1, #0xC8
	lsl r1, r1, #2
	b _0802C842
_0802C810:
	mov r1, #0xFA
	lsl r1, r1, #2
	b _0802C842
_0802C816:
	ldr r1, _0802C81C @ =0x00000BB8
	b _0802C842
	.align 2, 0
_0802C81C: .4byte 0x00000BB8
_0802C820:
	ldr r1, _0802C824 @ =0x00001388
	b _0802C842
_0802C824: .4byte 0x00001388
_0802C828:
	ldr r2, _0802C868 @ =0x020192E4
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802C86C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_080754A4
	add r1, r0, #0
_0802C83E:
	cmp r1, #0
	ble _0802C85E
_0802C842:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r2, #0x43
	cmp r0, #0
	beq _0802C850
	ldr r2, _0802C870 @ =0x00008043
_0802C850:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0802C85E:
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802C868: .4byte 0x020192E4
_0802C86C: .4byte 0x00000D64
_0802C870: .4byte 0x00008043
	thumb_func_end sub_0802C77C

