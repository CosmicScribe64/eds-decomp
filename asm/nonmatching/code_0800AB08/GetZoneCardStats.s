	thumb_func_start GetZoneCardStats
GetZoneCardStats: @ 0x0800ABC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x50
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r9, r2
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, #0
	str r1, [sp, #0x10]
	mov r2, #0
	str r2, [sp, #0x14]
	mov r3, #0
	str r3, [sp, #0x18]
	mov sl, r3
	mov r4, #0
	str r4, [sp, #0x1C]
	mov r5, #0
	str r5, [sp, #0x20]
	mov r6, #0
	str r6, [sp, #0x24]
	mov r7, #0
	str r7, [sp, #0x28]
	str r0, [sp, #0x2C]
	str r1, [sp, #0x30]
	mov r0, #1
	ldr r2, [sp, #0]
	and r0, r2
	ldr r1, _0800AC98 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	ldr r2, _0800AC9C @ =0x0201930C
	add r1, r3, r2
	mov r0, #0x94
	ldr r4, [sp, #4]
	mul r0, r4
	add r5, r1, r0
	str r6, [sp, #0x34]
	str r7, [sp, #0x38]
	mov r1, r9
	strb r7, [r1, #2]
	ldr r4, [sp, #0x38]
	str r4, [r1, #4]
	str r4, [r1, #8]
	add r0, r0, r3
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r1]
	cmp r0, #0
	bne _0800AC3A
	bl _0800C878 @ far jump
_0800AC3A:
	ldr r3, _0800ACA0 @ =0x000007FF
	add r0, r3, #0
	ldrh r6, [r1]
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _0800ACA4 @ =0x08621DE0
	add r0, r0, r7
	ldr r1, [r0]
	mov r4, #0xF8
	lsl r4, r4, #0x11
	and r1, r4
	lsr r1, r1, #0x14
	mov r0, #0x1F
	and r1, r0
	mov r0, r9
	strb r1, [r0, #2]
	add r0, r3, #0
	mov r2, r9
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #2
	add r6, r7, #0
	add r0, r0, r6
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	lsl r0, r0, #5
	mov r2, #0x1F
	and r1, r2
	orr r1, r0
	mov r7, r9
	strb r1, [r7, #2]
	ldrh r1, [r7]
	add r0, r1, #0
	and r0, r3
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	add r2, r1, #0
	cmp r0, #0x15
	blt _0800ACB2
	cmp r0, #0x17
	ble _0800ACA8
	cmp r0, #0x18
	beq _0800ACAC
	b _0800ACB2
_0800AC98: .4byte 0x00000D64
_0800AC9C: .4byte 0x0201930C
_0800ACA0: .4byte 0x000007FF
_0800ACA4: .4byte gCardStats
_0800ACA8:
	mov r0, #0
	b _0800ACC8
_0800ACAC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800ACC8
_0800ACB2:
	ldr r0, _0800ACF0 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _0800ACF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800ACC8:
	mov r3, r9
	str r0, [r3, #4]
	ldr r0, _0800ACF0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r4, _0800ACF4 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800AD02
	cmp r0, #0x17
	ble _0800ACF8
	cmp r0, #0x18
	beq _0800ACFC
	b _0800AD02
	.align 2, 0
_0800ACF0: .4byte 0x000007FF
_0800ACF4: .4byte gCardStats
_0800ACF8:
	mov r0, #0
	b _0800AD18
_0800ACFC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800AD18
_0800AD02:
	ldr r0, _0800AD6C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r6, _0800AD70 @ =0x08621DE0
	add r0, r0, r6
	ldr r1, [r0]
	ldr r0, _0800AD74 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800AD18:
	mov r7, r9
	str r0, [r7, #8]
	ldr r0, [sp, #4]
	cmp r0, #4
	ble _0800AD26
	bl _0800C878 @ far jump
_0800AD26:
	mov r2, #1
	ldr r1, [sp, #0]
	and r2, r1
	mov r0, #0x94
	ldr r3, [sp, #4]
	mul r0, r3
	ldr r1, _0800AD78 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r4, _0800AD7C @ =0x0201930C
	add r2, r0, r4
	mov r0, #2
	ldrb r6, [r2, #6]
	and r0, r6
	cmp r0, #0
	bne _0800AD4A
	bl _0800C878 @ far jump
_0800AD4A:
	ldr r0, _0800AD6C @ =0x000007FF
	ldrh r7, [r7]
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0800AD80 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0800AD84 @ =0x000004E6
	cmp r1, r0
	beq _0800ADB0
	cmp r1, r0
	bgt _0800AD88
	sub r0, #0x8E
	cmp r1, r0
	beq _0800AD9C
	b _0800ADDA
	.align 2, 0
_0800AD6C: .4byte 0x000007FF
_0800AD70: .4byte gCardStats
_0800AD74: .4byte 0x000001FF
_0800AD78: .4byte 0x00000D64
_0800AD7C: .4byte 0x0201930C
_0800AD80: .4byte gCardIdToNumber
_0800AD84: .4byte 0x000004E6
_0800AD88:
	ldr r0, _0800AD98 @ =0x0000052E
	cmp r1, r0
	beq _0800ADCC
	add r0, #3
	cmp r1, r0
	beq _0800ADCC
	b _0800ADDA
	.align 2, 0
_0800AD98: .4byte 0x0000052E
_0800AD9C:
	mov r0, #0x20
	ldrb r2, [r2, #7]
	and r0, r2
	cmp r0, #0
	bne _0800ADDA
	mov r2, r9
	ldr r0, [r2, #4]
	lsl r0, r0, #1
	str r0, [r2, #4]
	b _0800ADDA
_0800ADB0:
	mov r0, #0x20
	ldrb r2, [r2, #7]
	and r0, r2
	cmp r0, #0
	bne _0800ADDA
	mov r0, #0xFA
	lsl r0, r0, #1
	ldr r3, [sp, #0x2C]
	add r3, r3, r0
	str r3, [sp, #0x2C]
	ldr r4, [sp, #0x30]
	add r4, r4, r0
	str r4, [sp, #0x30]
	b _0800ADDA
_0800ADCC:
	bl GetFaceUpFieldMagicNumber
	ldr r1, _0800AF68 @ =0x0000014D
	cmp r0, r1
	bne _0800ADDA
	mov r6, #1
	str r6, [sp, #0x38]
_0800ADDA:
	mov r7, #0
	str r7, [sp, #8]
	add r5, #0x8C
	str r5, [sp, #0x4C]
	ldr r0, [sp, #0]
	mov r1, #1
	and r0, r1
	ldr r2, _0800AF6C @ =0x00000D64
	mov ip, r2
	mov r3, ip
	mul r3, r0
	str r3, [sp, #0x3C]
	mov r4, #0x20
	neg r4, r4
	mov r8, r4
_0800ADF8:
	mov r0, #0x94
	ldr r5, [sp, #8]
	mul r0, r5
	ldr r6, [sp, #0x3C]
	add r0, r0, r6
	ldr r7, _0800AF70 @ =0x0201930C
	add r2, r0, r7
	ldr r3, [r2]
	lsl r0, r3, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0800AE4E
	ldr r0, _0800AF74 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0800AF78 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r4, _0800AF7C @ =0x000002FA
	cmp r0, r4
	bne _0800AE4E
	lsl r0, r3, #0xE
	cmp r0, #0
	bge _0800AE4E
	mov r0, #2
	ldrb r5, [r2, #6]
	and r0, r5
	cmp r0, #0
	beq _0800AE4E
	ldrh r6, [r2, #4]
	ldr r7, [sp, #0x34]
	cmp r6, r7
	bls _0800AE4E
	ldrh r2, [r2, #4]
	str r2, [sp, #0x34]
	mov r0, r8
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #0xA
	orr r0, r1
	mov r2, r9
	strb r0, [r2, #2]
_0800AE4E:
	mov r4, #0
	ldr r6, [sp, #8]
	add r6, #1
	mov r0, #0x94
	ldr r5, [sp, #8]
	add r3, r5, #0
	mul r3, r0
	mov r5, #0x1F
_0800AE5E:
	add r0, r4, #0
	mov r7, #1
	and r0, r7
	mov r1, ip
	mul r1, r0
	add r0, r1, #0
	add r0, r0, r3
	ldr r7, _0800AF80 @ =0x020195F0
	add r2, r0, r7
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0800AECA
	ldr r0, _0800AF74 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0800AF78 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r7, _0800AF84 @ =0x00000479
	cmp r0, r7
	bne _0800AECA
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _0800AECA
	add r1, r2, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800AECA
	ldrh r7, [r2, #4]
	ldr r0, [sp, #0x34]
	cmp r7, r0
	bls _0800AECA
	ldrh r1, [r2, #4]
	str r1, [sp, #0x34]
	add r0, r2, #0
	add r0, #0x90
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x1B
	and r1, r5
	mov r0, r8
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	orr r0, r1
	mov r7, r9
	strb r0, [r7, #2]
_0800AECA:
	add r4, #1
	cmp r4, #1
	ble _0800AE5E
	str r6, [sp, #8]
	cmp r6, #4
	ble _0800ADF8
	mov r0, #0
	str r0, [sp, #8]
	mov r1, #1
	ldr r2, [sp, #0]
	and r1, r2
	mov r0, #0x94
	ldr r3, [sp, #4]
	mul r0, r3
	ldr r4, _0800AF6C @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r5, _0800AF70 @ =0x0201930C
	add r0, r0, r5
	add r0, #0x8A
	ldr r6, [sp, #8]
	ldrh r0, [r0]
	cmp r6, r0
	blt _0800AEFE
	bl _0800BE04 @ far jump
_0800AEFE:
	mov r7, #1
	mov ip, r7
	ldr r0, [sp, #0]
	mov r1, ip
	and r0, r1
	mov r4, #0x94
	ldr r3, [sp, #4]
	add r2, r3, #0
	mul r2, r4
	ldr r5, _0800AF6C @ =0x00000D64
	mul r0, r5
	add r2, r2, r0
	ldr r6, _0800AF70 @ =0x0201930C
	add r2, r2, r6
	ldr r7, [sp, #8]
	lsl r3, r7, #1
	add r0, r2, #0
	add r0, #0xA
	add r0, r0, r3
	add r2, #0x4A
	add r2, r2, r3
	ldrh r1, [r2]
	lsr r6, r1, #8
	ldrh r7, [r0]
	ldrb r5, [r0]
	lsr r0, r7, #8
	mov r8, r0
	add r1, r5, #0
	mov r0, ip
	and r1, r0
	mov r0, r8
	mul r0, r4
	ldr r4, _0800AF6C @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _0800AF70 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #0x40]
	ldrb r0, [r2]
	sub r0, #1
	cmp r0, #0xC
	bls _0800AF5C
	bl _0800BDDC @ far jump
_0800AF5C:
	lsl r0, r0, #2
	ldr r1, _0800AF88 @ =0x0800AF8C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800AF68: .4byte 0x0000014D
_0800AF6C: .4byte 0x00000D64
_0800AF70: .4byte 0x0201930C
_0800AF74: .4byte 0x000007FF
_0800AF78: .4byte gCardIdToNumber
_0800AF7C: .4byte 0x000002FA
_0800AF80: .4byte 0x020195F0
_0800AF84: .4byte 0x00000479
_0800AF88: .4byte 0x0800AF8C
_0800AF8C:
	.4byte _0800B32C
	.4byte _0800B260
	.4byte _0800AFC0
	.4byte _0800BCE4
	.4byte _0800BBD6
	.4byte _0800BDDC
	.4byte _0800BDDC
	.4byte _0800BCF4
	.4byte _0800BDA4
	.4byte _0800BDBE
	.4byte _0800BDC4
	.4byte _0800BDD2
	.4byte _0800BBC4
_0800AFC0:
	ldr r2, [sp, #0x38]
	cmp r2, #0
	beq _0800AFE2
	ldr r0, _0800B018 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r4, _0800B01C @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0800AFE2
	bl _0800BDDC @ far jump
_0800AFE2:
	ldr r4, _0800B018 @ =0x000007FF
	and r7, r4
	lsl r0, r7, #1
	ldr r5, _0800B020 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _0800B024 @ =0x0000043A
	cmp r1, r0
	bne _0800AFF6
	b _0800B134
_0800AFF6:
	cmp r1, r0
	bgt _0800B054
	sub r0, #0x43
	cmp r1, r0
	bne _0800B002
	b _0800B0EC
_0800B002:
	cmp r1, r0
	bgt _0800B02C
	ldr r0, _0800B028 @ =0x00000105
	cmp r1, r0
	beq _0800B0A4
	add r0, #0xA7
	cmp r1, r0
	beq _0800B0B6
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B018: .4byte 0x000007FF
_0800B01C: .4byte gCardStats
_0800B020: .4byte gCardIdToNumber
_0800B024: .4byte 0x0000043A
_0800B028: .4byte 0x00000105
_0800B02C:
	ldr r0, _0800B040 @ =0x00000433
	cmp r1, r0
	beq _0800B118
	cmp r1, r0
	bgt _0800B044
	sub r0, #0x3B
	cmp r1, r0
	beq _0800B102
	bl _0800BDDC @ far jump
_0800B040: .4byte 0x00000433
_0800B044:
	ldr r0, _0800B050 @ =0x00000434
	cmp r1, r0
	beq _0800B12A
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B050: .4byte 0x00000434
_0800B054:
	ldr r0, _0800B074 @ =0x000004CF
	cmp r1, r0
	bne _0800B05C
	b _0800B1C0
_0800B05C:
	cmp r1, r0
	bgt _0800B078
	sub r0, #0x1C
	cmp r1, r0
	beq _0800B14A
	add r0, #1
	cmp r1, r0
	bne _0800B06E
	b _0800B184
_0800B06E:
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B074: .4byte 0x000004CF
_0800B078:
	ldr r0, _0800B090 @ =0x00000587
	cmp r1, r0
	bne _0800B080
	b _0800B1E0
_0800B080:
	cmp r1, r0
	bgt _0800B094
	sub r0, #0x65
	cmp r1, r0
	bne _0800B08C
	b _0800B1D6
_0800B08C:
	bl _0800BDDC @ far jump
_0800B090: .4byte 0x00000587
_0800B094:
	ldr r0, _0800B0A0 @ =0x000005FE
	cmp r1, r0
	bne _0800B09C
	b _0800B1F2
_0800B09C:
	bl _0800BDDC @ far jump
_0800B0A0: .4byte 0x000005FE
_0800B0A4:
	add r0, r6, #1
	mov r1, #0xAF
	lsl r1, r1, #2
	mul r0, r1
	ldr r6, [sp, #0x24]
	add r6, r6, r0
	str r6, [sp, #0x24]
	bl _0800BDDC @ far jump
_0800B0B6:
	mov r2, #1
	ldr r7, [sp, #0]
	and r2, r7
	mov r0, #0x94
	ldr r1, [sp, #4]
	mul r0, r1
	ldr r1, _0800B0E4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r2, _0800B0E8 @ =0x0201930C
	add r0, r0, r2
	add r0, #0x4A
	add r0, r0, r3
	ldrh r0, [r0]
	lsr r0, r0, #8
	cmp r0, #0
	beq _0800B0DA
	b _0800B1D6
_0800B0DA:
	ldr r4, [sp, #0xC]
	add r4, #1
	str r4, [sp, #0xC]
	bl _0800BDDC @ far jump
_0800B0E4: .4byte 0x00000D64
_0800B0E8: .4byte 0x0201930C
_0800B0EC:
	add r1, r6, #1
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r5, [sp, #0x24]
	add r5, r5, r0
	str r5, [sp, #0x24]
	bl _0800BDDC @ far jump
_0800B102:
	add r1, r6, #1
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r6, [sp, #0x28]
	add r6, r6, r0
	str r6, [sp, #0x28]
	bl _0800BDDC @ far jump
_0800B118:
	add r0, r6, #1
	mov r1, #0xAF
	lsl r1, r1, #2
	mul r0, r1
	ldr r7, [sp, #0x24]
	add r7, r7, r0
	str r7, [sp, #0x24]
	bl _0800BDDC @ far jump
_0800B12A:
	add r0, r6, #1
	mov r1, #0xAF
	lsl r1, r1, #2
	mul r0, r1
	b _0800B172
_0800B134:
	add r1, r6, #1
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r2, [sp, #0x28]
	sub r2, r2, r0
	str r2, [sp, #0x28]
	bl _0800BDDC @ far jump
_0800B14A:
	mov r2, #1
	ldr r4, [sp, #0]
	and r2, r4
	mov r0, #0x94
	ldr r5, [sp, #4]
	mul r0, r5
	ldr r1, _0800B17C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r6, _0800B180 @ =0x0201930C
	add r0, r0, r6
	add r0, #0x4A
	add r0, r0, r3
	ldrh r0, [r0]
	lsr r1, r0, #8
	mov r0, #0x64
	mul r0, r1
	ldr r7, [sp, #0x24]
	add r7, r7, r0
	str r7, [sp, #0x24]
_0800B172:
	ldr r1, [sp, #0x28]
	add r1, r1, r0
	str r1, [sp, #0x28]
	bl _0800BDDC @ far jump
_0800B17C: .4byte 0x00000D64
_0800B180: .4byte 0x0201930C
_0800B184:
	mov r2, #1
	ldr r4, [sp, #0]
	and r2, r4
	mov r0, #0x94
	ldr r5, [sp, #4]
	mul r0, r5
	ldr r1, _0800B1B8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r6, _0800B1BC @ =0x0201930C
	add r0, r0, r6
	add r0, #0x4A
	add r0, r0, r3
	ldrh r0, [r0]
	lsr r1, r0, #8
	mov r0, #0x64
	mul r0, r1
	ldr r7, [sp, #0x24]
	sub r7, r7, r0
	str r7, [sp, #0x24]
	ldr r1, [sp, #0x28]
	sub r1, r1, r0
	str r1, [sp, #0x28]
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B1B8: .4byte 0x00000D64
_0800B1BC: .4byte 0x0201930C
_0800B1C0:
	add r1, r6, #1
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r2, [sp, #0x24]
	sub r2, r2, r0
	str r2, [sp, #0x24]
	bl _0800BDDC @ far jump
_0800B1D6:
	ldr r3, [sp, #0x10]
	add r3, #1
	str r3, [sp, #0x10]
	bl _0800BDDC @ far jump
_0800B1E0:
	add r0, r6, #1
	mov r1, #0xAF
	lsl r1, r1, #2
	mul r0, r1
	ldr r4, [sp, #0x24]
	sub r4, r4, r0
	str r4, [sp, #0x24]
	bl _0800BDDC @ far jump
_0800B1F2:
	mov r5, #0
	str r5, [sp, #8]
	ldr r3, _0800B250 @ =0x020192E4
	mov r0, #1
	ldr r6, [sp, #0]
	and r0, r6
	ldr r1, _0800B254 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r3
	ldrb r1, [r0, #4]
	cmp r5, r1
	blt _0800B210
	bl _0800BDDC @ far jump
_0800B210:
	ldr r7, _0800B258 @ =0x00000904
	add r0, r3, r7
	add r3, r1, #0
	add r1, r2, r0
	mov r2, #0xF8
	lsl r2, r2, #0x11
	str r3, [sp, #8]
_0800B21E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r5, _0800B25C @ =0x08621DE0
	add r0, r0, r5
	ldr r0, [r0]
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0800B23C
	ldr r6, [sp, #0x24]
	add r6, #0x64
	str r6, [sp, #0x24]
_0800B23C:
	add r1, #4
	ldr r7, [sp, #8]
	sub r7, #1
	str r7, [sp, #8]
	cmp r7, #0
	bne _0800B21E
	str r3, [sp, #8]
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B250: .4byte 0x020192E4
_0800B254: .4byte 0x00000D64
_0800B258: .4byte 0x00000904
_0800B25C: .4byte gCardStats
_0800B260:
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0800B26A
	bl _0800BDDC @ far jump
_0800B26A:
	mov r6, #1
	add r0, r5, #0
	and r0, r6
	mov r4, #0x94
	mov r1, r8
	mul r1, r4
	ldr r2, _0800B2B0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r7, _0800B2B4 @ =0x0201930C
	add r1, r1, r7
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800B290
	bl _0800BDDC @ far jump
_0800B290:
	ldr r0, _0800B2B8 @ =0x000007FF
	ldr r1, [sp, #0x40]
	and r1, r0
	lsl r0, r1, #1
	ldr r7, _0800B2BC @ =0x08622AB4
	add r0, r0, r7
	ldrh r1, [r0]
	ldr r0, _0800B2C0 @ =0x0000044B
	cmp r1, r0
	beq _0800B318
	cmp r1, r0
	bgt _0800B2C4
	cmp r1, #0x52
	beq _0800B2D4
	bl _0800BDDC @ far jump
_0800B2B0: .4byte 0x00000D64
_0800B2B4: .4byte 0x0201930C
_0800B2B8: .4byte 0x000007FF
_0800B2BC: .4byte gCardIdToNumber
_0800B2C0: .4byte 0x0000044B
_0800B2C4:
	ldr r0, _0800B2D0 @ =0x000004DC
	cmp r1, r0
	beq _0800B308
	bl _0800BDDC @ far jump
	.align 2, 0
_0800B2D0: .4byte 0x000004DC
_0800B2D4:
	ldr r1, [sp, #0]
	and r1, r6
	ldr r5, [sp, #4]
	add r0, r5, #0
	mul r0, r4
	mul r1, r2
	add r0, r0, r1
	ldr r6, _0800B304 @ =0x0201930C
	add r0, r0, r6
	add r0, #0x4A
	add r0, r0, r3
	ldrh r0, [r0]
	lsr r0, r0, #8
	add r0, #1
	mov r1, #0xC8
	mul r0, r1
	ldr r7, [sp, #0x2C]
	add r7, r7, r0
	str r7, [sp, #0x2C]
	ldr r1, [sp, #0x30]
	add r1, r1, r0
	str r1, [sp, #0x30]
	bl _0800BDDC @ far jump
_0800B304: .4byte 0x0201930C
_0800B308:
	ldr r2, [sp, #0x2C]
	ldr r3, _0800B314 @ =0xFFFFFD44
	add r2, r2, r3
	str r2, [sp, #0x2C]
	bl _0800BDDC @ far jump
_0800B314: .4byte 0xFFFFFD44
_0800B318:
	ldr r4, [sp, #0]
	cmp r5, r4
	bne _0800B322
	bl _0800BDDC @ far jump
_0800B322:
	ldr r5, [sp, #0xC]
	add r5, #1
	str r5, [sp, #0xC]
	bl _0800BDDC @ far jump
_0800B32C:
	ldr r6, [sp, #0x40]
	cmp r6, #0
	bne _0800B336
	bl _0800BDDC @ far jump
_0800B336:
	add r1, r5, #0
	mov r7, #1
	and r1, r7
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	ldr r4, _0800B410 @ =0x00000D64
	add r3, r1, #0
	mul r3, r4
	str r3, [sp, #0x44]
	add r0, r0, r3
	ldr r7, _0800B414 @ =0x0201930C
	add r6, r0, r7
	add r1, r6, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800B364
	bl _0800BBAA @ far jump
_0800B364:
	ldr r4, _0800B418 @ =0x00000601
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0800B376
	bl _0800BBAA @ far jump
_0800B376:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0800B386
	bl _0800BBAA @ far jump
_0800B386:
	ldr r0, _0800B41C @ =0x00001AA1
	add r1, r7, r0
	mov r0, #3
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800B398
	bl _0800BBAA @ far jump
_0800B398:
	ldr r1, [sp, #0x38]
	cmp r1, #0
	beq _0800B3BC
	ldr r0, _0800B420 @ =0x000007FF
	ldr r2, [sp, #0x40]
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _0800B424 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0800B3BC
	bl _0800BBAA @ far jump
_0800B3BC:
	ldr r3, _0800B420 @ =0x000007FF
	ldr r0, [sp, #0x40]
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _0800B428 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #0xA2
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B3D4
	b _0800B7CC
_0800B3D4:
	cmp r1, r0
	ble _0800B3DA
	b _0800B4F0
_0800B3DA:
	sub r0, #0xE
	cmp r1, r0
	bne _0800B3E2
	b _0800B6F4
_0800B3E2:
	cmp r1, r0
	bgt _0800B478
	sub r0, #6
	cmp r1, r0
	bne _0800B3EE
	b _0800B688
_0800B3EE:
	cmp r1, r0
	bgt _0800B440
	sub r0, #3
	cmp r1, r0
	bne _0800B3FA
	b _0800B65C
_0800B3FA:
	cmp r1, r0
	bgt _0800B42C
	cmp r1, #0x47
	bne _0800B404
	b _0800B620
_0800B404:
	sub r0, #1
	cmp r1, r0
	bne _0800B40C
	b _0800B648
_0800B40C:
	b _0800BBAA
	.align 2, 0
_0800B410: .4byte 0x00000D64
_0800B414: .4byte 0x0201930C
_0800B418: .4byte 0x00000601
_0800B41C: .4byte 0x00001AA1
_0800B420: .4byte 0x000007FF
_0800B424: .4byte gCardStats
_0800B428: .4byte gCardIdToNumber
_0800B42C:
	mov r0, #0x97
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B436
	b _0800B66C
_0800B436:
	add r0, #1
	cmp r1, r0
	bne _0800B43E
	b _0800B680
_0800B43E:
	b _0800BBAA
_0800B440:
	ldr r0, _0800B460 @ =0x00000133
	cmp r1, r0
	bne _0800B448
	b _0800B6B8
_0800B448:
	cmp r1, r0
	bgt _0800B464
	sub r0, #2
	cmp r1, r0
	bne _0800B454
	b _0800B698
_0800B454:
	add r0, #1
	cmp r1, r0
	bne _0800B45C
	b _0800B6A8
_0800B45C:
	b _0800BBAA
	.align 2, 0
_0800B460: .4byte 0x00000133
_0800B464:
	mov r0, #0x9A
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B46E
	b _0800B6C8
_0800B46E:
	add r0, #1
	cmp r1, r0
	bne _0800B476
	b _0800B6E4
_0800B476:
	b _0800BBAA
_0800B478:
	mov r0, #0x9E
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B482
	b _0800B74A
_0800B482:
	cmp r1, r0
	bgt _0800B4B8
	sub r0, #3
	cmp r1, r0
	bne _0800B48E
	b _0800B718
_0800B48E:
	cmp r1, r0
	bgt _0800B4A4
	sub r0, #2
	cmp r1, r0
	bne _0800B49A
	b _0800B6FC
_0800B49A:
	add r0, #1
	cmp r1, r0
	bne _0800B4A2
	b _0800B70C
_0800B4A2:
	b _0800BBAA
_0800B4A4:
	mov r0, #0x9D
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B4AE
	b _0800B726
_0800B4AE:
	add r0, #1
	cmp r1, r0
	bne _0800B4B6
	b _0800B73A
_0800B4B6:
	b _0800BBAA
_0800B4B8:
	ldr r0, _0800B4D8 @ =0x00000141
	cmp r1, r0
	bne _0800B4C0
	b _0800B790
_0800B4C0:
	cmp r1, r0
	bgt _0800B4DC
	sub r0, #3
	cmp r1, r0
	bne _0800B4CC
	b _0800B778
_0800B4CC:
	add r0, #2
	cmp r1, r0
	bne _0800B4D4
	b _0800B788
_0800B4D4:
	b _0800BBAA
	.align 2, 0
_0800B4D8: .4byte 0x00000141
_0800B4DC:
	mov r0, #0xA1
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800B4E6
	b _0800B7A0
_0800B4E6:
	add r0, #1
	cmp r1, r0
	bne _0800B4EE
	b _0800B7B0
_0800B4EE:
	b _0800BBAA
_0800B4F0:
	ldr r0, _0800B528 @ =0x00000412
	cmp r1, r0
	bne _0800B4F8
	b _0800B9DA
_0800B4F8:
	cmp r1, r0
	bgt _0800B584
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	bne _0800B506
	b _0800B84E
_0800B506:
	cmp r1, r0
	bgt _0800B548
	ldr r0, _0800B52C @ =0x00000147
	cmp r1, r0
	bne _0800B512
	b _0800B820
_0800B512:
	cmp r1, r0
	bgt _0800B530
	sub r0, #2
	cmp r1, r0
	bne _0800B51E
	b _0800B7E8
_0800B51E:
	add r0, #1
	cmp r1, r0
	bne _0800B526
	b _0800B804
_0800B526:
	b _0800BBAA
_0800B528: .4byte 0x00000412
_0800B52C: .4byte 0x00000147
_0800B530:
	ldr r0, _0800B544 @ =0x0000028A
	cmp r1, r0
	bne _0800B538
	b _0800B830
_0800B538:
	add r0, #3
	cmp r1, r0
	bne _0800B540
	b _0800B838
_0800B540:
	b _0800BBAA
	.align 2, 0
_0800B544: .4byte 0x0000028A
_0800B548:
	ldr r0, _0800B568 @ =0x000003C2
	cmp r1, r0
	bne _0800B550
	b _0800B974
_0800B550:
	cmp r1, r0
	bgt _0800B570
	ldr r0, _0800B56C @ =0x00000291
	cmp r1, r0
	bne _0800B55C
	b _0800B950
_0800B55C:
	add r0, #0xA
	cmp r1, r0
	bne _0800B564
	b _0800B95E
_0800B564:
	b _0800BBAA
	.align 2, 0
_0800B568: .4byte 0x000003C2
_0800B56C: .4byte 0x00000291
_0800B570:
	mov r0, #0xFD
	lsl r0, r0, #2
	cmp r1, r0
	bne _0800B57A
	b _0800B9A2
_0800B57A:
	add r0, #1
	cmp r1, r0
	bne _0800B582
	b _0800B9BE
_0800B582:
	b _0800BBAA
_0800B584:
	ldr r0, _0800B5B0 @ =0x0000058C
	cmp r1, r0
	bne _0800B58C
	b _0800BA70
_0800B58C:
	cmp r1, r0
	bgt _0800B5D0
	ldr r0, _0800B5B4 @ =0x00000424
	cmp r1, r0
	bne _0800B598
	b _0800BA38
_0800B598:
	cmp r1, r0
	bgt _0800B5B8
	sub r0, #0xE
	cmp r1, r0
	bne _0800B5A4
	b _0800B9E8
_0800B5A4:
	add r0, #0xC
	cmp r1, r0
	bne _0800B5AC
	b _0800BA28
_0800B5AC:
	b _0800BBAA
	.align 2, 0
_0800B5B0: .4byte 0x0000058C
_0800B5B4: .4byte 0x00000424
_0800B5B8:
	ldr r0, _0800B5CC @ =0x0000049E
	cmp r1, r0
	bne _0800B5C0
	b _0800BA4E
_0800B5C0:
	add r0, #0x3C
	cmp r1, r0
	bne _0800B5C8
	b _0800BA64
_0800B5C8:
	b _0800BBAA
	.align 2, 0
_0800B5CC: .4byte 0x0000049E
_0800B5D0:
	ldr r0, _0800B5F0 @ =0x000005A9
	cmp r1, r0
	bne _0800B5D8
	b _0800BAD0
_0800B5D8:
	cmp r1, r0
	bgt _0800B5F4
	sub r0, #0x1B
	cmp r1, r0
	bne _0800B5E4
	b _0800BA80
_0800B5E4:
	add r0, #0x1A
	cmp r1, r0
	bne _0800B5EC
	b _0800BA96
_0800B5EC:
	b _0800BBAA
	.align 2, 0
_0800B5F0: .4byte 0x000005A9
_0800B5F4:
	ldr r0, _0800B60C @ =0x00000604
	cmp r1, r0
	bne _0800B5FC
	b _0800BB38
_0800B5FC:
	cmp r1, r0
	bgt _0800B610
	sub r0, #0x5A
	cmp r1, r0
	bne _0800B608
	b _0800BB02
_0800B608:
	b _0800BBAA
	.align 2, 0
_0800B60C: .4byte 0x00000604
_0800B610:
	ldr r0, _0800B61C @ =0x0000060E
	cmp r1, r0
	bne _0800B618
	b _0800BB58
_0800B618:
	b _0800BBAA
	.align 2, 0
_0800B61C: .4byte 0x0000060E
_0800B620:
	add r0, r3, #0
	mov r4, r9
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	add r0, r0, r2
	ldr r1, _0800B644 @ =0x00000115
	ldrh r0, [r0]
	cmp r0, r1
	beq _0800B636
	b _0800BBAA
_0800B636:
	mov r0, #0
	mov r5, r9
	str r0, [r5, #4]
	mov r0, #0xFA
	lsl r0, r0, #3
	str r0, [r5, #8]
	b _0800BBAA
_0800B644: .4byte 0x00000115
_0800B648:
	mov r0, #0x1F
	mov r6, r9
	ldrb r6, [r6, #2]
	and r0, r6
	cmp r0, #0xF
	beq _0800B656
	b _0800BBAA
_0800B656:
	mov r0, #0x96
	lsl r0, r0, #1
	b _0800B9DE
_0800B65C:
	mov r0, #0xE0
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0x40
	beq _0800B66A
	b _0800BBAA
_0800B66A:
	b _0800B9B0
_0800B66C:
	mov r0, #0x1F
	mov r4, r9
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #3
	beq _0800B67A
	b _0800BBAA
_0800B67A:
	mov r0, #0x96
	lsl r0, r0, #1
	b _0800B71C
_0800B680:
	mov r6, #0xFA
	lsl r6, r6, #2
	add sl, r6
	b _0800BBAA
_0800B688:
	mov r0, #0x1F
	mov r7, r9
	ldrb r7, [r7, #2]
	and r0, r7
	cmp r0, #0xA
	beq _0800B696
	b _0800BBAA
_0800B696:
	b _0800B950
_0800B698:
	mov r0, #0x1F
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	cmp r0, #0xA
	beq _0800B6A6
	b _0800BBAA
_0800B6A6:
	b _0800B96C
_0800B6A8:
	mov r0, #0xE0
	mov r4, r9
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0x20
	beq _0800B6B6
	b _0800BBAA
_0800B6B6:
	b _0800B9CC
_0800B6B8:
	mov r0, #0x1F
	mov r7, r9
	ldrb r7, [r7, #2]
	and r0, r7
	cmp r0, #0xB
	beq _0800B6C6
	b _0800BBAA
_0800B6C6:
	b _0800B950
_0800B6C8:
	mov r0, #0xE0
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	cmp r0, #0x60
	beq _0800B6D6
	b _0800BBAA
_0800B6D6:
	mov r3, #0xC8
	lsl r3, r3, #1
	add sl, r3
	ldr r4, [sp, #0x1C]
	sub r4, #0xC8
	str r4, [sp, #0x1C]
	b _0800BBAA
_0800B6E4:
	mov r0, #0x1F
	mov r5, r9
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0xD
	beq _0800B6F2
	b _0800BBAA
_0800B6F2:
	b _0800B812
_0800B6F4:
	mov r7, #0xFA
	lsl r7, r7, #1
	add sl, r7
	b _0800BBAA
_0800B6FC:
	mov r0, #0x1F
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0x11
	beq _0800B70A
	b _0800BBAA
_0800B70A:
	b _0800B7DA
_0800B70C:
	ldr r3, [sp, #0x1C]
	mov r4, #0xC8
	lsl r4, r4, #2
	add r3, r3, r4
	str r3, [sp, #0x1C]
	b _0800BBAA
_0800B718:
	mov r0, #0xAF
	lsl r0, r0, #2
_0800B71C:
	add sl, r0
	ldr r5, [sp, #0x1C]
	add r5, r5, r0
	str r5, [sp, #0x1C]
	b _0800BBAA
_0800B726:
	mov r0, #0x1F
	mov r6, r9
	ldrb r6, [r6, #2]
	and r0, r6
	cmp r0, #1
	beq _0800B734
	b _0800BBAA
_0800B734:
	mov r0, #0x96
	lsl r0, r0, #1
	b _0800B9DE
_0800B73A:
	mov r0, #0x1F
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0x13
	beq _0800B748
	b _0800BBAA
_0800B748:
	b _0800B7DA
_0800B74A:
	add r0, r3, #0
	mov r3, r9
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r1, [r0]
	add r0, r1, #0
	sub r0, #0x3D
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bls _0800B76C
	ldr r0, _0800B774 @ =0x000004E1
	cmp r1, r0
	beq _0800B76C
	b _0800BBAA
_0800B76C:
	mov r4, #0xFA
	lsl r4, r4, #1
	add sl, r4
	b _0800BBAA
_0800B774: .4byte 0x000004E1
_0800B778:
	mov r0, #0x1F
	mov r5, r9
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0xC
	beq _0800B786
	b _0800BBAA
_0800B786:
	b _0800B812
_0800B788:
	mov r7, #0xAF
	lsl r7, r7, #2
	add sl, r7
	b _0800BBAA
_0800B790:
	mov r0, #0x1F
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #2
	beq _0800B79E
	b _0800BBAA
_0800B79E:
	b _0800B7DA
_0800B7A0:
	mov r0, #0x1F
	mov r3, r9
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #0x12
	beq _0800B7AE
	b _0800BBAA
_0800B7AE:
	b _0800B7F6
_0800B7B0:
	mov r0, #0xE0
	mov r5, r9
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0xA0
	beq _0800B7BE
	b _0800BBAA
_0800B7BE:
	mov r6, #0xC8
	lsl r6, r6, #1
	add sl, r6
	ldr r7, [sp, #0x1C]
	sub r7, #0xC8
	str r7, [sp, #0x1C]
	b _0800BBAA
_0800B7CC:
	mov r0, #0x1F
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #7
	beq _0800B7DA
	b _0800BBAA
_0800B7DA:
	mov r0, #0x96
	lsl r0, r0, #1
	add sl, r0
	ldr r2, [sp, #0x1C]
	add r2, r2, r0
	str r2, [sp, #0x1C]
	b _0800BBAA
_0800B7E8:
	mov r0, #0x1F
	mov r3, r9
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #9
	beq _0800B7F6
	b _0800BBAA
_0800B7F6:
	mov r0, #0x96
	lsl r0, r0, #1
	add sl, r0
	ldr r4, [sp, #0x1C]
	add r4, r4, r0
	str r4, [sp, #0x1C]
	b _0800BBAA
_0800B804:
	mov r0, #0x1F
	mov r5, r9
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0x10
	beq _0800B812
	b _0800BBAA
_0800B812:
	mov r0, #0x96
	lsl r0, r0, #1
	add sl, r0
	ldr r6, [sp, #0x1C]
	add r6, r6, r0
	str r6, [sp, #0x1C]
	b _0800BBAA
_0800B820:
	mov r0, #0x1F
	mov r7, r9
	ldrb r7, [r7, #2]
	and r0, r7
	cmp r0, #0xE
	beq _0800B82E
	b _0800BBAA
_0800B82E:
	b _0800B950
_0800B830:
	mov r2, #0xFA
	lsl r2, r2, #1
	add sl, r2
	b _0800BBAA
_0800B838:
	mov r0, #0xE0
	mov r3, r9
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #0x80
	beq _0800B846
	b _0800BBAA
_0800B846:
	mov r4, #0xAF
	lsl r4, r4, #2
	add sl, r4
	b _0800BBAA
_0800B84E:
	add r1, r7, #0
	sub r1, #0x28
	ldr r6, [sp, #0x44]
	add r2, r6, r1
	mov r7, #1
	sub r0, r7, r5
	and r0, r7
	ldr r4, _0800B890 @ =0x00000D64
	mul r0, r4
	add r0, r0, r1
	ldrh r2, [r2]
	ldrh r0, [r0]
	cmp r2, r0
	bcs _0800B8BE
	mov r6, r9
	ldrh r2, [r6]
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _0800B894 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800B8A2
	cmp r0, #0x17
	ble _0800B898
	cmp r0, #0x18
	beq _0800B89C
	b _0800B8A2
_0800B890: .4byte 0x00000D64
_0800B894: .4byte gCardStats
_0800B898:
	mov r0, #0
	b _0800B8B8
_0800B89C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800B8B8
_0800B8A2:
	ldr r0, _0800B904 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0800B908 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800B8B8:
	lsl r0, r0, #1
	mov r2, r9
	str r0, [r2, #4]
_0800B8BE:
	ldr r4, _0800B90C @ =0x020192E4
	mov r2, #1
	add r0, r5, #0
	and r0, r2
	ldr r3, _0800B910 @ =0x00000D64
	add r1, r0, #0
	mul r1, r3
	add r1, r1, r4
	sub r0, r2, r5
	and r0, r2
	mul r0, r3
	add r0, r0, r4
	ldrh r1, [r1]
	ldrh r0, [r0]
	cmp r1, r0
	bls _0800B93C
	mov r3, r9
	ldrh r2, [r3]
	ldr r0, _0800B904 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r4, _0800B908 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800B91E
	cmp r0, #0x17
	ble _0800B914
	cmp r0, #0x18
	beq _0800B918
	b _0800B91E
_0800B904: .4byte 0x000007FF
_0800B908: .4byte gCardStats
_0800B90C: .4byte 0x020192E4
_0800B910: .4byte 0x00000D64
_0800B914:
	mov r0, #0
	b _0800B934
_0800B918:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800B934
_0800B91E:
	ldr r0, _0800B948 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r5, _0800B94C @ =0x08621DE0
	add r0, r0, r5
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800B934:
	bl HalveRoundUp
	mov r6, r9
	str r0, [r6, #4]
_0800B93C:
	mov r7, #0
	str r7, [sp, #0x2C]
	mov r0, #0
	str r0, [sp, #0x24]
	b _0800BBAA
	.align 2, 0
_0800B948: .4byte 0x000007FF
_0800B94C: .4byte gCardStats
_0800B950:
	mov r0, #0x96
	lsl r0, r0, #1
	add sl, r0
	ldr r1, [sp, #0x1C]
	add r1, r1, r0
	str r1, [sp, #0x1C]
	b _0800BBAA
_0800B95E:
	mov r0, #0xE0
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	cmp r0, #0x20
	beq _0800B96C
	b _0800BBAA
_0800B96C:
	mov r3, #0xAF
	lsl r3, r3, #2
	add sl, r3
	b _0800BBAA
_0800B974:
	mov r0, #0x1F
	mov r4, r9
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #7
	beq _0800B982
	b _0800BBAA
_0800B982:
	add r0, r6, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1B
	cmp r0, #1
	beq _0800BA5C
	cmp r0, #2
	beq _0800B996
	b _0800BBAA
_0800B996:
	ldr r6, [sp, #0x1C]
	mov r7, #0xAF
	lsl r7, r7, #2
	add r6, r6, r7
	str r6, [sp, #0x1C]
	b _0800BBAA
_0800B9A2:
	mov r0, #0xE0
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0x80
	beq _0800B9B0
	b _0800BBAA
_0800B9B0:
	mov r2, #0xC8
	lsl r2, r2, #1
	add sl, r2
	ldr r3, [sp, #0x1C]
	sub r3, #0xC8
	str r3, [sp, #0x1C]
	b _0800BBAA
_0800B9BE:
	mov r0, #0xE0
	mov r4, r9
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0xC0
	beq _0800B9CC
	b _0800BBAA
_0800B9CC:
	mov r5, #0xC8
	lsl r5, r5, #1
	add sl, r5
	ldr r6, [sp, #0x1C]
	sub r6, #0xC8
	str r6, [sp, #0x1C]
	b _0800BBAA
_0800B9DA:
	mov r0, #0xFA
	lsl r0, r0, #1
_0800B9DE:
	add sl, r0
	ldr r7, [sp, #0x1C]
	add r7, r7, r0
	str r7, [sp, #0x1C]
	b _0800BBAA
_0800B9E8:
	mov r0, #0x1F
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #7
	bne _0800B9F6
	b _0800BBAA
_0800B9F6:
	mov r0, #1
	and r5, r0
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _0800BA20 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _0800BA24 @ =0x0201930C
	add r1, r1, r0
	ldrb r1, [r1, #6]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	mov r2, sl
	b _0800BA48
	.align 2, 0
_0800BA20: .4byte 0x00000D64
_0800BA24: .4byte 0x0201930C
_0800BA28:
	ldr r0, _0800BA34 @ =0xFFFFFE0C
	add sl, r0
	ldr r3, [sp, #0x1C]
	add r3, r3, r0
	str r3, [sp, #0x1C]
	b _0800BBAA
_0800BA34: .4byte 0xFFFFFE0C
_0800BA38:
	mov r2, #0xAF
	lsl r2, r2, #2
	add r2, sl
	ldrb r6, [r6, #6]
	lsl r0, r6, #0x1A
	lsr r0, r0, #0x1C
	mov r1, #0xC8
	mul r0, r1
_0800BA48:
	sub r2, r2, r0
	mov sl, r2
	b _0800BBAA
_0800BA4E:
	mov r0, #0x1F
	mov r4, r9
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0xF
	beq _0800BA5C
	b _0800BBAA
_0800BA5C:
	mov r5, #0xAF
	lsl r5, r5, #2
	add sl, r5
	b _0800BBAA
_0800BA64:
	ldr r6, _0800BA6C @ =0xFFFFFE0C
	add sl, r6
	b _0800BBAA
	.align 2, 0
_0800BA6C: .4byte 0xFFFFFE0C
_0800BA70:
	mov r7, #0xFA
	lsl r7, r7, #2
	add sl, r7
	ldr r0, [sp, #0x1C]
	ldr r1, _0800BA7C @ =0xFFFFFC18
	b _0800BB32
_0800BA7C: .4byte 0xFFFFFC18
_0800BA80:
	mov r0, #0x1F
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	cmp r0, #0xF
	beq _0800BA8E
	b _0800BBAA
_0800BA8E:
	mov r3, #0xC8
	lsl r3, r3, #2
	add sl, r3
	b _0800BBAA
_0800BA96:
	mov r0, #1
	and r5, r0
	mov r0, #0x94
	mov r4, r8
	mul r4, r0
	add r0, r4, #0
	ldr r1, _0800BAC8 @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	ldr r1, _0800BACC @ =0x0201930C
	add r0, r0, r1
	add r0, #0x90
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x1B
	lsl r1, r1, #5
	mov r0, #0x1F
	mov r5, r9
	ldrb r5, [r5, #2]
	and r0, r5
	orr r0, r1
	mov r6, r9
	strb r0, [r6, #2]
	b _0800BBAA
	.align 2, 0
_0800BAC8: .4byte 0x00000D64
_0800BACC: .4byte 0x0201930C
_0800BAD0:
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #5
	add sl, r1
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #5
	ldr r7, [sp, #0x1C]
	add r7, r7, r1
	str r7, [sp, #0x1C]
	b _0800BBAA
_0800BB02:
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl CountSpellTrapsFiltered
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add sl, r1
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl CountSpellTrapsFiltered
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r0, [sp, #0x1C]
_0800BB32:
	add r0, r0, r1
	str r0, [sp, #0x1C]
	b _0800BBAA
_0800BB38:
	add r0, r3, #0
	mov r1, r9
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	ldr r1, _0800BB54 @ =0x0000053B
	ldrh r0, [r0]
	cmp r0, r1
	bne _0800BBAA
	mov r2, #0x96
	lsl r2, r2, #1
	add sl, r2
	b _0800BBAA
_0800BB54: .4byte 0x0000053B
_0800BB58:
	mov r3, r9
	ldrb r1, [r3, #2]
	mov r0, #0x1F
	and r0, r1
	cmp r0, #0xF
	bne _0800BB7C
	mov r0, #0x20
	neg r0, r0
	and r0, r1
	mov r1, #1
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, #0xFA
	lsl r0, r0, #1
	add sl, r0
	ldr r4, [sp, #0x1C]
	add r4, r4, r0
	str r4, [sp, #0x1C]
_0800BB7C:
	mov r0, #1
	and r5, r0
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _0800BBBC @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _0800BBC0 @ =0x0201930C
	add r1, r1, r0
	ldrh r1, [r1, #4]
	ldr r5, [sp, #0x34]
	cmp r1, r5
	bls _0800BBAA
	mov r0, #0x20
	neg r0, r0
	mov r6, r9
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #1
	orr r0, r1
	mov r7, r9
	strb r0, [r7, #2]
_0800BBAA:
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0800BBB2
	b _0800BDDC
_0800BBB2:
	ldr r1, [sp, #0x20]
	add r1, #1
	str r1, [sp, #0x20]
	b _0800BDDC
	.align 2, 0
_0800BBBC: .4byte 0x00000D64
_0800BBC0: .4byte 0x0201930C
_0800BBC4:
	mov r0, #0x64
	mul r0, r6
	ldr r2, [sp, #0x2C]
	add r2, r2, r0
	str r2, [sp, #0x2C]
	ldr r3, [sp, #0x30]
	add r3, r3, r0
	str r3, [sp, #0x30]
	b _0800BDDC
_0800BBD6:
	ldr r2, _0800BC40 @ =0x000007FF
	add r0, r2, #0
	mov r4, r9
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r6, _0800BC44 @ =0x08622AB4
	add r0, r0, r6
	ldrh r1, [r0]
	ldr r0, _0800BC48 @ =0x000002DA
	cmp r1, r0
	beq _0800BBF6
	ldr r0, _0800BC4C @ =0x00000536
	cmp r1, r0
	beq _0800BBF6
	b _0800BDDC
_0800BBF6:
	mov r0, #0
	mov r7, r9
	str r0, [r7, #4]
	str r0, [r7, #8]
	mov r0, #1
	and r5, r0
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _0800BC50 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _0800BC54 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0800BC1E
	b _0800BDDC
_0800BC1E:
	ldr r0, [sp, #0x40]
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0800BC58 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800BC66
	cmp r0, #0x17
	ble _0800BC5C
	cmp r0, #0x18
	beq _0800BC60
	b _0800BC66
_0800BC40: .4byte 0x000007FF
_0800BC44: .4byte gCardIdToNumber
_0800BC48: .4byte 0x000002DA
_0800BC4C: .4byte 0x00000536
_0800BC50: .4byte 0x00000D64
_0800BC54: .4byte 0x0201930C
_0800BC58: .4byte gCardStats
_0800BC5C:
	mov r0, #0
	b _0800BC7E
_0800BC60:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800BC7E
_0800BC66:
	ldr r0, _0800BCA8 @ =0x000007FF
	ldr r2, [sp, #0x40]
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _0800BCAC @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800BC7E:
	mov r4, r9
	str r0, [r4, #4]
	ldr r0, _0800BCA8 @ =0x000007FF
	ldr r5, [sp, #0x40]
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _0800BCAC @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800BCBA
	cmp r0, #0x17
	ble _0800BCB0
	cmp r0, #0x18
	beq _0800BCB4
	b _0800BCBA
	.align 2, 0
_0800BCA8: .4byte 0x000007FF
_0800BCAC: .4byte gCardStats
_0800BCB0:
	mov r0, #0
	b _0800BCD2
_0800BCB4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800BCD2
_0800BCBA:
	ldr r0, _0800BCD8 @ =0x000007FF
	ldr r7, [sp, #0x40]
	and r7, r0
	lsl r0, r7, #2
	ldr r1, _0800BCDC @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _0800BCE0 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800BCD2:
	mov r2, r9
	str r0, [r2, #8]
	b _0800BDDC
_0800BCD8: .4byte 0x000007FF
_0800BCDC: .4byte gCardStats
_0800BCE0: .4byte 0x000001FF
_0800BCE4:
	ldr r3, [sp, #0x38]
	cmp r3, #0
	bne _0800BDDC
	mov r4, r9
	ldr r0, [r4, #4]
	add r0, r0, r7
	str r0, [r4, #4]
	b _0800BDDC
_0800BCF4:
	ldr r0, _0800BD18 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r5, _0800BD1C @ =0x08621DE0
	add r0, r0, r5
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800BD2A
	cmp r0, #0x17
	ble _0800BD20
	cmp r0, #0x18
	beq _0800BD24
	b _0800BD2A
	.align 2, 0
_0800BD18: .4byte 0x000007FF
_0800BD1C: .4byte gCardStats
_0800BD20:
	mov r0, #0
	b _0800BD40
_0800BD24:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800BD40
_0800BD2A:
	ldr r0, _0800BD68 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r6, _0800BD6C @ =0x08621DE0
	add r0, r0, r6
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800BD40:
	ldr r1, [sp, #0x2C]
	add r1, r0, r1
	str r1, [sp, #0x2C]
	ldr r0, _0800BD68 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _0800BD6C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800BD7A
	cmp r0, #0x17
	ble _0800BD70
	cmp r0, #0x18
	beq _0800BD74
	b _0800BD7A
_0800BD68: .4byte 0x000007FF
_0800BD6C: .4byte gCardStats
_0800BD70:
	mov r0, #0
	b _0800BD90
_0800BD74:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800BD90
_0800BD7A:
	ldr r0, _0800BD98 @ =0x000007FF
	and r7, r0
	lsl r0, r7, #2
	ldr r3, _0800BD9C @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	ldr r0, _0800BDA0 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800BD90:
	ldr r4, [sp, #0x30]
	add r4, r0, r4
	str r4, [sp, #0x30]
	b _0800BDDC
_0800BD98: .4byte 0x000007FF
_0800BD9C: .4byte gCardStats
_0800BDA0: .4byte 0x000001FF
_0800BDA4:
	add r1, r6, #1
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r5, [sp, #0x2C]
	sub r5, r5, r0
	str r5, [sp, #0x2C]
	ldr r6, [sp, #0x30]
	sub r6, r6, r0
	str r6, [sp, #0x30]
	b _0800BDDC
_0800BDBE:
	mov r7, #0xC8
	add sl, r7
	b _0800BDDC
_0800BDC4:
	lsl r1, r6, #2
	add r1, r1, r6
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	add sl, r0
	b _0800BDDC
_0800BDD2:
	mov r0, #0xC8
	mul r0, r6
	ldr r1, [sp, #0x2C]
	sub r1, r1, r0
	str r1, [sp, #0x2C]
_0800BDDC:
	ldr r2, [sp, #8]
	add r2, #1
	str r2, [sp, #8]
	mov r1, #1
	ldr r3, [sp, #0]
	and r1, r3
	mov r0, #0x94
	ldr r4, [sp, #4]
	mul r0, r4
	ldr r5, _0800BE40 @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	ldr r6, _0800BE44 @ =0x0201930C
	add r0, r0, r6
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r2, r0
	bge _0800BE04
	bl _0800AEFE @ far jump
_0800BE04:
	ldr r4, _0800BE48 @ =0x000007FF
	add r0, r4, #0
	mov r7, r9
	ldrh r7, [r7]
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0800BE4C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0800BE50 @ =0x00000267
	cmp r1, r0
	bne _0800BE1E
	b _0800C04E
_0800BE1E:
	cmp r1, r0
	bgt _0800BE90
	sub r0, #0xE6
	cmp r1, r0
	bne _0800BE2A
	b _0800BF98
_0800BE2A:
	cmp r1, r0
	bgt _0800BE60
	cmp r1, #0xEA
	bne _0800BE34
	b _0800BF80
_0800BE34:
	cmp r1, #0xEA
	bgt _0800BE54
	cmp r1, #0xB
	beq _0800BF00
	b _0800C27C
	.align 2, 0
_0800BE40: .4byte 0x00000D64
_0800BE44: .4byte 0x0201930C
_0800BE48: .4byte 0x000007FF
_0800BE4C: .4byte gCardIdToNumber
_0800BE50: .4byte 0x00000267
_0800BE54:
	ldr r0, _0800BE5C @ =0x0000016F
	cmp r1, r0
	beq _0800BF20
	b _0800C27C
_0800BE5C: .4byte 0x0000016F
_0800BE60:
	mov r0, #0xF6
	lsl r0, r0, #1
	cmp r1, r0
	bne _0800BE6A
	b _0800BFF0
_0800BE6A:
	cmp r1, r0
	bgt _0800BE78
	sub r0, #0x56
	cmp r1, r0
	bne _0800BE76
	b _0800BFD0
_0800BE76:
	b _0800C27C
_0800BE78:
	ldr r0, _0800BE8C @ =0x00000203
	cmp r1, r0
	bne _0800BE80
	b _0800C004
_0800BE80:
	add r0, #0x26
	cmp r1, r0
	bne _0800BE88
	b _0800C034
_0800BE88:
	b _0800C27C
	.align 2, 0
_0800BE8C: .4byte 0x00000203
_0800BE90:
	ldr r0, _0800BEB4 @ =0x00000457
	cmp r1, r0
	bne _0800BE98
	b _0800C1C8
_0800BE98:
	cmp r1, r0
	bgt _0800BEC8
	ldr r0, _0800BEB8 @ =0x000002F9
	cmp r1, r0
	bne _0800BEA4
	b _0800C128
_0800BEA4:
	cmp r1, r0
	bgt _0800BEBC
	sub r0, #2
	cmp r1, r0
	bne _0800BEB0
	b _0800C092
_0800BEB0:
	b _0800C27C
	.align 2, 0
_0800BEB4: .4byte 0x00000457
_0800BEB8: .4byte 0x000002F9
_0800BEBC:
	mov r0, #0xCA
	lsl r0, r0, #2
	cmp r1, r0
	bne _0800BEC6
	b _0800C14C
_0800BEC6:
	b _0800C27C
_0800BEC8:
	ldr r0, _0800BEE0 @ =0x00000585
	cmp r1, r0
	bne _0800BED0
	b _0800C214
_0800BED0:
	cmp r1, r0
	bgt _0800BEE8
	ldr r0, _0800BEE4 @ =0x0000045E
	cmp r1, r0
	bne _0800BEDC
	b _0800C1F8
_0800BEDC:
	b _0800C27C
	.align 2, 0
_0800BEE0: .4byte 0x00000585
_0800BEE4: .4byte 0x0000045E
_0800BEE8:
	ldr r0, _0800BEFC @ =0x000005EC
	cmp r1, r0
	bne _0800BEF0
	b _0800C228
_0800BEF0:
	add r0, #2
	cmp r1, r0
	bne _0800BEF8
	b _0800C258
_0800BEF8:
	b _0800C27C
	.align 2, 0
_0800BEFC: .4byte 0x000005EC
_0800BF00:
	ldr r1, _0800BF1C @ =0x00000229
	ldr r0, [sp, #0]
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r2, [sp, #0x2C]
	add r2, r2, r1
	str r2, [sp, #0x2C]
	b _0800C27C
	.align 2, 0
_0800BF1C: .4byte 0x00000229
_0800BF20:
	ldr r2, _0800BF70 @ =0x020192E4
	mov r0, #1
	ldr r3, [sp, #0]
	and r0, r3
	ldr r1, _0800BF74 @ =0x00000D64
	mul r1, r0
	add r0, r1, r2
	ldrb r3, [r0, #4]
	cmp r3, #0
	bne _0800BF36
	b _0800C27C
_0800BF36:
	ldr r5, _0800BF78 @ =0x00000904
	add r0, r2, r5
	add r1, r1, r0
	mov r2, #0xF8
	lsl r2, r2, #0x11
	str r3, [sp, #8]
_0800BF42:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r6, _0800BF7C @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0800BF60
	ldr r7, [sp, #0x2C]
	add r7, #0x64
	str r7, [sp, #0x2C]
_0800BF60:
	add r1, #4
	ldr r0, [sp, #8]
	sub r0, #1
	str r0, [sp, #8]
	cmp r0, #0
	bne _0800BF42
	b _0800C27C
	.align 2, 0
_0800BF70: .4byte 0x020192E4
_0800BF74: .4byte 0x00000D64
_0800BF78: .4byte 0x00000904
_0800BF7C: .4byte gCardStats
_0800BF80:
	mov r0, #0
	mov r1, #0xD
	bl CountFaceUpMonstersOfType
	mov r4, #0x64
	mul r0, r4
	ldr r1, [sp, #0x2C]
	add r1, r1, r0
	str r1, [sp, #0x2C]
	mov r0, #1
	mov r1, #0xD
	b _0800C13E
_0800BF98:
	mov r0, #0
	mov r1, #0x3D
	bl CountActiveCardsOnField
	add r4, r0, #0
	mov r0, #1
	mov r1, #0x3D
	bl CountActiveCardsOnField
	mov r8, r0
	ldr r7, _0800BFCC @ =0x000004E1
	mov r0, #0
	add r1, r7, #0
	bl CountActiveCardsOnField
	add r6, r0, #0
	mov r0, #1
	add r1, r7, #0
	bl CountActiveCardsOnField
	add r4, r8
	add r4, r4, r6
	add r4, r4, r0
	lsl r1, r4, #2
	add r1, r1, r4
	b _0800C018
_0800BFCC: .4byte 0x000004E1
_0800BFD0:
	mov r0, #0
	mov r1, #7
	bl CountFaceUpMonstersOfType
	mov r4, #0x64
	mul r0, r4
	ldr r5, [sp, #0x2C]
	add r5, r5, r0
	mov r0, #1
	mov r1, #7
	bl CountFaceUpMonstersOfType
	mul r0, r4
	add r5, r5, r0
	str r5, [sp, #0x2C]
	b _0800C27C
_0800BFF0:
	ldr r6, [sp, #0x20]
	lsl r0, r6, #5
	sub r0, r0, r6
	lsl r0, r0, #2
	add r0, r0, r6
	lsl r0, r0, #2
	ldr r7, [sp, #0x2C]
	add r7, r7, r0
	str r7, [sp, #0x2C]
	b _0800C27C
_0800C004:
	ldr r2, _0800C02C @ =0x020192E4
	mov r0, #1
	ldr r1, [sp, #0]
	and r0, r1
	ldr r1, _0800C030 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r2, [r0, #2]
	lsl r1, r2, #2
	add r1, r1, r2
_0800C018:
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r3, [sp, #0x2C]
	add r3, r3, r0
	str r3, [sp, #0x2C]
	ldr r4, [sp, #0x30]
	add r4, r4, r0
	str r4, [sp, #0x30]
	b _0800C27C
_0800C02C: .4byte 0x020192E4
_0800C030: .4byte 0x00000D64
_0800C034:
	ldr r0, [sp, #0]
	mov r1, #0xB
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r5, [sp, #0x2C]
	add r5, r5, r1
	str r5, [sp, #0x2C]
	b _0800C27C
_0800C04E:
	mov r2, #1
	ldr r6, [sp, #0]
	and r2, r6
	mov r0, #0x94
	ldr r7, [sp, #4]
	mul r0, r7
	ldr r1, _0800C07C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0800C080 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	bhi _0800C084
	ldr r2, [sp, #0xC]
	add r2, #1
	str r2, [sp, #0xC]
	ldr r3, [sp, #0x14]
	add r3, #1
	str r3, [sp, #0x14]
	b _0800C27C
_0800C07C: .4byte 0x00000D64
_0800C080: .4byte 0x0201930C
_0800C084:
	ldr r4, [sp, #0x10]
	add r4, #1
	str r4, [sp, #0x10]
	ldr r5, [sp, #0x18]
	add r5, #1
	str r5, [sp, #0x18]
	b _0800C27C
_0800C092:
	mov r6, #0
	str r6, [sp, #8]
	mov r7, #1
	mov ip, r7
	ldr r0, _0800C0E4 @ =0x00000D64
	mov r8, r0
_0800C09E:
	ldr r2, [sp, #8]
	mov r1, ip
	and r2, r1
	mov r3, r8
	mul r3, r2
	ldr r4, _0800C0E8 @ =0x020192E4
	add r0, r3, r4
	ldrb r0, [r0, #4]
	ldr r6, [sp, #8]
	add r6, #1
	cmp r0, #0
	beq _0800C116
	add r1, r4, #0
	ldr r5, _0800C0EC @ =0x00000904
	add r7, r1, r5
	ldr r5, _0800C0F0 @ =0x000002D1
	ldr r0, _0800C0E4 @ =0x00000D64
	mul r0, r2
	add r0, r0, r1
	ldrb r4, [r0, #4]
_0800C0C6:
	add r0, r3, r7
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0800C0F4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, r5
	beq _0800C104
	cmp r1, r5
	bgt _0800C0F8
	cmp r1, #0x22
	beq _0800C104
	b _0800C10E
	.align 2, 0
_0800C0E4: .4byte 0x00000D64
_0800C0E8: .4byte 0x020192E4
_0800C0EC: .4byte 0x00000904
_0800C0F0: .4byte 0x000002D1
_0800C0F4: .4byte gCardIdToNumber
_0800C0F8:
	ldr r0, _0800C120 @ =0x000004BA
	cmp r1, r0
	beq _0800C104
	ldr r0, _0800C124 @ =0x000007F2
	cmp r1, r0
	bne _0800C10E
_0800C104:
	ldr r2, [sp, #0x2C]
	mov r0, #0x96
	lsl r0, r0, #1
	add r2, r2, r0
	str r2, [sp, #0x2C]
_0800C10E:
	add r3, #4
	sub r4, #1
	cmp r4, #0
	bne _0800C0C6
_0800C116:
	str r6, [sp, #8]
	cmp r6, #1
	ble _0800C09E
	b _0800C27C
	.align 2, 0
_0800C120: .4byte 0x000004BA
_0800C124: .4byte 0x000007F2
_0800C128:
	mov r0, #0
	mov r1, #0xA
	bl CountFaceUpMonstersOfType
	mov r4, #0xC8
	mul r0, r4
	ldr r1, [sp, #0x2C]
	add r1, r1, r0
	str r1, [sp, #0x2C]
	mov r0, #1
	mov r1, #0xA
_0800C13E:
	bl CountFaceUpMonstersOfType
	mul r0, r4
	ldr r2, [sp, #0x2C]
	add r2, r2, r0
	str r2, [sp, #0x2C]
	b _0800C27C
_0800C14C:
	ldr r2, _0800C1B8 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #0]
	sub r0, r1, r3
	and r0, r1
	ldr r1, _0800C1BC @ =0x00000D64
	mul r1, r0
	add r0, r1, r2
	ldrb r3, [r0, #4]
	cmp r3, #0
	beq _0800C19C
	ldr r5, _0800C1C0 @ =0x00000904
	add r0, r2, r5
	add r2, r1, r0
	str r3, [sp, #8]
_0800C16A:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r6, _0800C1C4 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #1
	bne _0800C190
	ldr r7, [sp, #0x2C]
	mov r0, #0xFA
	lsl r0, r0, #1
	add r7, r7, r0
	str r7, [sp, #0x2C]
_0800C190:
	add r2, #4
	ldr r1, [sp, #8]
	sub r1, #1
	str r1, [sp, #8]
	cmp r1, #0
	bne _0800C16A
_0800C19C:
	mov r0, #1
	ldr r2, [sp, #0]
	sub r0, r0, r2
	mov r1, #1
	bl CountFaceUpMonstersOfType
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r3, [sp, #0x2C]
	add r3, r3, r1
	b _0800C27A
_0800C1B8: .4byte 0x020192E4
_0800C1BC: .4byte 0x00000D64
_0800C1C0: .4byte 0x00000904
_0800C1C4: .4byte gCardStats
_0800C1C8:
	ldr r2, _0800C1F0 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #0]
	and r0, r4
	ldr r1, _0800C1F4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #2]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	lsl r0, r0, #4
	ldr r5, [sp, #0x2C]
	sub r5, r5, r0
	str r5, [sp, #0x2C]
	ldr r6, [sp, #0x30]
	sub r6, r6, r0
	str r6, [sp, #0x30]
	b _0800C27C
_0800C1F0: .4byte 0x020192E4
_0800C1F4: .4byte 0x00000D64
_0800C1F8:
	mov r0, #1
	ldr r7, [sp, #0]
	sub r0, r0, r7
	bl CountMonsters
	cmp r0, #0
	ble _0800C27C
	ldr r0, [sp, #0x2C]
	ldr r1, _0800C210 @ =0xFFFFFC18
	add r0, r0, r1
	str r0, [sp, #0x2C]
	b _0800C27C
_0800C210: .4byte 0xFFFFFC18
_0800C214:
	mov r0, #1
	ldr r2, [sp, #0]
	sub r0, r0, r2
	bl CountMonsters
	mov r1, #0xC8
	mul r0, r1
	ldr r3, [sp, #0x2C]
	sub r3, r3, r0
	b _0800C27A
_0800C228:
	ldr r0, _0800C250 @ =0x020192E0
	ldr r4, _0800C254 @ =0x00001B12
	add r0, r0, r4
	ldrb r1, [r0]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r5, [sp, #0]
	cmp r5, r0
	bne _0800C27C
	mov r0, #0x1C
	and r0, r1
	cmp r0, #0xC
	bne _0800C27C
	ldr r6, [sp, #0x2C]
	mov r7, #0x96
	lsl r7, r7, #1
	add r6, r6, r7
	str r6, [sp, #0x2C]
	b _0800C27C
	.align 2, 0
_0800C250: .4byte 0x020192E0
_0800C254: .4byte 0x00001B12
_0800C258:
	ldr r0, _0800C2D8 @ =0x020192E0
	ldr r1, _0800C2DC @ =0x00001B12
	add r0, r0, r1
	ldrb r1, [r0]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, [sp, #0]
	cmp r2, r0
	beq _0800C27C
	mov r0, #0x1C
	and r0, r1
	cmp r0, #0xC
	bne _0800C27C
	ldr r3, [sp, #0x2C]
	mov r4, #0x96
	lsl r4, r4, #1
	add r3, r3, r4
_0800C27A:
	str r3, [sp, #0x2C]
_0800C27C:
	mov r5, r9
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1B
	lsr r0, r0, #0x1B
	cmp r0, #0xD
	bne _0800C2BE
	ldr r4, _0800C2E0 @ =0x000004E4
	ldr r0, [sp, #0]
	add r1, r4, #0
	mov r2, #1
	bl CountFaceUpMonstersByNumberInPosition
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r6, [sp, #0x2C]
	add r6, r6, r1
	str r6, [sp, #0x2C]
	ldr r0, [sp, #0]
	add r1, r4, #0
	mov r2, #1
	bl CountFaceUpMonstersByNumberInPosition
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r7, [sp, #0x30]
	add r7, r7, r1
	str r7, [sp, #0x30]
_0800C2BE:
	mov r1, r9
	ldrb r1, [r1, #2]
	lsr r0, r1, #5
	sub r0, #1
	cmp r0, #5
	bls _0800C2CC
	b _0800C550
_0800C2CC:
	lsl r0, r0, #2
	ldr r1, _0800C2E4 @ =0x0800C2E8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800C2D8: .4byte 0x020192E0
_0800C2DC: .4byte 0x00001B12
_0800C2E0: .4byte 0x000004E4
_0800C2E4: .4byte 0x0800C2E8
_0800C2E8:
	.4byte _0800C300
	.4byte _0800C364
	.4byte _0800C398
	.4byte _0800C430
	.4byte _0800C490
	.4byte _0800C4F8
_0800C300:
	ldr r4, _0800C360 @ =0x000001EB
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r2, [sp, #0x2C]
	add r2, r2, r1
	str r2, [sp, #0x2C]
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r3, [sp, #0x2C]
	add r3, r3, r1
	str r3, [sp, #0x2C]
	add r4, #0x88
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r5, [sp, #0x2C]
	sub r5, r5, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	sub r5, r5, r1
	b _0800C54E
_0800C360: .4byte 0x000001EB
_0800C364:
	ldr r4, _0800C394 @ =0x000001EB
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r6, [sp, #0x2C]
	sub r6, r6, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	sub r6, r6, r1
	add r4, #0x88
	b _0800C45E
_0800C394: .4byte 0x000001EB
_0800C398:
	ldr r4, _0800C428 @ =0x0000020B
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r7, [sp, #0x2C]
	add r7, r7, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r0, r7, #0
	add r0, r0, r1
	str r0, [sp, #0x2C]
	add r4, #0x4A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r2, [sp, #0x2C]
	sub r2, r2, r1
	str r2, [sp, #0x2C]
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r3, [sp, #0x2C]
	sub r3, r3, r1
	str r3, [sp, #0x2C]
	ldr r4, _0800C42C @ =0x0000058E
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r5, [sp, #0x2C]
	sub r5, r5, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	sub r5, r5, r1
	b _0800C54E
_0800C428: .4byte 0x0000020B
_0800C42C: .4byte 0x0000058E
_0800C430:
	ldr r4, _0800C48C @ =0x0000020B
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r6, [sp, #0x2C]
	sub r6, r6, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	sub r6, r6, r1
	add r4, #0x4A
_0800C45E:
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r6, r6, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r6, r6, r1
	str r6, [sp, #0x2C]
	b _0800C550
	.align 2, 0
_0800C48C: .4byte 0x0000020B
_0800C490:
	ldr r4, _0800C4F4 @ =0x0000020E
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r7, [sp, #0x2C]
	add r7, r7, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r0, r7, #0
	add r0, r0, r1
	str r0, [sp, #0x2C]
	add r4, #0x52
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r2, [sp, #0x2C]
	sub r2, r2, r1
	str r2, [sp, #0x2C]
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r3, [sp, #0x2C]
	sub r3, r3, r1
	str r3, [sp, #0x2C]
	b _0800C550
_0800C4F4: .4byte 0x0000020E
_0800C4F8:
	ldr r4, _0800C624 @ =0x0000020E
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r5, [sp, #0x2C]
	sub r5, r5, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r1, r1, r0
	lsl r1, r1, #4
	sub r5, r5, r1
	add r4, #0x52
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r5, r5, r1
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r5, r5, r1
_0800C54E:
	str r5, [sp, #0x2C]
_0800C550:
	ldr r2, _0800C628 @ =0x020192E0
	ldr r6, _0800C62C @ =0x00001ACD
	add r1, r2, r6
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800C562
	b _0800C6CE
_0800C562:
	ldr r7, [sp, #0x38]
	cmp r7, #0
	beq _0800C56A
	b _0800C6CE
_0800C56A:
	mov r0, #0
	str r0, [sp, #8]
	mov r6, #1
	ldr r1, _0800C630 @ =0x00000D64
	mov ip, r1
	add r5, r2, #0
	add r5, #0x2C
	ldr r2, _0800C634 @ =0x080815A8
	mov r8, r2
	ldr r7, _0800C638 @ =0x080816C8
	ldr r1, [sp, #0]
	and r1, r6
	mov r0, #0x94
	ldr r3, [sp, #4]
	mul r0, r3
	mov r4, ip
	mul r4, r1
	add r1, r4, #0
	add r0, r0, r1
	add r0, r0, r5
	str r0, [sp, #0x48]
_0800C594:
	ldr r0, [sp, #8]
	and r0, r6
	mov r1, ip
	mul r1, r0
	add r3, r1, r5
	mov r0, #0xB9
	lsl r0, r0, #3
	add r4, r3, r0
	add r0, r5, r0
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	bne _0800C5B4
	b _0800C6C2
_0800C5B4:
	ldr r0, _0800C63C @ =0x00000659
	add r1, r3, r0
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800C5C4
	b _0800C6C2
_0800C5C4:
	mov r0, #2
	ldrb r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	beq _0800C6C2
	ldr r0, _0800C640 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0800C644 @ =0x08622AB4
	add r0, r0, r1
	ldrh r4, [r0]
	ldr r0, _0800C648 @ =0x0000042D
	cmp r4, r0
	beq _0800C6AC
	cmp r4, r0
	bgt _0800C650
	mov r0, #0xA7
	lsl r0, r0, #1
	cmp r4, r0
	bgt _0800C6C2
	sub r0, #5
	cmp r4, r0
	blt _0800C6C2
	mov r2, r9
	ldrb r2, [r2, #2]
	lsl r1, r2, #0x1B
	lsr r3, r1, #0x1A
	ldr r2, _0800C64C @ =0xFFFFFEB7
	add r0, r4, r2
	lsl r2, r0, #1
	add r2, r2, r0
	lsl r2, r2, #4
	add r3, r3, r2
	add r3, r8
	mov r4, #0
	ldsh r0, [r3, r4]
	ldr r3, [sp, #0x2C]
	add r3, r3, r0
	str r3, [sp, #0x2C]
	lsr r1, r1, #0x1A
	add r1, r1, r2
	add r1, r8
	mov r4, #0
	ldsh r0, [r1, r4]
	ldr r1, [sp, #0x30]
	add r1, r1, r0
	str r1, [sp, #0x30]
	b _0800C6C2
_0800C624: .4byte 0x0000020E
_0800C628: .4byte 0x020192E0
_0800C62C: .4byte 0x00001ACD
_0800C630: .4byte 0x00000D64
_0800C634: .4byte gFieldTypeBonuses
_0800C638: .4byte gFieldAttributeBonuses
_0800C63C: .4byte 0x00000659
_0800C640: .4byte 0x000007FF
_0800C644: .4byte gCardIdToNumber
_0800C648: .4byte 0x0000042D
_0800C64C: .4byte 0xFFFFFEB7
_0800C650:
	ldr r0, _0800C6A4 @ =0x0000046A
	cmp r4, r0
	bgt _0800C6C2
	sub r0, #5
	cmp r4, r0
	blt _0800C6C2
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r2, r3, #0x18
	lsr r0, r2, #0x1D
	lsl r0, r0, #1
	ldr r1, _0800C6A8 @ =0xFFFFFB9B
	add r3, r4, r1
	lsl r3, r3, #4
	add r0, r0, r3
	add r0, r0, r7
	mov r4, #0
	ldsh r1, [r0, r4]
	lsl r0, r1, #5
	sub r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r1, [sp, #0x2C]
	add r1, r1, r0
	str r1, [sp, #0x2C]
	lsr r2, r2, #0x1D
	lsl r2, r2, #1
	add r2, r2, r3
	add r2, r2, r7
	mov r3, #0
	ldsh r1, [r2, r3]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	lsl r0, r0, #4
	ldr r4, [sp, #0x30]
	sub r4, r4, r0
	str r4, [sp, #0x30]
	b _0800C6C2
	.align 2, 0
_0800C6A4: .4byte 0x0000046A
_0800C6A8: .4byte 0xFFFFFB9B
_0800C6AC:
	add r0, r6, #0
	ldr r1, [sp, #0x48]
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800C6C2
	ldr r2, [sp, #0x30]
	mov r3, #0xFA
	lsl r3, r3, #1
	add r2, r2, r3
	str r2, [sp, #0x30]
_0800C6C2:
	ldr r4, [sp, #8]
	add r4, #1
	str r4, [sp, #8]
	cmp r4, #1
	bgt _0800C6CE
	b _0800C594
_0800C6CE:
	ldr r5, _0800C764 @ =0x020192E0
	ldr r6, _0800C768 @ =0x00001B12
	add r0, r5, r6
	ldrb r2, [r0]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	ldr r7, [sp, #0]
	cmp r7, r0
	bne _0800C6FE
	mov r0, #0x1C
	and r0, r2
	cmp r0, #0xC
	bne _0800C6FE
	mov r0, #1
	sub r0, r0, r7
	ldr r1, _0800C76C @ =0x000005EB
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0800C6FE
	ldr r0, [sp, #0x2C]
	ldr r1, _0800C770 @ =0xFFFFFED4
	add r0, r0, r1
	str r0, [sp, #0x2C]
_0800C6FE:
	ldr r0, [sp, #0]
	ldr r1, [sp, #4]
	bl CountAquaChorusBoosts
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r2, [sp, #0x2C]
	add r2, r2, r1
	str r2, [sp, #0x2C]
	ldr r0, [sp, #0]
	ldr r1, [sp, #4]
	bl CountAquaChorusBoosts
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r3, [sp, #0x30]
	add r3, r3, r1
	str r3, [sp, #0x30]
	ldr r0, _0800C764 @ =0x020192E0
	ldr r4, _0800C774 @ =0x00001ACC
	add r0, r0, r4
	mov r1, #0x82
	lsl r1, r1, #6
	ldrh r0, [r0]
	and r1, r0
	mov r0, #0x80
	lsl r0, r0, #6
	cmp r1, r0
	bne _0800C778
	ldr r1, [sp, #0x2C]
	add r1, sl
	ldr r5, [sp, #0x24]
	add r1, r1, r5
	mov r6, r9
	ldr r0, [r6, #4]
	sub r0, r0, r1
	str r0, [r6, #4]
	ldr r7, [sp, #0x1C]
	add r1, r3, r7
	ldr r0, [sp, #0x28]
	add r1, r1, r0
	ldr r0, [r6, #8]
	sub r0, r0, r1
	str r0, [r6, #8]
	b _0800C798
_0800C764: .4byte 0x020192E0
_0800C768: .4byte 0x00001B12
_0800C76C: .4byte 0x000005EB
_0800C770: .4byte 0xFFFFFED4
_0800C774: .4byte 0x00001ACC
_0800C778:
	ldr r1, [sp, #0x2C]
	add r1, sl
	ldr r2, [sp, #0x24]
	add r1, r1, r2
	mov r3, r9
	ldr r0, [r3, #4]
	add r0, r0, r1
	str r0, [r3, #4]
	ldr r4, [sp, #0x30]
	ldr r5, [sp, #0x1C]
	add r1, r4, r5
	ldr r6, [sp, #0x28]
	add r1, r1, r6
	ldr r0, [r3, #8]
	add r0, r0, r1
	str r0, [r3, #8]
_0800C798:
	mov r7, r9
	ldr r0, [r7, #4]
	cmp r0, #0
	bge _0800C7A4
	mov r0, #0
	str r0, [r7, #4]
_0800C7A4:
	mov r1, r9
	ldr r0, [r1, #8]
	cmp r0, #0
	bge _0800C7B0
	mov r0, #0
	str r0, [r1, #8]
_0800C7B0:
	mov r0, #0x20
	ldr r2, [sp, #0x4C]
	ldrb r2, [r2]
	and r0, r2
	cmp r0, #0
	beq _0800C7C2
	ldr r3, [sp, #0xC]
	add r3, #1
	str r3, [sp, #0xC]
_0800C7C2:
	ldr r4, [sp, #0x10]
	ldr r5, [sp, #0xC]
	cmp r4, r5
	ble _0800C7E6
	sub r0, r4, r5
	cmp r0, #0
	ble _0800C7E6
	mov r6, r9
	ldr r1, [r6, #4]
	str r0, [sp, #8]
_0800C7D6:
	lsl r1, r1, #1
	ldr r7, [sp, #8]
	sub r7, #1
	str r7, [sp, #8]
	cmp r7, #0
	bne _0800C7D6
	mov r0, r9
	str r1, [r0, #4]
_0800C7E6:
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0xC]
	cmp r1, r2
	bge _0800C80C
	sub r0, r2, r1
	cmp r0, #0
	ble _0800C80C
	str r0, [sp, #8]
_0800C7F6:
	mov r3, r9
	ldr r0, [r3, #4]
	bl HalveRoundUp
	mov r4, r9
	str r0, [r4, #4]
	ldr r5, [sp, #8]
	sub r5, #1
	str r5, [sp, #8]
	cmp r5, #0
	bne _0800C7F6
_0800C80C:
	ldr r6, [sp, #0x18]
	ldr r7, [sp, #0x14]
	cmp r6, r7
	ble _0800C830
	sub r0, r6, r7
	cmp r0, #0
	ble _0800C830
	mov r2, r9
	ldr r1, [r2, #8]
	str r0, [sp, #8]
_0800C820:
	lsl r1, r1, #1
	ldr r3, [sp, #8]
	sub r3, #1
	str r3, [sp, #8]
	cmp r3, #0
	bne _0800C820
	mov r4, r9
	str r1, [r4, #8]
_0800C830:
	ldr r5, [sp, #0x18]
	ldr r6, [sp, #0x14]
	cmp r5, r6
	bge _0800C854
	sub r0, r6, r5
	cmp r0, #0
	ble _0800C854
	str r0, [sp, #8]
_0800C840:
	mov r7, r9
	ldr r0, [r7, #8]
	bl HalveRoundUp
	str r0, [r7, #8]
	ldr r0, [sp, #8]
	sub r0, #1
	str r0, [sp, #8]
	cmp r0, #0
	bne _0800C840
_0800C854:
	ldr r0, _0800C888 @ =0x020192E0
	ldr r1, _0800C88C @ =0x00001ACC
	add r0, r0, r1
	ldr r1, _0800C890 @ =0x00004040
	ldrh r0, [r0]
	and r1, r0
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r1, r0
	bne _0800C878
	ldr r2, [sp, #0x38]
	cmp r2, #0
	bne _0800C878
	mov r3, r9
	ldr r0, [r3, #8]
	ldr r1, [r3, #4]
	str r0, [r3, #4]
	str r1, [r3, #8]
_0800C878:
	add sp, #0x50
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800C888: .4byte 0x020192E0
_0800C88C: .4byte 0x00001ACC
_0800C890: .4byte 0x00004040
	thumb_func_end GetZoneCardStats

