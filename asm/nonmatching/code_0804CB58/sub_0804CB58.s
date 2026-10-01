	thumb_func_start sub_0804CB58
sub_0804CB58: @ 0x0804CB58
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x100
	add r7, r0, #0
	ldr r0, _0804CB84 @ =0x020192E0
	ldr r2, _0804CB88 @ =0x00001B16
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	add r3, r0, #0
	cmp r1, #0xC
	bls _0804CB78
	b _0804D1A0
_0804CB78:
	lsl r0, r1, #2
	ldr r1, _0804CB8C @ =0x0804CB90
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804CB84: .4byte 0x020192E0
_0804CB88: .4byte 0x00001B16
_0804CB8C: .4byte 0x0804CB90
_0804CB90:
	.4byte _0804CBC4
	.4byte _0804CDB4
	.4byte _0804CE20
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D1A0
	.4byte _0804D00C
	.4byte _0804D0F8
	.4byte _0804D144
_0804CBC4:
	ldr r6, _0804CC54 @ =0x02018450
	lsl r0, r7, #1
	add r1, r0, r7
	lsl r1, r1, #2
	add r3, r1, r6
	ldrh r1, [r3, #0x12]
	mov r8, r0
	cmp r1, #0
	bne _0804CBD8
	b _0804D0C0
_0804CBD8:
	ldr r2, _0804CC58 @ =0x020192E4
	mov r4, #1
	mov r9, r4
	add r0, r7, #0
	and r0, r4
	ldr r1, _0804CC5C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _0804CBF2
	b _0804D0C0
_0804CBF2:
	lsl r0, r1, #0x1E
	cmp r0, #0
	bge _0804CBFA
	b _0804D0C0
_0804CBFA:
	ldrh r1, [r3, #0x12]
	lsl r4, r7, #0x18
	lsr r2, r4, #0x18
	ldrh r5, [r6]
	lsl r0, r5, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	mov r3, #1
	sub r3, r3, r7
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	ldrb r5, [r6, #1]
	lsl r0, r5, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r3, r0
	add r0, r7, #0
	bl sub_08019894
	mov r0, r9
	sub r5, r0, r7
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #2
	add r2, r0, r6
	ldr r0, _0804CC60 @ =0x000007FF
	ldrh r1, [r2, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0804CC64 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0804CC68 @ =0x0000020A
	mov r9, r4
	cmp r1, r0
	beq _0804CCA2
	cmp r1, r0
	bgt _0804CC6C
	cmp r1, #0x71
	beq _0804CC7C
	cmp r1, #0xDB
	beq _0804CC90
	b _0804CD0E
	.align 2, 0
_0804CC54: .4byte 0x02018450
_0804CC58: .4byte 0x020192E4
_0804CC5C: .4byte 0x00000D64
_0804CC60: .4byte 0x000007FF
_0804CC64: .4byte gUnk_08622AB4
_0804CC68: .4byte 0x0000020A
_0804CC6C:
	mov r0, #0xA6
	lsl r0, r0, #3
	cmp r1, r0
	beq _0804CCB4
	add r0, #0xB7
	cmp r1, r0
	beq _0804CCB4
	b _0804CD0E
_0804CC7C:
	ldrh r1, [r2, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r7, #0
	mov r1, #1
	mov r2, #1
	bl sub_08022784
	b _0804CD0E
_0804CC90:
	ldrh r1, [r2, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #1
	bl sub_080199E0
	b _0804CD0E
_0804CCA2:
	ldrh r1, [r2, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r7, #0
	mov r1, #2
	bl sub_080199E0
	b _0804CD0E
_0804CCB4:
	mov r1, #1
	sub r3, r1, r7
	add r0, r3, #0
	and r0, r1
	lsl r0, r0, #0x1F
	ldr r4, _0804CD94 @ =0x02018450
	ldrb r1, [r4, #1]
	lsl r5, r1, #0x1C
	lsr r1, r5, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0xE2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	lsl r1, r3, #1
	add r1, r1, r3
	lsl r1, r1, #2
	add r1, r1, r4
	ldrh r1, [r1, #0xA]
	orr r0, r1
	mov r2, r8
	add r3, r2, r7
	lsl r3, r3, #2
	add r3, r3, r4
	mov r6, #0xF
	add r1, r7, #0
	and r1, r6
	ldrh r4, [r4]
	lsl r2, r4, #0x17
	lsr r2, r2, #0x1D
	lsl r2, r2, #4
	orr r1, r2
	mov r2, #1
	sub r2, r2, r7
	and r2, r6
	lsr r5, r5, #0x1D
	lsl r5, r5, #4
	orr r2, r5
	lsl r2, r2, #8
	orr r1, r2
	lsl r1, r1, #0x10
	ldrh r3, [r3, #0x12]
	orr r1, r3
	bl sub_0801FBCC
_0804CD0E:
	ldr r4, _0804CD94 @ =0x02018450
	mov r3, r8
	add r0, r3, r7
	lsl r0, r0, #2
	add r6, r0, r4
	ldr r0, _0804CD98 @ =0x000007FF
	ldrh r5, [r6, #0xA]
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _0804CD9C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804CDA0 @ =0x000002DA
	cmp r1, r0
	beq _0804CD34
	ldr r0, _0804CDA4 @ =0x00000536
	cmp r1, r0
	beq _0804CD34
	b _0804D0C0
_0804CD34:
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0800A430
	ldr r1, _0804CDA8 @ =0x0000FFFF
	cmp r0, r1
	bne _0804CD48
	b _0804D0C0
_0804CD48:
	ldr r2, _0804CDAC @ =0x020192E4
	mov r1, #1
	sub r5, r1, r7
	add r0, r5, #0
	and r0, r1
	ldr r1, _0804CDB0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _0804CD62
	b _0804D0C0
_0804CD62:
	lsl r0, r1, #0x1E
	cmp r0, #0
	bge _0804CD6A
	b _0804D0C0
_0804CD6A:
	ldrh r1, [r6, #0x12]
	mov r3, r9
	lsr r2, r3, #0x18
	ldrh r3, [r4]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	mov r3, #1
	sub r3, r3, r7
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	ldrb r4, [r4, #1]
	lsl r0, r4, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r3, r0
	add r0, r5, #0
	bl sub_08019894
	b _0804D0C0
_0804CD94: .4byte 0x02018450
_0804CD98: .4byte 0x000007FF
_0804CD9C: .4byte gUnk_08622AB4
_0804CDA0: .4byte 0x000002DA
_0804CDA4: .4byte 0x00000536
_0804CDA8: .4byte 0x0000FFFF
_0804CDAC: .4byte 0x020192E4
_0804CDB0: .4byte 0x00000D64
_0804CDB4:
	ldr r0, _0804CDFC @ =0x02018450
	mov r3, #1
	sub r2, r3, r7
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r0, [r1, #0x12]
	cmp r0, #0
	beq _0804CE10
	ldr r4, _0804CE00 @ =0x020192E4
	add r0, r2, #0
	and r0, r3
	ldr r1, _0804CE04 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x1F
	cmp r0, #0
	bne _0804CE10
	lsl r0, r1, #0x1E
	cmp r0, #0
	blt _0804CE10
	add r0, r2, #0
	mov r1, #0x39
	bl sub_0800A2A8
	cmp r0, #0
	beq _0804CE10
	ldr r5, _0804CE08 @ =0x00001B12
	add r0, r4, r5
	ldr r1, _0804CE0C @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #0x14
	b _0804D182
_0804CDFC: .4byte 0x02018450
_0804CE00: .4byte 0x020192E4
_0804CE04: .4byte 0x00000D64
_0804CE08: .4byte 0x00001B12
_0804CE0C: .4byte 0xFFFFFE01
_0804CE10:
	ldr r2, _0804CE18 @ =0x020192E0
	ldr r3, _0804CE1C @ =0x00001B16
	add r2, r2, r3
	b _0804D0C6
_0804CE18: .4byte 0x020192E0
_0804CE1C: .4byte 0x00001B16
_0804CE20:
	ldr r5, _0804CEB0 @ =0x02018450
	mov r1, #1
	sub r6, r1, r7
	lsl r0, r6, #1
	add r0, r0, r6
	lsl r0, r0, #2
	add r3, r0, r5
	ldrh r0, [r3, #0x12]
	cmp r0, #0
	bne _0804CE36
	b _0804D0C0
_0804CE36:
	ldr r2, _0804CEB4 @ =0x020192E4
	add r0, r6, #0
	and r0, r1
	ldr r1, _0804CEB8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _0804CE4C
	b _0804D0C0
_0804CE4C:
	lsl r0, r1, #0x1E
	cmp r0, #0
	bge _0804CE54
	b _0804D0C0
_0804CE54:
	ldrh r3, [r3, #0x12]
	mov r8, r3
	lsl r4, r7, #0x18
	lsr r2, r4, #0x18
	ldrh r1, [r5]
	lsl r0, r1, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	mov r3, #1
	sub r3, r3, r7
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	ldrb r1, [r5, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r3, r0
	add r0, r6, #0
	mov r1, r8
	bl sub_08019894
	lsl r1, r7, #1
	add r0, r1, r7
	lsl r0, r0, #2
	add r5, r0, r5
	ldr r0, _0804CEBC @ =0x000007FF
	ldrh r2, [r5, #0xA]
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _0804CEC0 @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	ldr r0, _0804CEC4 @ =0x0000020A
	mov r8, r1
	mov r9, r4
	cmp r2, r0
	beq _0804CEFE
	cmp r2, r0
	bgt _0804CEC8
	cmp r2, #0x71
	beq _0804CED8
	cmp r2, #0xDB
	beq _0804CEEC
	b _0804CF66
	.align 2, 0
_0804CEB0: .4byte 0x02018450
_0804CEB4: .4byte 0x020192E4
_0804CEB8: .4byte 0x00000D64
_0804CEBC: .4byte 0x000007FF
_0804CEC0: .4byte gUnk_08622AB4
_0804CEC4: .4byte 0x0000020A
_0804CEC8:
	mov r0, #0xA6
	lsl r0, r0, #3
	cmp r2, r0
	beq _0804CF10
	add r0, #0xB7
	cmp r2, r0
	beq _0804CF10
	b _0804CF66
_0804CED8:
	ldrh r1, [r5, #0xA]
	add r0, r7, #0
	bl sub_080197E0
	add r0, r6, #0
	mov r1, #1
	mov r2, #1
	bl sub_08022784
	b _0804CF66
_0804CEEC:
	ldrh r1, [r5, #0xA]
	add r0, r7, #0
	bl sub_080197E0
	add r0, r7, #0
	mov r1, #1
	bl sub_080199E0
	b _0804CF66
_0804CEFE:
	ldrh r1, [r5, #0xA]
	add r0, r7, #0
	bl sub_080197E0
	add r0, r6, #0
	mov r1, #2
	bl sub_080199E0
	b _0804CF66
_0804CF10:
	mov r4, #1
	lsl r0, r7, #0x1F
	ldr r6, _0804CFEC @ =0x02018450
	ldrh r1, [r6]
	lsl r5, r1, #0x17
	lsr r1, r5, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0xD2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	mov r2, r8
	add r1, r2, r7
	lsl r1, r1, #2
	add r1, r1, r6
	ldrh r1, [r1, #0xA]
	orr r0, r1
	sub r4, r4, r7
	lsl r3, r4, #1
	add r3, r3, r4
	lsl r3, r3, #2
	add r3, r3, r6
	mov r1, #1
	sub r1, r1, r7
	mov r4, #0xF
	and r1, r4
	ldrb r6, [r6, #1]
	lsl r2, r6, #0x1C
	lsr r2, r2, #0x1D
	lsl r2, r2, #4
	orr r1, r2
	add r2, r7, #0
	and r2, r4
	lsr r5, r5, #0x1D
	lsl r5, r5, #4
	orr r2, r5
	lsl r2, r2, #8
	orr r1, r2
	lsl r1, r1, #0x10
	ldrh r3, [r3, #0x12]
	orr r1, r3
	bl sub_0801FBCC
_0804CF66:
	ldr r4, _0804CFEC @ =0x02018450
	mov r6, #1
	sub r2, r6, r7
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r5, r0, r4
	ldr r0, _0804CFF0 @ =0x000007FF
	ldrh r3, [r5, #0xA]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0804CFF4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804CFF8 @ =0x000002DA
	cmp r1, r0
	beq _0804CF90
	ldr r0, _0804CFFC @ =0x00000536
	cmp r1, r0
	beq _0804CF90
	b _0804D0C0
_0804CF90:
	ldrb r3, [r4, #1]
	lsl r1, r3, #0x1C
	lsr r1, r1, #0x1D
	add r0, r2, #0
	bl sub_0800A430
	ldr r1, _0804D000 @ =0x0000FFFF
	cmp r0, r1
	bne _0804CFA4
	b _0804D0C0
_0804CFA4:
	ldr r2, _0804D004 @ =0x020192E4
	add r0, r7, #0
	and r0, r6
	ldr r1, _0804D008 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _0804CFBA
	b _0804D0C0
_0804CFBA:
	lsl r0, r1, #0x1E
	cmp r0, #0
	bge _0804CFC2
	b _0804D0C0
_0804CFC2:
	ldrh r1, [r5, #0x12]
	mov r5, r9
	lsr r2, r5, #0x18
	ldrh r3, [r4]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	mov r3, #1
	sub r3, r3, r7
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	ldrb r4, [r4, #1]
	lsl r0, r4, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r3, r0
	add r0, r7, #0
	bl sub_08019894
	b _0804D0C0
_0804CFEC: .4byte 0x02018450
_0804CFF0: .4byte 0x000007FF
_0804CFF4: .4byte gUnk_08622AB4
_0804CFF8: .4byte 0x000002DA
_0804CFFC: .4byte 0x00000536
_0804D000: .4byte 0x0000FFFF
_0804D004: .4byte 0x020192E4
_0804D008: .4byte 0x00000D64
_0804D00C:
	cmp r7, #0
	beq _0804D05C
	ldr r1, _0804D044 @ =0x08085B7C
	ldr r0, _0804D048 @ =0x08623E66
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r5, _0804D04C @ =0x0822C720
	add r2, r2, r5
	mov r0, sp
	bl sub_080753F4
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804D050 @ =0x00000915
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r2, _0804D054 @ =0x020192E0
	ldr r0, _0804D058 @ =0x00001B16
	add r2, r2, r0
	b _0804D074
	.align 2, 0
_0804D044: .4byte gUnk_08085B7C
_0804D048: .4byte gUnk_08623E66
_0804D04C: .4byte gUnk_0822C720
_0804D050: .4byte 0x00000915
_0804D054: .4byte 0x020192E0
_0804D058: .4byte 0x00001B16
_0804D05C:
	ldr r1, _0804D08C @ =0x02015EE8
	mov r2, #1
	add r0, r2, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	bne _0804D0A0
	ldr r0, _0804D090 @ =0x0201AE60
	strh r2, [r0, #0x14]
	ldr r2, _0804D094 @ =0x020192E0
	ldr r1, _0804D098 @ =0x00001B16
	add r2, r2, r1
_0804D074:
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804D09C @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0804D0C0
_0804D08C: .4byte 0x02015EE8
_0804D090: .4byte 0x0201AE60
_0804D094: .4byte 0x020192E0
_0804D098: .4byte 0x00001B16
_0804D09C: .4byte 0xFFFFFE01
_0804D0A0:
	ldr r0, _0804D0E0 @ =0x0000F057
	ldr r1, _0804D0E4 @ =0x08623E66
	ldrh r1, [r1]
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _0804D0E8 @ =0x02017FB0
	mov r2, #0x8A
	lsl r2, r2, #3
	add r1, r1, r2
	mov r0, #3
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_0804D0C0:
	ldr r2, _0804D0EC @ =0x020192E0
	ldr r4, _0804D0F0 @ =0x00001B16
	add r2, r2, r4
_0804D0C6:
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804D0F4 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0804D0DC:
	mov r0, #0
	b _0804D288
_0804D0E0: .4byte 0x0000F057
_0804D0E4: .4byte gUnk_08623E66
_0804D0E8: .4byte 0x02017FB0
_0804D0EC: .4byte 0x020192E0
_0804D0F0: .4byte 0x00001B16
_0804D0F4: .4byte 0xFFFFFE01
_0804D0F8:
	ldr r2, _0804D130 @ =0x02017FB0
	mov r5, #0x8A
	lsl r5, r5, #3
	add r0, r2, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _0804D0DC
	ldr r1, _0804D134 @ =0x0201AE60
	ldr r4, _0804D138 @ =0x0000045A
	add r0, r2, r4
	ldrh r0, [r0]
	strh r0, [r1, #0x14]
	ldr r5, _0804D13C @ =0x00001B16
	add r3, r3, r5
	ldrh r2, [r3]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804D140 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0804D0DC
	.align 2, 0
_0804D130: .4byte 0x02017FB0
_0804D134: .4byte 0x0201AE60
_0804D138: .4byte 0x0000045A
_0804D13C: .4byte 0x00001B16
_0804D140: .4byte 0xFFFFFE01
_0804D144:
	ldr r0, _0804D188 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0804D174
	mov r0, #1
	sub r4, r0, r7
	ldr r0, _0804D18C @ =0x08623E66
	ldrh r1, [r0]
	add r0, r4, #0
	bl sub_080197C0
	add r0, r4, #0
	mov r1, #0x39
	bl sub_0801A130
	cmp r0, #0
	beq _0804D174
	ldr r0, _0804D190 @ =0x02018450
	lsl r1, r4, #1
	add r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r0
	mov r0, #0
	strh r0, [r1, #0x12]
_0804D174:
	ldr r0, _0804D194 @ =0x020192E0
	ldr r1, _0804D198 @ =0x00001B16
	add r0, r0, r1
	ldr r1, _0804D19C @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #4
_0804D182:
	orr r1, r2
	strh r1, [r0]
	b _0804D0DC
_0804D188: .4byte 0x0201AE60
_0804D18C: .4byte gUnk_08623E66
_0804D190: .4byte 0x02018450
_0804D194: .4byte 0x020192E0
_0804D198: .4byte 0x00001B16
_0804D19C: .4byte 0xFFFFFE01
_0804D1A0:
	mov r1, #2
	neg r1, r1
	add r0, r1, #0
	ldrb r4, [r3, #0xC]
	and r0, r4
	strb r0, [r3, #0xC]
	mov r5, #0xD7
	lsl r5, r5, #4
	add r0, r3, r5
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	ldr r4, _0804D254 @ =0x02018450
	mov r8, r4
	mov r6, #1
	sub r4, r6, r7
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #2
	mov r1, r8
	add r5, r0, r1
	ldr r0, _0804D258 @ =0x000007FF
	ldrh r2, [r5, #0xA]
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _0804D25C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0804D260 @ =0x000004B1
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804D226
	ldrb r2, [r5, #8]
	lsl r0, r2, #0x1C
	cmp r0, #0
	blt _0804D226
	add r2, r4, #0
	and r2, r6
	mov r0, r8
	ldrb r1, [r0, #1]
	lsl r1, r1, #0x1C
	lsr r0, r1, #0x1D
	mov r1, #0x94
	mul r1, r0
	ldr r0, _0804D264 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r0, r3, #0
	add r0, #0x2C
	add r1, r1, r0
	add r0, r6, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804D226
	ldrh r1, [r5, #0xA]
	add r0, r4, #0
	bl sub_080197E0
	mov r2, r8
	ldrb r2, [r2, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	add r0, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_0804D226:
	mov r4, #0
	ldr r6, _0804D254 @ =0x02018450
	ldr r3, _0804D268 @ =0x08624730
	mov r8, r3
	mov r5, #0
_0804D230:
	ldr r0, _0804D254 @ =0x02018450
	add r0, r5, r0
	ldrb r1, [r0, #8]
	lsl r0, r1, #0x19
	cmp r0, #0
	bge _0804D27E
	lsl r0, r1, #0x1C
	cmp r0, #0
	blt _0804D27E
	mov r0, r8
	ldrh r1, [r0]
	lsl r0, r4, #0x18
	lsr r3, r0, #0x18
	cmp r4, r7
	bne _0804D26C
	ldrh r2, [r6]
	lsl r0, r2, #0x17
	b _0804D270
_0804D254: .4byte 0x02018450
_0804D258: .4byte 0x000007FF
_0804D25C: .4byte gUnk_08622AB4
_0804D260: .4byte 0x000004B1
_0804D264: .4byte 0x00000D64
_0804D268: .4byte gUnk_08624730
_0804D26C:
	ldrb r2, [r6, #1]
	lsl r0, r2, #0x1C
_0804D270:
	lsr r0, r0, #0x1D
	lsl r2, r0, #8
	orr r2, r3
	add r0, r7, #0
	mov r3, #3
	bl sub_08017AB4
_0804D27E:
	add r5, #0xC
	add r4, #1
	cmp r4, #1
	ble _0804D230
	mov r0, #1
_0804D288:
	add sp, #0x100
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804CB58
	.align 2, 0

