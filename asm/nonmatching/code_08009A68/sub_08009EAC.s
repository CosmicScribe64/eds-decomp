	thumb_func_start sub_08009EAC
sub_08009EAC: @ 0x08009EAC
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r2, _08009F08 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08009F0C @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldr r3, _08009F10 @ =0x00000684
	add r2, r2, r3
	add r0, r0, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #2
	add r6, r0, r1
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08009FBA
	ldr r3, _08009F14 @ =0x000007FF
	add r0, r1, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _08009F18 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r2, _08009F1C @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08009FBA
	add r2, r1, #0
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _08009F18 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08009F20 @ =0x00000776
	cmp r1, r0
	bne _08009F24
	mov r0, #3
	b _08009F86
	.align 2, 0
_08009F08: .4byte 0x020192E4
_08009F0C: .4byte 0x00000D64
_08009F10: .4byte 0x00000684
_08009F14: .4byte 0x000007FF
_08009F18: .4byte gUnk_08622AB4
_08009F1C: .4byte 0xFFFFF880
_08009F20: .4byte 0x00000776
_08009F24:
	cmp r1, r0
	blt _08009F34
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08009F34
	mov r0, #1
	b _08009F86
_08009F34:
	ldr r0, _08009F58 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08009F5C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08009F66
	cmp r0, #0x16
	bgt _08009F60
	cmp r0, #0x15
	beq _08009F6A
	b _08009F72
	.align 2, 0
_08009F58: .4byte 0x000007FF
_08009F5C: .4byte gUnk_08621DE0
_08009F60:
	cmp r0, #0x17
	beq _08009F6E
	b _08009F72
_08009F66:
	mov r0, #7
	b _08009F86
_08009F6A:
	mov r0, #8
	b _08009F86
_08009F6E:
	mov r0, #9
	b _08009F86
_08009F72:
	ldr r0, _08009F98 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r2, _08009F9C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08009F86:
	cmp r0, #2
	bne _08009FA0
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08007D18
	b _08009FBA
_08009F98: .4byte 0x000007FF
_08009F9C: .4byte gUnk_08621DE0
_08009FA0:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_08007558
	ldr r2, _08009FC0 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08009FC4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #2]
	add r1, #1
	strb r1, [r0, #2]
_08009FBA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08009FC0: .4byte 0x020192E4
_08009FC4: .4byte 0x00000D64
	thumb_func_end sub_08009EAC

