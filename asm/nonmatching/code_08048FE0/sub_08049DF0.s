	thumb_func_start sub_08049DF0
sub_08049DF0: @ 0x08049DF0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r1, #0
	add r7, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	mov r0, #1
	and r0, r6
	ldr r1, _08049E24 @ =0x00000D64
	mul r1, r0
	ldr r0, _08049E28 @ =0x0201930C
	mov ip, r0
	add r1, ip
	mov r0, #0x94
	mul r0, r7
	mov r2, #0xB9
	lsl r2, r2, #2
	add r0, r0, r2
	add r3, r1, r0
	mov r5, #0
	cmp r6, #0
	beq _08049E2C
	mov r0, #0
	b _08049FE4
_08049E24: .4byte 0x00000D64
_08049E28: .4byte 0x0201930C
_08049E2C:
	ldr r4, _08049EE0 @ =0x000007FF
	mov r0, r8
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _08049EE4 @ =0x08621DE0
	add r0, r0, r1
	ldr r2, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08049E48
	b _08049F48
_08049E48:
	cmp r0, #0x16
	beq _08049E4E
	b _08049FAC
_08049E4E:
	add r1, r3, #0
	add r1, #0x91
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08049EA6
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	cmp r0, #0
	blt _08049EA6
	mov r3, #0
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r2, r0
	lsr r0, r2, #0x11
	cmp r0, #5
	bne _08049E74
	mov r3, #1
_08049E74:
	ldr r0, _08049EE8 @ =0x00001AE6
	add r0, ip
	ldrb r1, [r0]
	mov r0, #0x1C
	and r0, r1
	cmp r0, #8
	beq _08049E86
	cmp r0, #0x10
	bne _08049E90
_08049E86:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _08049E90
	mov r3, #1
_08049E90:
	cmp r3, #0
	beq _08049EA6
	add r0, r6, #0
	mov r1, r8
	mov r2, #0
	bl sub_0802CFA0
	cmp r0, #0
	beq _08049EA6
	mov r0, #0x40
	orr r5, r0
_08049EA6:
	ldr r3, _08049EEC @ =0x020192E0
	ldr r2, _08049EF0 @ =0x00001B12
	add r0, r3, r2
	ldrb r1, [r0]
	mov r0, #0x1C
	and r0, r1
	cmp r0, #4
	bne _08049FAC
	mov r2, #2
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _08049EFC
	ldr r0, _08049EE0 @ =0x000007FF
	mov r4, r8
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08049EF4 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08049EF8 @ =0x00000489
	ldrh r0, [r0]
	cmp r0, r1
	bne _08049FAC
	mov r0, #1
	sub r0, r0, r6
	add r1, r7, #5
	mov r2, #3
	b _08049F9C
	.align 2, 0
_08049EE0: .4byte 0x000007FF
_08049EE4: .4byte gUnk_08621DE0
_08049EE8: .4byte 0x00001AE6
_08049EEC: .4byte 0x020192E0
_08049EF0: .4byte 0x00001B12
_08049EF4: .4byte gUnk_08622AB4
_08049EF8: .4byte 0x00000489
_08049EFC:
	ldr r0, _08049F3C @ =0x000007FF
	mov r4, r8
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08049F40 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0x85
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _08049FAC
	mov r0, #1
	and r0, r6
	ldr r1, _08049F44 @ =0x00000D64
	mul r1, r0
	mov r0, #0x94
	mul r0, r7
	add r1, r1, r0
	mov r4, #0xC4
	lsl r4, r4, #2
	add r0, r3, r4
	add r1, r1, r0
	add r0, r2, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08049FAC
	add r1, r7, #5
	add r0, r6, #0
	mov r2, #2
	b _08049F9C
	.align 2, 0
_08049F3C: .4byte 0x000007FF
_08049F40: .4byte gUnk_08622AB4
_08049F44: .4byte 0x00000D64
_08049F48:
	add r1, r3, #0
	add r1, #0x91
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08049FAC
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r1, r0, #0x1F
	lsl r0, r4, #1
	ldr r2, _08049F78 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _08049F7C @ =0x0000052C
	cmp r2, r0
	beq _08049F90
	cmp r2, r0
	bgt _08049F84
	ldr r0, _08049F80 @ =0x000003F9
	cmp r2, r0
	beq _08049F90
	b _08049F92
	.align 2, 0
_08049F78: .4byte gUnk_08622AB4
_08049F7C: .4byte 0x0000052C
_08049F80: .4byte 0x000003F9
_08049F84:
	ldr r0, _08049FF0 @ =0x00000594
	cmp r2, r0
	beq _08049F90
	add r0, #0x68
	cmp r2, r0
	bne _08049F92
_08049F90:
	mov r1, #0
_08049F92:
	cmp r1, #0
	bne _08049FAC
	add r1, r7, #5
	add r0, r6, #0
	mov r2, #0
_08049F9C:
	bl sub_0802CFD0
	cmp r0, #0
	beq _08049FAC
	mov r0, #0x40
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
_08049FAC:
	ldr r0, _08049FF4 @ =0x000007FF
	mov r4, r8
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08049FF8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08049FE2
	cmp r0, #0x15
	blt _08049FE2
	ldr r2, _08049FFC @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _0804A000 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsr r0, r0, #6
	cmp r0, #0
	beq _08049FE2
	ldr r0, _0804A004 @ =0x0000FFBF
	and r5, r0
_08049FE2:
	add r0, r5, #0
_08049FE4:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08049FF0: .4byte 0x00000594
_08049FF4: .4byte 0x000007FF
_08049FF8: .4byte gUnk_08621DE0
_08049FFC: .4byte 0x020192E4
_0804A000: .4byte 0x00000D64
_0804A004: .4byte 0x0000FFBF
	thumb_func_end sub_08049DF0

