	thumb_func_start sub_0804AE64
sub_0804AE64: @ 0x0804AE64
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r7, r0, #0
	mov r0, #1
	add r3, r7, #0
	and r3, r0
	ldr r4, _0804AED8 @ =0x02018450
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	mov r2, #0x94
	mul r1, r2
	ldr r2, _0804AEDC @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r6, _0804AEE0 @ =0x0201930C
	add r1, r1, r6
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r5, r1, #0x14
	sub r0, r0, r7
	bl sub_08008860
	cmp r0, #0
	bne _0804AEA6
	mov r0, #2
	ldrb r3, [r4]
	orr r0, r3
	strb r0, [r4]
_0804AEA6:
	mov r0, #2
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _0804AEE8
	mov r0, #0x34
	cmp r7, #0
	beq _0804AEB8
	ldr r0, _0804AEE4 @ =0x00008034
_0804AEB8:
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0xF
	neg r0, r0
	ldrb r3, [r4, #1]
	and r0, r3
_0804AECE:
	mov r1, #0xA
	orr r0, r1
	strb r0, [r4, #1]
	b _0804B62C
	.align 2, 0
_0804AED8: .4byte 0x02018450
_0804AEDC: .4byte 0x00000D64
_0804AEE0: .4byte 0x0201930C
_0804AEE4: .4byte 0x00008034
_0804AEE8:
	ldr r1, _0804AF04 @ =0x00001AEA
	add r0, r6, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x18
	cmp r0, #0x1E
	bls _0804AEF8
	b _0804B62C
_0804AEF8:
	lsl r0, r0, #2
	ldr r1, _0804AF08 @ =0x0804AF0C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804AF04: .4byte 0x00001AEA
_0804AF08: .4byte 0x0804AF0C
_0804AF0C:
	.4byte _0804AF88
	.4byte _0804B180
	.4byte _0804B214
	.4byte _0804B30C
	.4byte _0804B3F8
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B41C
	.4byte _0804B48C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B4DC
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B62C
	.4byte _0804B55C
_0804AF88:
	mov r0, #1
	sub r0, r0, r7
	mov r8, r0
	ldr r4, _0804B040 @ =0x00000422
	add r1, r4, #0
	bl sub_0800AB08
	cmp r0, #0
	bgt _0804AF9C
	b _0804B164
_0804AF9C:
	lsl r0, r4, #1
	ldr r2, _0804B044 @ =0x08623DF4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, r8
	bl sub_080197E0
	mov r6, r8
	mov sl, r4
	add r0, r6, #0
	mov r1, sl
	bl sub_0800AB08
	add r5, r0, #0
	cmp r5, #1
	beq _0804AFBE
	b _0804B0F8
_0804AFBE:
	cmp r7, #0
	bne _0804AFC4
	b _0804B0BC
_0804AFC4:
	add r0, r6, #0
	mov r1, sl
	bl sub_0800AB6C
	add r4, r0, #0
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C894
	str r0, [sp, #0]
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C8A8
	str r0, [sp, #4]
	ldr r3, _0804B048 @ =0x02018450
	mov r9, r3
	ldrh r0, [r3]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0800C894
	add r3, r0, #0
	add r2, r6, #0
	and r2, r5
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0804B04C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r4, _0804B050 @ =0x0201930C
	add r0, r0, r4
	ldrb r0, [r0, #6]
	and r5, r0
	cmp r5, #0
	beq _0804B05C
	ldr r1, [sp, #4]
	cmp r3, r1
	ble _0804B098
	add r0, r6, #0
	mov r1, sl
	bl sub_0800AB6C
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0xF
	neg r1, r1
	mov r2, r9
	ldrb r2, [r2, #1]
	and r1, r2
	orr r1, r0
	mov r3, r9
	strb r1, [r3, #1]
	ldr r0, _0804B054 @ =0x00001AEA
	add r2, r4, r0
	ldr r0, _0804B058 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #6
	b _0804B470
_0804B040: .4byte 0x00000422
_0804B044: .4byte gUnk_08623DF4
_0804B048: .4byte 0x02018450
_0804B04C: .4byte 0x00000D64
_0804B050: .4byte 0x0201930C
_0804B054: .4byte 0x00001AEA
_0804B058: .4byte 0xFFFFFE01
_0804B05C:
	ldr r2, [sp, #0]
	cmp r3, r2
	ble _0804B098
	mov r0, r8
	mov r1, sl
	bl sub_0800AB6C
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0xF
	neg r1, r1
	mov r3, r9
	ldrb r3, [r3, #1]
	and r1, r3
	orr r1, r0
	mov r0, r9
	strb r1, [r0, #1]
	ldr r1, _0804B090 @ =0x00001AEA
	add r2, r4, r1
	ldr r0, _0804B094 @ =0xFFFFFE01
	ldrh r3, [r2]
	and r0, r3
	mov r1, #6
	b _0804B470
	.align 2, 0
_0804B090: .4byte 0x00001AEA
_0804B094: .4byte 0xFFFFFE01
_0804B098:
	ldr r0, _0804B0B0 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A39C
	ldr r2, _0804B0B4 @ =0x020192E0
	ldr r0, _0804B0B8 @ =0x00001B16
	add r1, r2, r0
	b _0804B110
	.align 2, 0
_0804B0B0: .4byte 0x02018450
_0804B0B4: .4byte 0x020192E0
_0804B0B8: .4byte 0x00001B16
_0804B0BC:
	mov r0, #1
	add r1, r4, #0
	bl sub_0800AB6C
	ldr r2, _0804B0E8 @ =0x02018450
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0xF
	neg r1, r1
	ldrb r3, [r2, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r2, #1]
	ldr r2, _0804B0EC @ =0x020192E0
	ldr r0, _0804B0F0 @ =0x00001B16
	add r2, r2, r0
	ldr r0, _0804B0F4 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #8
	b _0804B470
_0804B0E8: .4byte 0x02018450
_0804B0EC: .4byte 0x020192E0
_0804B0F0: .4byte 0x00001B16
_0804B0F4: .4byte 0xFFFFFE01
_0804B0F8:
	cmp r7, #0
	beq _0804B148
	ldr r0, _0804B130 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A39C
	ldr r2, _0804B134 @ =0x020192E0
	ldr r3, _0804B138 @ =0x00001B16
	add r1, r2, r3
_0804B110:
	ldr r0, _0804B13C @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r0, _0804B140 @ =0x00001B14
	add r2, r2, r0
	ldr r0, [r2]
	ldr r1, _0804B144 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	orr r0, r1
	str r0, [r2]
_0804B12A:
	mov r0, #0
	b _0804B62E
	.align 2, 0
_0804B130: .4byte 0x02018450
_0804B134: .4byte 0x020192E0
_0804B138: .4byte 0x00001B16
_0804B13C: .4byte 0xFFFFFE01
_0804B140: .4byte 0x00001B14
_0804B144: .4byte 0xFFFE01FF
_0804B148:
	ldr r0, _0804B158 @ =0x020192E0
	ldr r1, _0804B15C @ =0x00001B16
	add r0, r0, r1
	ldr r1, _0804B160 @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #0x28
	b _0804B4CA
_0804B158: .4byte 0x020192E0
_0804B15C: .4byte 0x00001B16
_0804B160: .4byte 0xFFFFFE01
_0804B164:
	ldr r2, _0804B1B0 @ =0x020192E0
	ldr r3, _0804B1B4 @ =0x00001B16
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804B1B8 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0804B180:
	ldr r0, _0804B1BC @ =0x000007FF
	and r5, r0
	lsl r0, r5, #1
	ldr r1, _0804B1C0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_0800756C
	cmp r0, #0
	beq _0804B1C4
	mov r0, #1
	sub r0, r0, r7
	bl sub_08008F74
	cmp r0, #0
	beq _0804B1C4
	ldr r0, _0804B1B0 @ =0x020192E0
	ldr r2, _0804B1B4 @ =0x00001B16
	add r0, r0, r2
	ldr r1, _0804B1B8 @ =0xFFFFFE01
	ldrh r3, [r0]
	and r1, r3
	mov r2, #0x3C
	b _0804B4CA
_0804B1B0: .4byte 0x020192E0
_0804B1B4: .4byte 0x00001B16
_0804B1B8: .4byte 0xFFFFFE01
_0804B1BC: .4byte 0x000007FF
_0804B1C0: .4byte gUnk_08622AB4
_0804B1C4:
	ldr r0, _0804B1E8 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A3D8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804B1F8
	ldr r0, _0804B1EC @ =0x020192E0
	ldr r1, _0804B1F0 @ =0x00001B16
	add r0, r0, r1
	ldr r1, _0804B1F4 @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #0x14
	b _0804B4CA
_0804B1E8: .4byte 0x02018450
_0804B1EC: .4byte 0x020192E0
_0804B1F0: .4byte 0x00001B16
_0804B1F4: .4byte 0xFFFFFE01
_0804B1F8:
	ldr r2, _0804B254 @ =0x020192E0
	ldr r3, _0804B258 @ =0x00001B16
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804B25C @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0804B214:
	cmp r7, #0
	beq _0804B2F0
	mov r0, #0
	bl sub_0805809C
	cmp r0, #0
	beq _0804B2A0
	ldr r2, _0804B260 @ =0x02015F00
	mov r0, #2
	ldrb r1, [r2, #0xC]
	and r0, r1
	cmp r0, #0
	beq _0804B26C
	ldr r4, _0804B264 @ =0x02018450
	mov r0, #2
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r0, _0804B268 @ =0x00008034
	ldrh r3, [r4]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0xF
	neg r0, r0
	ldrb r1, [r4, #1]
	and r0, r1
	b _0804AECE
	.align 2, 0
_0804B254: .4byte 0x020192E0
_0804B258: .4byte 0x00001B16
_0804B25C: .4byte 0xFFFFFE01
_0804B260: .4byte 0x02015F00
_0804B264: .4byte 0x02018450
_0804B268: .4byte 0x00008034
_0804B26C:
	ldr r1, _0804B298 @ =0x02018450
	ldrh r2, [r2, #0xC]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1D
	lsl r0, r0, #1
	mov r2, #0xF
	neg r2, r2
	ldrb r3, [r1, #1]
	and r2, r3
	orr r2, r0
	strb r2, [r1, #1]
	ldr r0, _0804B29C @ =0x00008033
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x1D
	lsl r2, r2, #0x1C
	lsr r2, r2, #0x1D
_0804B28E:
	mov r3, #0
	bl sub_0801EC58
	b _0804B62C
	.align 2, 0
_0804B298: .4byte 0x02018450
_0804B29C: .4byte 0x00008033
_0804B2A0:
	ldr r0, _0804B2D4 @ =0x00008035
	ldr r1, _0804B2D8 @ =0x02018450
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r2, _0804B2DC @ =0x020192E0
	ldr r0, _0804B2E0 @ =0x00001B14
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _0804B2E4 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	orr r0, r1
	str r0, [r3]
	ldr r1, _0804B2E8 @ =0x00001B16
	add r2, r2, r1
	ldr r0, _0804B2EC @ =0xFFFFFE01
	ldrh r3, [r2]
	and r0, r3
	b _0804B472
	.align 2, 0
_0804B2D4: .4byte 0x00008035
_0804B2D8: .4byte 0x02018450
_0804B2DC: .4byte 0x020192E0
_0804B2E0: .4byte 0x00001B14
_0804B2E4: .4byte 0xFFFE01FF
_0804B2E8: .4byte 0x00001B16
_0804B2EC: .4byte 0xFFFFFE01
_0804B2F0:
	ldr r0, _0804B300 @ =0x00000206
	ldr r1, _0804B304 @ =0x00000511
	ldr r3, _0804B308 @ =0x08085958
	mov r2, #0xB
	bl sub_080602A4
	b _0804B458
	.align 2, 0
_0804B300: .4byte 0x00000206
_0804B304: .4byte 0x00000511
_0804B308: .4byte gUnk_08085958
_0804B30C:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0804B3B4
	ldr r0, _0804B360 @ =0x0201CFB0
	ldr r1, _0804B364 @ =0x0000082C
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0804B368 @ =0x0201A070
	add r1, r1, r0
	mov r0, #2
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	beq _0804B37C
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	ble _0804B37C
	ldr r0, _0804B36C @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _0804B370 @ =0x08622AB4
	add r0, r0, r3
	ldr r1, _0804B374 @ =0x0000052E
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804B37C
	bl sub_080094E4
	ldr r1, _0804B378 @ =0x0000014D
	cmp r0, r1
	bne _0804B37C
	mov r0, #3
	bl sub_08077AEC
	b _0804B12A
_0804B360: .4byte 0x0201CFB0
_0804B364: .4byte 0x0000082C
_0804B368: .4byte 0x0201A070
_0804B36C: .4byte 0x000007FF
_0804B370: .4byte gUnk_08622AB4
_0804B374: .4byte 0x0000052E
_0804B378: .4byte 0x0000014D
_0804B37C:
	ldr r2, _0804B3DC @ =0x02018450
	ldr r0, _0804B3E0 @ =0x0201CFB0
	ldr r1, _0804B3E4 @ =0x0000082C
	add r0, r0, r1
	mov r1, #7
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r3, [r2, #1]
	and r0, r3
	orr r0, r1
	strb r0, [r2, #1]
	ldr r2, _0804B3E8 @ =0x020192E0
	ldr r0, _0804B3EC @ =0x00001B16
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804B3F0 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0804B3B4:
	ldr r1, _0804B3F4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804B3C2
	b _0804B12A
_0804B3C2:
	mov r0, #2
	bl sub_08077AEC
	ldr r0, _0804B3DC @ =0x02018450
	ldrh r0, [r0]
	lsl r2, r0, #0x17
	lsr r2, r2, #0x1D
	add r0, r7, #0
	mov r1, #0
	bl sub_08024134
	b _0804B5CC
	.align 2, 0
_0804B3DC: .4byte 0x02018450
_0804B3E0: .4byte 0x0201CFB0
_0804B3E4: .4byte 0x0000082C
_0804B3E8: .4byte 0x020192E0
_0804B3EC: .4byte 0x00001B16
_0804B3F0: .4byte 0xFFFFFE01
_0804B3F4: .4byte 0x03000040
_0804B3F8:
	mov r3, #0x33
	cmp r7, #0
	beq _0804B400
	ldr r3, _0804B414 @ =0x00008033
_0804B400:
	ldr r0, _0804B418 @ =0x02018450
	ldrh r2, [r0]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	ldrb r0, [r0, #1]
	lsl r2, r0, #0x1C
	lsr r2, r2, #0x1D
	add r0, r3, #0
	b _0804B28E
	.align 2, 0
_0804B414: .4byte 0x00008033
_0804B418: .4byte 0x02018450
_0804B41C:
	cmp r7, #0
	beq _0804B440
	ldr r1, _0804B438 @ =0x02018450
	mov r0, #2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r0, _0804B43C @ =0x00008034
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	b _0804B28E
	.align 2, 0
_0804B438: .4byte 0x02018450
_0804B43C: .4byte 0x00008034
_0804B440:
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804B478 @ =0x00000715
	ldr r3, _0804B47C @ =0x0808598C
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
_0804B458:
	ldr r2, _0804B480 @ =0x020192E0
	ldr r0, _0804B484 @ =0x00001B16
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804B488 @ =0xFFFFFE01
	and r0, r3
_0804B470:
	orr r0, r1
_0804B472:
	strh r0, [r2]
	b _0804B12A
	.align 2, 0
_0804B478: .4byte 0x00000715
_0804B47C: .4byte gUnk_0808598C
_0804B480: .4byte 0x020192E0
_0804B484: .4byte 0x00001B16
_0804B488: .4byte 0xFFFFFE01
_0804B48C:
	ldr r0, _0804B4B0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0804B4BC
	ldr r1, _0804B4B4 @ =0x02018450
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r0, #0x34
	cmp r7, #0
	beq _0804B4A6
	ldr r0, _0804B4B8 @ =0x00008034
_0804B4A6:
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	b _0804B28E
_0804B4B0: .4byte 0x0201AE60
_0804B4B4: .4byte 0x02018450
_0804B4B8: .4byte 0x00008034
_0804B4BC:
	ldr r0, _0804B4D0 @ =0x020192E0
	ldr r3, _0804B4D4 @ =0x00001B16
	add r0, r0, r3
	ldr r1, _0804B4D8 @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #4
_0804B4CA:
	orr r1, r2
	strh r1, [r0]
	b _0804B12A
_0804B4D0: .4byte 0x020192E0
_0804B4D4: .4byte 0x00001B16
_0804B4D8: .4byte 0xFFFFFE01
_0804B4DC:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0804B54A
	mov r0, #1
	sub r0, r0, r7
	ldr r1, _0804B528 @ =0x0201CFB0
	ldr r3, _0804B52C @ =0x0000082C
	add r4, r1, r3
	ldr r1, [r4]
	ldr r2, _0804B530 @ =0x00000422
	bl sub_0800A78C
	cmp r0, #0
	beq _0804B544
	ldr r2, _0804B534 @ =0x02018450
	mov r1, #7
	ldrb r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r3, [r2, #1]
	and r0, r3
	orr r0, r1
	strb r0, [r2, #1]
	ldr r2, _0804B538 @ =0x020192E0
	ldr r0, _0804B53C @ =0x00001B16
	add r2, r2, r0
	ldr r0, _0804B540 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strh r0, [r2]
	b _0804B54A
_0804B528: .4byte 0x0201CFB0
_0804B52C: .4byte 0x0000082C
_0804B530: .4byte 0x00000422
_0804B534: .4byte 0x02018450
_0804B538: .4byte 0x020192E0
_0804B53C: .4byte 0x00001B16
_0804B540: .4byte 0xFFFFFE01
_0804B544:
	mov r0, #3
	bl sub_08077AEC
_0804B54A:
	ldr r1, _0804B558 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804B5C6
	b _0804B12A
_0804B558: .4byte 0x03000040
_0804B55C:
	cmp r7, #0
	beq _0804B562
	b _0804B12A
_0804B562:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0804B5B8
	ldr r0, _0804B5FC @ =0x0201CFB0
	ldr r1, _0804B600 @ =0x0000082C
	add r4, r0, r1
	ldr r1, [r4]
	mov r0, #0x94
	mul r0, r1
	ldr r5, _0804B604 @ =0x0201A070
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r2, _0804B608 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_0800756C
	cmp r0, #0
	beq _0804B5B8
	ldr r2, _0804B60C @ =0x02018450
	mov r1, #7
	ldrb r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r3, [r2, #1]
	and r0, r3
	orr r0, r1
	strb r0, [r2, #1]
	ldr r0, _0804B610 @ =0x00000D86
	add r2, r5, r0
	ldr r0, _0804B614 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #6
	orr r0, r1
	strh r0, [r2]
_0804B5B8:
	ldr r1, _0804B618 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804B5C6
	b _0804B12A
_0804B5C6:
	mov r0, #2
	bl sub_08077AEC
_0804B5CC:
	ldr r2, _0804B61C @ =0x020192E0
	ldr r3, _0804B620 @ =0x00001B16
	add r1, r2, r3
	ldr r0, _0804B614 @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r0, _0804B624 @ =0x00001B14
	add r2, r2, r0
	ldr r3, [r2]
	lsl r0, r3, #0xF
	lsr r0, r0, #0x18
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #9
	ldr r1, _0804B628 @ =0xFFFE01FF
	and r1, r3
	orr r1, r0
	str r1, [r2]
	b _0804B12A
	.align 2, 0
_0804B5FC: .4byte 0x0201CFB0
_0804B600: .4byte 0x0000082C
_0804B604: .4byte 0x0201A070
_0804B608: .4byte gUnk_08622AB4
_0804B60C: .4byte 0x02018450
_0804B610: .4byte 0x00000D86
_0804B614: .4byte 0xFFFFFE01
_0804B618: .4byte 0x03000040
_0804B61C: .4byte 0x020192E0
_0804B620: .4byte 0x00001B16
_0804B624: .4byte 0x00001B14
_0804B628: .4byte 0xFFFE01FF
_0804B62C:
	mov r0, #1
_0804B62E:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804AE64
	.align 2, 0

