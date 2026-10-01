	thumb_func_start sub_080199E0
sub_080199E0: @ 0x080199E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	add r5, r0, #0
	str r1, [sp, #0]
	ldr r2, _08019B10 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08019B14 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	mov r8, r0
	ldr r6, _08019B18 @ =0x00000519
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08008524
	add r4, r0, #0
	cmp r4, #0
	ble _08019A3A
	mov r2, #0x73
	cmp r5, #0
	beq _08019A18
	ldr r2, _08019B1C @ =0x00008073
_08019A18:
	lsl r0, r6, #1
	ldr r1, _08019B20 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	add r0, r5, #0
	bl sub_08019980
_08019A3A:
	mov r2, #0
	mov sl, r2
	ldr r3, [sp, #0]
	cmp sl, r3
	blt _08019A46
	b _08019C66
_08019A46:
	mov r0, #1
	and r0, r5
	lsl r1, r0, #0x1F
	str r1, [sp, #0xC]
	ldr r1, _08019B14 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	str r2, [sp, #4]
_08019A56:
	ldr r0, _08019B24 @ =0x02019AA8
	ldr r3, [sp, #4]
	add r0, r3, r0
	mov r2, sl
	lsl r1, r2, #2
	add r7, r0, r1
	mov r3, #0
	str r3, [sp, #8]
	mov r9, r3
	mov r0, #0x61
	cmp r5, #0
	beq _08019A70
	ldr r0, _08019B28 @ =0x00008061
_08019A70:
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r1, [r7]
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	cmp r0, r5
	beq _08019B52
	lsl r0, r1, #0xE
	cmp r0, #0
	bge _08019B52
	lsl r0, r1, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08019B2C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08019B30 @ =0x000002FA
	ldrh r0, [r0]
	cmp r0, r1
	bne _08019B52
	add r0, r5, #0
	bl sub_08008A44
	add r4, r0, #0
	mov r2, #1
	str r2, [sp, #8]
	mov r9, r2
	mov r0, #0x73
	cmp r5, #0
	beq _08019AB0
	ldr r0, _08019B1C @ =0x00008073
_08019AB0:
	ldr r1, [r7]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	cmp r4, #0
	blt _08019B3C
	mov r6, #0xC4
	cmp r5, #0
	beq _08019ACA
	ldr r6, _08019B34 @ =0x000080C4
_08019ACA:
	ldr r1, [r7]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r3, #0xF
	mov r2, r8
	and r2, r3
	lsl r2, r2, #4
	add r0, r4, #0
	and r0, r3
	orr r2, r0
	mov r3, #0xC0
	lsl r3, r3, #2
	add r0, r3, #0
	orr r2, r0
	add r0, r6, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x90
	cmp r5, #0
	beq _08019AF6
	ldr r0, _08019B38 @ =0x00008090
_08019AF6:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	mov r2, #0xD
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019860
	b _08019B52
	.align 2, 0
_08019B10: .4byte 0x020192E4
_08019B14: .4byte 0x00000D64
_08019B18: .4byte 0x00000519
_08019B1C: .4byte 0x00008073
_08019B20: .4byte gUnk_08623DF4
_08019B24: .4byte 0x02019AA8
_08019B28: .4byte 0x00008061
_08019B2C: .4byte gUnk_08622AB4
_08019B30: .4byte 0x000002FA
_08019B34: .4byte 0x000080C4
_08019B38: .4byte 0x00008090
_08019B3C:
	mov r0, #0xC0
	cmp r5, #0
	beq _08019B44
	ldr r0, _08019BC4 @ =0x000080C0
_08019B44:
	mov r2, r8
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_08019B52:
	ldr r1, _08019BC8 @ =0x020192E4
	ldr r3, [sp, #4]
	add r0, r3, r1
	ldrb r0, [r0, #0xB]
	lsl r0, r0, #0x1D
	cmp r0, #0
	beq _08019C50
	ldr r0, _08019BCC @ =0x00001AC8
	add r1, r1, r0
	mov r0, #0x80
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08019C50
	ldr r1, [sp, #8]
	cmp r1, #0
	bne _08019C50
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r2, #0x73
	cmp r5, #0
	beq _08019B82
	ldr r2, _08019BD0 @ =0x00008073
_08019B82:
	ldr r0, _08019BD4 @ =0x0862431C
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08019840
	ldr r2, _08019BD8 @ =0x000007FF
	add r1, r2, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08019BDC @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08019C50
	cmp r0, #0x15
	blt _08019BEA
	cmp r0, #0x17
	ble _08019BE0
	cmp r0, #0x18
	beq _08019BE4
	b _08019BEA
	.align 2, 0
_08019BC4: .4byte 0x000080C0
_08019BC8: .4byte 0x020192E4
_08019BCC: .4byte 0x00001AC8
_08019BD0: .4byte 0x00008073
_08019BD4: .4byte gUnk_0862431C
_08019BD8: .4byte 0x000007FF
_08019BDC: .4byte gUnk_08621DE0
_08019BE0:
	mov r1, #0
	b _08019C04
_08019BE4:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _08019C04
_08019BEA:
	ldr r0, _08019CAC @ =0x000007FF
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08019CB0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_08019C04:
	ldr r0, _08019CB4 @ =0x000005DB
	cmp r1, r0
	bls _08019C50
	mov r0, #0xC0
	cmp r5, #0
	beq _08019C12
	ldr r0, _08019CB8 @ =0x000080C0
_08019C12:
	mov r2, r8
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r3, _08019CAC @ =0x000007FF
	add r1, r3, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08019CBC @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08019CC0 @ =0x000004DA
	ldrh r0, [r0]
	cmp r0, r1
	bne _08019C4C
	mov r0, #0x1F
	and r0, r5
	lsl r0, r0, #0x10
	ldr r1, _08019CC4 @ =0x3A600000
	orr r0, r1
	ldr r2, [sp, #0xC]
	orr r0, r2
	orr r0, r4
	mov r1, #0
	bl sub_0801FBCC
_08019C4C:
	mov r3, #1
	mov r9, r3
_08019C50:
	mov r0, r9
	cmp r0, #0
	bne _08019C5A
	mov r1, #1
	add r8, r1
_08019C5A:
	mov r2, #1
	add sl, r2
	ldr r3, [sp, #0]
	cmp sl, r3
	bge _08019C66
	b _08019A56
_08019C66:
	ldr r0, _08019CC8 @ =0x020192E0
	ldr r1, _08019CCC @ =0x00001B12
	add r6, r0, r1
	mov r0, #0x1C
	ldrb r2, [r6]
	and r0, r2
	cmp r0, #0
	beq _08019C9C
	mov r4, #1
	sub r0, r4, r5
	bl sub_08046BA8
	ldrb r6, [r6]
	lsl r0, r6, #0x1E
	lsr r0, r0, #0x1F
	sub r4, r4, r0
	lsl r2, r5, #0x18
	lsr r2, r2, #0x18
	mov r0, #1
	sub r0, r0, r5
	lsl r0, r0, #0x18
	lsr r0, r0, #8
	orr r2, r0
	add r0, r4, #0
	mov r1, #0x1A
	bl sub_08042AB0
_08019C9C:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019CAC: .4byte 0x000007FF
_08019CB0: .4byte gUnk_08621DE0
_08019CB4: .4byte 0x000005DB
_08019CB8: .4byte 0x000080C0
_08019CBC: .4byte gUnk_08622AB4
_08019CC0: .4byte 0x000004DA
_08019CC4: .4byte 0x3A600000
_08019CC8: .4byte 0x020192E0
_08019CCC: .4byte 0x00001B12
	thumb_func_end sub_080199E0

