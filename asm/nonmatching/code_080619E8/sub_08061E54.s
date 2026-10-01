	thumb_func_start sub_08061E54
sub_08061E54: @ 0x08061E54
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r0, _08061E94 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08061E98 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08061EA8
	cmp r0, #0x16
	beq _08061EBC
	ldr r0, _08061E9C @ =0x0006004E
	ldr r2, _08061EA0 @ =0x0819897C
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r2, r1, r2
	ldr r2, [r2]
	ldr r3, _08061EA4 @ =0x08198950
	add r1, r1, r3
	ldr r3, [r1]
	b _08061EC2
_08061E94: .4byte 0x000007FF
_08061E98: .4byte gUnk_08621DE0
_08061E9C: .4byte 0x0006004E
_08061EA0: .4byte gUnk_0819897C
_08061EA4: .4byte gUnk_08198950
_08061EA8:
	ldr r0, _08061EB0 @ =0x0006004E
	ldr r2, _08061EB4 @ =0x086366A8
	ldr r3, _08061EB8 @ =0x08636348
	b _08061EC2
_08061EB0: .4byte 0x0006004E
_08061EB4: .4byte gUnk_086366A8
_08061EB8: .4byte gUnk_08636348
_08061EBC:
	ldr r0, _08061EF4 @ =0x0006004E
	ldr r2, _08061EF8 @ =0x08636728
	ldr r3, _08061EFC @ =0x08636368
_08061EC2:
	mov r1, #0x70
	bl sub_0806196C
	ldr r0, _08061F00 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08061F04 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08061EE4
	b _08062100
_08061EE4:
	cmp r0, #0x15
	blt _08061F10
	cmp r0, #0x17
	ble _08061F08
	cmp r0, #0x18
	beq _08061F0C
	b _08061F10
	.align 2, 0
_08061EF4: .4byte 0x0006004E
_08061EF8: .4byte gUnk_08636728
_08061EFC: .4byte gUnk_08636368
_08061F00: .4byte 0x000007FF
_08061F04: .4byte gUnk_08621DE0
_08061F08:
	mov r0, #0
	b _08061F26
_08061F0C:
	mov r0, #0xA
	b _08061F26
_08061F10:
	ldr r0, _08061F4C @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08061F50 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08061F26:
	add r4, r0, #0
	mov r5, #0
	cmp r5, r4
	bge _08061F7E
	mov r0, #0x54
	mov r9, r0
	mov r7, #0xA0
	lsl r7, r7, #0xD
	mov r6, #0x54
_08061F38:
	cmp r4, #9
	bgt _08061F5C
	add r0, r6, #0
	orr r0, r7
	mov r1, #0x80
	ldr r2, _08061F54 @ =0x0822C360
	ldr r3, _08061F58 @ =0x0822C300
	bl sub_080618C4
	b _08061F76
_08061F4C: .4byte 0x000007FF
_08061F50: .4byte gUnk_08621DE0
_08061F54: .4byte gUnk_0822C360
_08061F58: .4byte gUnk_0822C300
_08061F5C:
	mov r0, #0x4E
	mul r0, r5
	add r1, r4, #0
	bl __divsi3
	mov r1, r9
	sub r0, r1, r0
	orr r0, r7
	mov r1, #0x80
	ldr r2, _08061FBC @ =0x0822C360
	ldr r3, _08061FC0 @ =0x0822C300
	bl sub_080618C4
_08061F76:
	sub r6, #8
	add r5, #1
	cmp r5, r4
	blt _08061F38
_08061F7E:
	ldr r0, _08061FC4 @ =0x0076003F
	ldr r2, _08061FC8 @ =0x0863856C
	ldr r4, _08061FCC @ =0x0863840C
	mov r1, #0x90
	add r3, r4, #0
	bl sub_080618C4
	ldr r0, _08061FD0 @ =0x00760047
	ldr r2, _08061FD4 @ =0x0863858C
	mov r1, #0x90
	add r3, r4, #0
	bl sub_080618C4
	ldr r0, _08061FD8 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08061FDC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08061FEA
	cmp r0, #0x17
	ble _08061FE0
	cmp r0, #0x18
	beq _08061FE4
	b _08061FEA
_08061FBC: .4byte gUnk_0822C360
_08061FC0: .4byte gUnk_0822C300
_08061FC4: .4byte 0x0076003F
_08061FC8: .4byte gUnk_0863856C
_08061FCC: .4byte gUnk_0863840C
_08061FD0: .4byte 0x00760047
_08061FD4: .4byte gUnk_0863858C
_08061FD8: .4byte 0x000007FF
_08061FDC: .4byte gUnk_08621DE0
_08061FE0:
	mov r0, #0
	b _08062002
_08061FE4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08062002
_08061FEA:
	ldr r0, _08062074 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08062078 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08062002:
	add r5, r0, #0
	mov r6, #0x57
_08062006:
	mov r4, #0xEC
	lsl r4, r4, #0xF
	orr r4, r6
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	lsl r2, r2, #5
	ldr r0, _0806207C @ =0x0863842C
	add r2, r2, r0
	add r0, r4, #0
	mov r1, #0x90
	ldr r3, _08062080 @ =0x0863840C
	bl sub_080618C4
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #4
	cmp r5, #0
	bne _08062006
	ldr r0, _08062084 @ =0x007E003F
	ldr r2, _08062088 @ =0x086385AC
	ldr r4, _08062080 @ =0x0863840C
	mov r1, #0x90
	add r3, r4, #0
	bl sub_080618C4
	ldr r0, _0806208C @ =0x007E0047
	ldr r2, _08062090 @ =0x086385CC
	mov r1, #0x90
	add r3, r4, #0
	bl sub_080618C4
	ldr r0, _08062074 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08062078 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0806209E
	cmp r0, #0x17
	ble _08062094
	cmp r0, #0x18
	beq _08062098
	b _0806209E
_08062074: .4byte 0x000007FF
_08062078: .4byte gUnk_08621DE0
_0806207C: .4byte gUnk_0863842C
_08062080: .4byte gUnk_0863840C
_08062084: .4byte 0x007E003F
_08062088: .4byte gUnk_086385AC
_0806208C: .4byte 0x007E0047
_08062090: .4byte gUnk_086385CC
_08062094:
	mov r0, #0
	b _080620B6
_08062098:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080620B6
_0806209E:
	ldr r0, _080620EC @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080620F0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _080620F4 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080620B6:
	add r5, r0, #0
	mov r6, #0x57
_080620BA:
	mov r4, #0xFC
	lsl r4, r4, #0xF
	orr r4, r6
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	lsl r2, r2, #5
	ldr r0, _080620F8 @ =0x0863842C
	add r2, r2, r0
	add r0, r4, #0
	mov r1, #0x90
	ldr r3, _080620FC @ =0x0863840C
	bl sub_080618C4
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #4
	cmp r5, #0
	bne _080620BA
	b _08062128
_080620EC: .4byte 0x000007FF
_080620F0: .4byte gUnk_08621DE0
_080620F4: .4byte 0x000001FF
_080620F8: .4byte gUnk_0863842C
_080620FC: .4byte gUnk_0863840C
_08062100:
	cmp r0, #0x16
	bgt _08062112
	cmp r0, #0x15
	blt _08062112
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r2, r1, #0x11
	b _08062114
_08062112:
	mov r2, #0
_08062114:
	cmp r2, #0
	beq _08062128
	ldr r0, _08062134 @ =0x00140050
	lsl r2, r2, #5
	ldr r1, _08062138 @ =0x08637374
	add r2, r2, r1
	ldr r3, _0806213C @ =0x08637454
	mov r1, #0x90
	bl sub_080618C4
_08062128:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08062134: .4byte 0x00140050
_08062138: .4byte gUnk_08637374
_0806213C: .4byte gUnk_08637454
	thumb_func_end sub_08061E54

