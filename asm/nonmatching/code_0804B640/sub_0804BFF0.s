	thumb_func_start sub_0804BFF0
sub_0804BFF0: @ 0x0804BFF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r4, _0804C020 @ =0xFFFFFE00
	add sp, r4
	add r7, r0, #0
	ldr r0, _0804C024 @ =0x020192E0
	ldr r2, _0804C028 @ =0x00001B16
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov sl, r0
	cmp r1, #0x23
	bls _0804C016
	bl _0804CB40 @ far jump
_0804C016:
	lsl r0, r1, #2
	ldr r1, _0804C02C @ =0x0804C030
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0804C020: .4byte 0xFFFFFE00
_0804C024: .4byte 0x020192E0
_0804C028: .4byte 0x00001B16
_0804C02C: .4byte 0x0804C030
_0804C030:
	.4byte _0804C0C0
	.4byte _0804C228
	.4byte _0804C370
	.4byte _0804C3B4
	.4byte _0804C438
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804C480
	.4byte _0804C5BC
	.4byte _0804C5F0
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804C678
	.4byte _0804C6B8
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804CB40
	.4byte _0804C740
	.4byte _0804C880
	.4byte _0804C8B4
	.4byte _0804C91C
	.4byte _0804C9DC
	.4byte _0804CA6C
_0804C0C0:
	mov r2, #1
	and r2, r7
	ldr r0, _0804C0F8 @ =0x02018450
	ldrh r0, [r0]
	lsl r3, r0, #0x17
	lsr r0, r3, #0x1D
	mov r5, #0x94
	mul r0, r5
	ldr r1, _0804C0FC @ =0x00000D64
	mul r2, r1
	add r0, r0, r2
	ldr r4, _0804C100 @ =0x0201930C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804C104 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xA7
	lsl r0, r0, #3
	cmp r1, r0
	beq _0804C108
	add r0, #8
	cmp r1, r0
	beq _0804C134
	b _0804C146
	.align 2, 0
_0804C0F8: .4byte 0x02018450
_0804C0FC: .4byte 0x00000D64
_0804C100: .4byte 0x0201930C
_0804C104: .4byte gUnk_08622AB4
_0804C108:
	lsr r0, r3, #0x1D
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r2
	add r1, r1, r4
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _0804C146
	mov r0, #0x92
	cmp r7, #0
	beq _0804C124
	ldr r0, _0804C130 @ =0x00008092
_0804C124:
	lsr r1, r3, #0x1D
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _0804C166
_0804C130: .4byte 0x00008092
_0804C134:
	mov r0, #0x92
	cmp r7, #0
	beq _0804C13C
	ldr r0, _0804C1FC @ =0x00008092
_0804C13C:
	lsr r1, r3, #0x1D
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0804C146:
	mov r0, #0
	cmp r0, #0
	bne _0804C166
	mov r2, #0x35
	cmp r7, #0
	beq _0804C154
	ldr r2, _0804C200 @ =0x00008035
_0804C154:
	ldr r0, _0804C204 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0804C166:
	mov r4, #5
	mov r0, #1
	sub r6, r0, r7
	add r5, r6, #0
	and r5, r0
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	lsl r0, r7, #0x18
	lsr r7, r0, #0x18
_0804C17A:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0804C208 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _0804C20C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0804C1D8
	mov r0, #2
	ldrb r3, [r1, #6]
	and r0, r3
	cmp r0, #0
	beq _0804C1D8
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0804C1D8
	ldr r0, _0804C210 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0804C214 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0804C218 @ =0x0000044B
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804C1D8
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	mov r2, r8
	orr r1, r2
	ldr r0, _0804C204 @ =0x02018450
	ldrh r0, [r0]
	lsl r2, r0, #0x17
	lsr r2, r2, #0x1D
	lsl r2, r2, #8
	orr r2, r7
	add r0, r6, #0
	mov r3, #2
	bl sub_08017AB4
_0804C1D8:
	add r4, #1
	cmp r4, #9
	ble _0804C17A
	ldr r2, _0804C21C @ =0x020192E0
	ldr r3, _0804C220 @ =0x00001B16
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C224 @ =0xFFFFFE01
	and r0, r3
	bl _0804CB10 @ far jump
	.align 2, 0
_0804C1FC: .4byte 0x00008092
_0804C200: .4byte 0x00008035
_0804C204: .4byte 0x02018450
_0804C208: .4byte 0x00000D64
_0804C20C: .4byte 0x0201930C
_0804C210: .4byte 0x000007FF
_0804C214: .4byte gUnk_08622AB4
_0804C218: .4byte 0x0000044B
_0804C21C: .4byte 0x020192E0
_0804C220: .4byte 0x00001B16
_0804C224: .4byte 0xFFFFFE01
_0804C228:
	add r0, r7, #0
	mov r1, #0
	bl sub_0801D264
	ldr r4, _0804C29C @ =0x02018450
	mov r9, r4
	mov r5, #2
	add r0, r5, #0
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _0804C242
	b _0804C354
_0804C242:
	mov r0, #1
	sub r4, r0, r7
	add r2, r4, #0
	and r2, r0
	mov r0, r9
	ldrb r0, [r0, #1]
	lsl r3, r0, #0x1C
	lsr r0, r3, #0x1D
	mov r6, #0x94
	add r1, r0, #0
	mul r1, r6
	ldr r0, _0804C2A0 @ =0x00000D64
	mul r2, r0
	add r1, r1, r2
	ldr r0, _0804C2A4 @ =0x0201930C
	mov r8, r0
	add r1, r8
	add r0, r5, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804C354
	lsr r0, r3, #0x1D
	mul r0, r6
	add r0, r0, r2
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r5, _0804C2A8 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _0804C2AC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804C2B0 @ =0x000004DB
	cmp r1, r0
	beq _0804C2F0
	cmp r1, r0
	ble _0804C2B8
	ldr r0, _0804C2B4 @ =0x000005F2
	cmp r1, r0
	beq _0804C334
	b _0804C354
	.align 2, 0
_0804C29C: .4byte 0x02018450
_0804C2A0: .4byte 0x00000D64
_0804C2A4: .4byte 0x0201930C
_0804C2A8: .4byte 0x000007FF
_0804C2AC: .4byte gUnk_08622AB4
_0804C2B0: .4byte 0x000004DB
_0804C2B4: .4byte 0x000005F2
_0804C2B8:
	mov r0, #0xBA
	lsl r0, r0, #1
	cmp r1, r0
	bgt _0804C354
	sub r0, #2
	cmp r1, r0
	blt _0804C354
	lsr r0, r3, #0x1D
	add r1, r0, #0
	mul r1, r6
	add r1, r1, r2
	add r1, r8
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _0804C354
	ldr r0, _0804C2E8 @ =0x00001AEA
	add r0, r8
	ldr r1, _0804C2EC @ =0xFFFFFE01
	ldrh r2, [r0]
	and r1, r2
	mov r2, #0x14
	b _0804C8C8
_0804C2E8: .4byte 0x00001AEA
_0804C2EC: .4byte 0xFFFFFE01
_0804C2F0:
	add r0, r5, #0
	mov r3, r9
	ldrh r3, [r3, #2]
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _0804C328 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804C354
	add r0, r4, #0
	bl sub_08008A1C
	cmp r0, #0
	ble _0804C354
	add r0, r7, #0
	bl sub_08008860
	cmp r0, #1
	ble _0804C354
	ldr r0, _0804C32C @ =0x00001AEA
	add r0, r8
	ldr r1, _0804C330 @ =0xFFFFFE01
	ldrh r3, [r0]
	and r1, r3
	mov r2, #0x3C
	b _0804C8C8
	.align 2, 0
_0804C328: .4byte gUnk_08622AB4
_0804C32C: .4byte 0x00001AEA
_0804C330: .4byte 0xFFFFFE01
_0804C334:
	add r0, r4, #0
	bl sub_08008860
	cmp r0, #1
	ble _0804C354
	ldr r0, _0804C34C @ =0x00001AEA
	add r0, r8
	ldr r1, _0804C350 @ =0xFFFFFE01
	ldrh r4, [r0]
	and r1, r4
	mov r2, #0x28
	b _0804C8C8
_0804C34C: .4byte 0x00001AEA
_0804C350: .4byte 0xFFFFFE01
_0804C354:
	ldr r2, _0804C3A0 @ =0x020192E0
	ldr r0, _0804C3A4 @ =0x00001B16
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C3A8 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0804C370:
	mov r3, #0x30
	cmp r7, #0
	beq _0804C378
	ldr r3, _0804C3AC @ =0x00008030
_0804C378:
	ldr r0, _0804C3B0 @ =0x02018450
	ldrh r1, [r0, #0xA]
	ldrh r2, [r0, #0x16]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r2, _0804C3A0 @ =0x020192E0
	ldr r1, _0804C3A4 @ =0x00001B16
	add r2, r2, r1
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C3A8 @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
_0804C3A0: .4byte 0x020192E0
_0804C3A4: .4byte 0x00001B16
_0804C3A8: .4byte 0xFFFFFE01
_0804C3AC: .4byte 0x00008030
_0804C3B0: .4byte 0x02018450
_0804C3B4:
	mov r2, #0x31
	mov r8, r2
	cmp r7, #0
	beq _0804C3C0
	ldr r3, _0804C3F0 @ =0x00008031
	mov r8, r3
_0804C3C0:
	ldr r2, _0804C3F4 @ =0x02018450
	ldrh r5, [r2, #0x10]
	ldrh r6, [r2, #0x1C]
	ldrb r0, [r2, #0x14]
	lsl r1, r0, #0x1B
	lsr r3, r1, #0x1F
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1F
	lsl r0, r0, #1
	orr r3, r0
	ldrb r0, [r2, #8]
	lsl r1, r0, #0x1B
	lsr r1, r1, #0x1F
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1F
	lsl r0, r0, #1
	orr r1, r0
	ldrh r0, [r2, #0x12]
	cmp r0, #0
	beq _0804C3F8
	mov r4, #4
	orr r4, r1
	b _0804C3FA
	.align 2, 0
_0804C3F0: .4byte 0x00008031
_0804C3F4: .4byte 0x02018450
_0804C3F8:
	add r4, r1, #0
_0804C3FA:
	ldrh r0, [r2, #0x1E]
	cmp r0, #0
	beq _0804C404
	mov r0, #4
	orr r3, r0
_0804C404:
	lsl r3, r3, #8
	orr r3, r4
	mov r0, r8
	add r1, r5, #0
	add r2, r6, #0
	bl sub_0801EC58
	ldr r2, _0804C42C @ =0x020192E0
	ldr r4, _0804C430 @ =0x00001B16
	add r2, r2, r4
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C434 @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
_0804C42C: .4byte 0x020192E0
_0804C430: .4byte 0x00001B16
_0804C434: .4byte 0xFFFFFE01
_0804C438:
	mov r4, #0
	ldr r5, _0804C474 @ =0x02018450
_0804C43C:
	ldrb r1, [r5, #8]
	lsl r0, r1, #0x1C
	cmp r0, #0
	bge _0804C458
	mov r0, #0x78
	cmp r4, #0
	beq _0804C44C
	ldr r0, _0804C478 @ =0x00008078
_0804C44C:
	lsl r1, r1, #0x1D
	lsr r1, r1, #0x1D
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0804C458:
	add r5, #0xC
	add r4, #1
	cmp r4, #1
	ble _0804C43C
	mov r0, #0x12
	cmp r7, #0
	beq _0804C468
	ldr r0, _0804C47C @ =0x00008012
_0804C468:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _0804CB40
_0804C474: .4byte 0x02018450
_0804C478: .4byte 0x00008078
_0804C47C: .4byte 0x00008012
_0804C480:
	cmp r7, #0
	beq _0804C508
	ldr r1, _0804C4E8 @ =0x08085A48
	mov r0, #1
	sub r3, r0, r7
	and r3, r0
	ldr r0, _0804C4EC @ =0x02018450
	ldrb r0, [r0, #1]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	mov r2, #0x94
	mul r0, r2
	ldr r2, _0804C4F0 @ =0x00000D64
	mul r2, r3
	add r0, r0, r2
	ldr r4, _0804C4F4 @ =0x0201930C
	add r0, r0, r4
	ldr r2, [r0]
	lsl r2, r2, #0x14
	lsr r2, r2, #0xE
	ldr r7, _0804C4F8 @ =0x0822C720
	add r2, r2, r7
	mov r0, sp
	bl sub_080753F4
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804C4FC @ =0x00000B16
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r0, _0804C500 @ =0x00001AEA
	add r4, r4, r0
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C504 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	b _0804C582
	.align 2, 0
_0804C4E8: .4byte gUnk_08085A48
_0804C4EC: .4byte 0x02018450
_0804C4F0: .4byte 0x00000D64
_0804C4F4: .4byte 0x0201930C
_0804C4F8: .4byte gUnk_0822C720
_0804C4FC: .4byte 0x00000B16
_0804C500: .4byte 0x00001AEA
_0804C504: .4byte 0xFFFFFE01
_0804C508:
	ldr r1, _0804C538 @ =0x02015EE8
	mov r2, #1
	add r0, r2, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	bne _0804C54C
	ldr r0, _0804C53C @ =0x0201AE60
	strh r2, [r0, #0x14]
	ldr r2, _0804C540 @ =0x020192E0
	ldr r1, _0804C544 @ =0x00001B16
	add r2, r2, r1
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C548 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0804C582
_0804C538: .4byte 0x02015EE8
_0804C53C: .4byte 0x0201AE60
_0804C540: .4byte 0x020192E0
_0804C544: .4byte 0x00001B16
_0804C548: .4byte 0xFFFFFE01
_0804C54C:
	ldr r0, _0804C59C @ =0x0000F057
	ldr r1, _0804C5A0 @ =0x02018450
	ldrb r1, [r1, #1]
	lsl r1, r1, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0x94
	mul r1, r2
	ldr r2, _0804C5A4 @ =0x00000D64
	add r1, r1, r2
	ldr r2, _0804C5A8 @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _0804C5AC @ =0x02017FB0
	mov r3, #0x8A
	lsl r3, r3, #3
	add r1, r1, r3
	mov r0, #3
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_0804C582:
	ldr r2, _0804C5B0 @ =0x020192E0
	ldr r7, _0804C5B4 @ =0x00001B16
	add r2, r2, r7
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C5B8 @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
_0804C59C: .4byte 0x0000F057
_0804C5A0: .4byte 0x02018450
_0804C5A4: .4byte 0x00000D64
_0804C5A8: .4byte 0x0201930C
_0804C5AC: .4byte 0x02017FB0
_0804C5B0: .4byte 0x020192E0
_0804C5B4: .4byte 0x00001B16
_0804C5B8: .4byte 0xFFFFFE01
_0804C5BC:
	ldr r2, _0804C5E0 @ =0x02017FB0
	mov r1, #0x8A
	lsl r1, r1, #3
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	blt _0804C5CE
	b _0804C720
_0804C5CE:
	ldr r1, _0804C5E4 @ =0x0201AE60
	ldr r3, _0804C5E8 @ =0x0000045A
	add r0, r2, r3
	ldrh r0, [r0]
	strh r0, [r1, #0x14]
	ldr r3, _0804C5EC @ =0x00001B16
	add r3, sl
	b _0804CA3E
	.align 2, 0
_0804C5E0: .4byte 0x02017FB0
_0804C5E4: .4byte 0x0201AE60
_0804C5E8: .4byte 0x0000045A
_0804C5EC: .4byte 0x00001B16
_0804C5F0:
	ldr r0, _0804C658 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0804C650
	mov r1, #1
	sub r0, r1, r7
	add r3, r0, #0
	and r3, r1
	ldr r4, _0804C65C @ =0x02018450
	ldrb r2, [r4, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0x94
	mul r1, r2
	ldr r2, _0804C660 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _0804C664 @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_080197C0
	mov r0, #0x92
	cmp r7, #1
	beq _0804C628
	ldr r0, _0804C668 @ =0x00008092
_0804C628:
	ldrb r4, [r4, #1]
	lsl r1, r4, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x3A
	cmp r7, #0
	beq _0804C63E
	ldr r0, _0804C66C @ =0x0000803A
_0804C63E:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r7, #0
	mov r1, #1
	bl sub_0801D264
_0804C650:
	ldr r0, _0804C670 @ =0x020192E0
	ldr r3, _0804C674 @ =0x00001B16
	add r0, r0, r3
	b _0804C8C0
_0804C658: .4byte 0x0201AE60
_0804C65C: .4byte 0x02018450
_0804C660: .4byte 0x00000D64
_0804C664: .4byte 0x0201930C
_0804C668: .4byte 0x00008092
_0804C66C: .4byte 0x0000803A
_0804C670: .4byte 0x020192E0
_0804C674: .4byte 0x00001B16
_0804C678:
	mov r0, #1
	sub r0, r0, r7
	ldr r1, _0804C6A8 @ =0x02018450
	ldrb r1, [r1, #1]
	lsl r2, r1, #0x1C
	lsr r2, r2, #0x1D
	mov r1, #0x14
	mov r3, #0
	bl sub_08022678
	ldr r2, _0804C6AC @ =0x020192E0
	ldr r7, _0804C6B0 @ =0x00001B16
	add r2, r2, r7
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C6B4 @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
	.align 2, 0
_0804C6A8: .4byte 0x02018450
_0804C6AC: .4byte 0x020192E0
_0804C6B0: .4byte 0x00001B16
_0804C6B4: .4byte 0xFFFFFE01
_0804C6B8:
	mov r0, #8
	cmp r7, #1
	beq _0804C6C0
	ldr r0, _0804C724 @ =0x00008008
_0804C6C0:
	mov r1, #1
	sub r1, r1, r7
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r4, _0804C728 @ =0x00001B64
	add r4, sl
	ldrb r3, [r4]
	lsl r2, r3, #8
	mov r3, #0
	bl sub_0801EC58
	mov r2, #0x38
	cmp r7, #1
	beq _0804C6DE
	ldr r2, _0804C72C @ =0x00008038
_0804C6DE:
	mov r1, #1
	sub r1, r1, r7
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r4, [r4]
	lsl r0, r4, #8
	orr r1, r0
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r3, _0804C730 @ =0x00001B14
	add r3, sl
	ldr r2, [r3]
	lsl r0, r2, #0xF
	lsr r0, r0, #0x18
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #9
	ldr r1, _0804C734 @ =0xFFFE01FF
	and r1, r2
	orr r1, r0
	str r1, [r3]
	ldr r1, _0804C738 @ =0x00001B16
	add r1, sl
	ldr r0, _0804C73C @ =0xFFFFFE01
	ldrh r4, [r1]
	and r0, r4
	strh r0, [r1]
_0804C720:
	mov r0, #0
	b _0804CB42
_0804C724: .4byte 0x00008008
_0804C728: .4byte 0x00001B64
_0804C72C: .4byte 0x00008038
_0804C730: .4byte 0x00001B14
_0804C734: .4byte 0xFFFE01FF
_0804C738: .4byte 0x00001B16
_0804C73C: .4byte 0xFFFFFE01
_0804C740:
	cmp r7, #0
	beq _0804C7CC
	add r5, sp, #0x100
	ldr r1, _0804C7A8 @ =0x08085ADC
	mov r0, #1
	sub r3, r0, r7
	and r3, r0
	ldr r0, _0804C7AC @ =0x02018450
	ldrb r0, [r0, #1]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	mov r2, #0x94
	mul r0, r2
	ldr r2, _0804C7B0 @ =0x00000D64
	mul r2, r3
	add r0, r0, r2
	ldr r4, _0804C7B4 @ =0x0201930C
	add r0, r0, r4
	ldr r2, [r0]
	lsl r2, r2, #0x14
	lsr r2, r2, #0xE
	ldr r7, _0804C7B8 @ =0x0822C720
	add r2, r2, r7
	add r0, r5, #0
	bl sub_080753F4
	ldr r0, _0804C7BC @ =0x00000206
	ldr r1, _0804C7C0 @ =0x00000713
	mov r2, #0xB
	add r3, r5, #0
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r0, _0804C7C4 @ =0x00001AEA
	add r4, r4, r0
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C7C8 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	b _0804C846
	.align 2, 0
_0804C7A8: .4byte gUnk_08085ADC
_0804C7AC: .4byte 0x02018450
_0804C7B0: .4byte 0x00000D64
_0804C7B4: .4byte 0x0201930C
_0804C7B8: .4byte gUnk_0822C720
_0804C7BC: .4byte 0x00000206
_0804C7C0: .4byte 0x00000713
_0804C7C4: .4byte 0x00001AEA
_0804C7C8: .4byte 0xFFFFFE01
_0804C7CC:
	ldr r1, _0804C7FC @ =0x02015EE8
	mov r2, #1
	add r0, r2, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	bne _0804C810
	ldr r0, _0804C800 @ =0x0201AE60
	strh r2, [r0, #0x14]
	ldr r2, _0804C804 @ =0x020192E0
	ldr r1, _0804C808 @ =0x00001B16
	add r2, r2, r1
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C80C @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0804C846
_0804C7FC: .4byte 0x02015EE8
_0804C800: .4byte 0x0201AE60
_0804C804: .4byte 0x020192E0
_0804C808: .4byte 0x00001B16
_0804C80C: .4byte 0xFFFFFE01
_0804C810:
	ldr r0, _0804C860 @ =0x0000F057
	ldr r1, _0804C864 @ =0x02018450
	ldrb r1, [r1, #1]
	lsl r1, r1, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0x94
	mul r1, r2
	ldr r2, _0804C868 @ =0x00000D64
	add r1, r1, r2
	ldr r2, _0804C86C @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _0804C870 @ =0x02017FB0
	mov r3, #0x8A
	lsl r3, r3, #3
	add r1, r1, r3
	mov r0, #3
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_0804C846:
	ldr r2, _0804C874 @ =0x020192E0
	ldr r7, _0804C878 @ =0x00001B16
	add r2, r2, r7
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C87C @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
_0804C860: .4byte 0x0000F057
_0804C864: .4byte 0x02018450
_0804C868: .4byte 0x00000D64
_0804C86C: .4byte 0x0201930C
_0804C870: .4byte 0x02017FB0
_0804C874: .4byte 0x020192E0
_0804C878: .4byte 0x00001B16
_0804C87C: .4byte 0xFFFFFE01
_0804C880:
	ldr r2, _0804C8A4 @ =0x02017FB0
	mov r1, #0x8A
	lsl r1, r1, #3
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	blt _0804C892
	b _0804C720
_0804C892:
	ldr r1, _0804C8A8 @ =0x0201AE60
	ldr r3, _0804C8AC @ =0x0000045A
	add r0, r2, r3
	ldrh r0, [r0]
	strh r0, [r1, #0x14]
	ldr r3, _0804C8B0 @ =0x00001B16
	add r3, sl
	b _0804CA3E
	.align 2, 0
_0804C8A4: .4byte 0x02017FB0
_0804C8A8: .4byte 0x0201AE60
_0804C8AC: .4byte 0x0000045A
_0804C8B0: .4byte 0x00001B16
_0804C8B4:
	ldr r0, _0804C8D0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0804C8DC
	ldr r0, _0804C8D4 @ =0x00001B16
	add r0, sl
_0804C8C0:
	ldr r1, _0804C8D8 @ =0xFFFFFE01
	ldrh r4, [r0]
	and r1, r4
	mov r2, #4
_0804C8C8:
	orr r1, r2
	strh r1, [r0]
	b _0804C720
	.align 2, 0
_0804C8D0: .4byte 0x0201AE60
_0804C8D4: .4byte 0x00001B16
_0804C8D8: .4byte 0xFFFFFE01
_0804C8DC:
	mov r0, #1
	sub r0, r0, r7
	ldr r1, _0804C90C @ =0x02018450
	ldrh r1, [r1]
	lsl r2, r1, #0x17
	lsr r2, r2, #0x1D
	mov r1, #0x10
	mov r3, #0
	bl sub_08022678
	ldr r2, _0804C910 @ =0x020192E0
	ldr r7, _0804C914 @ =0x00001B16
	add r2, r2, r7
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804C918 @ =0xFFFFFE01
	and r0, r3
	b _0804CB10
	.align 2, 0
_0804C90C: .4byte 0x02018450
_0804C910: .4byte 0x020192E0
_0804C914: .4byte 0x00001B16
_0804C918: .4byte 0xFFFFFE01
_0804C91C:
	mov r0, #8
	cmp r7, #0
	beq _0804C924
	ldr r0, _0804C9C4 @ =0x00008008
_0804C924:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	ldr r2, _0804C9C8 @ =0x00001B64
	add r2, sl
	mov r8, r2
	ldrb r3, [r2]
	lsl r2, r3, #8
	mov r3, #0
	bl sub_0801EC58
	mov r4, #1
	sub r0, r4, r7
	mov r9, r0
	bl sub_08008A44
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	mov r1, #0x7F
	and r2, r1
	ldr r6, _0804C9CC @ =0x00001B17
	add r6, sl
	lsl r2, r2, #1
	add r3, r4, #0
	ldrb r1, [r6]
	and r3, r1
	orr r3, r2
	strb r3, [r6]
	lsr r0, r0, #0x17
	mov r2, #1
	mov ip, r2
	ldr r5, _0804C9D0 @ =0x00001B18
	add r5, sl
	and r0, r4
	mov r1, #2
	neg r1, r1
	ldrb r2, [r5]
	and r1, r2
	orr r1, r0
	strb r1, [r5]
	lsl r1, r7, #0x18
	lsr r1, r1, #0x18
	mov r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #8
	orr r1, r0
	mov r0, ip
	sub r2, r0, r7
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsr r3, r3, #1
	add r0, r4, #0
	ldrb r7, [r5]
	and r0, r7
	lsl r0, r0, #7
	orr r0, r3
	lsl r0, r0, #8
	orr r2, r0
	mov r0, r9
	bl sub_08019078
	ldr r1, _0804C9D4 @ =0x02018450
	ldrb r6, [r6]
	lsr r0, r6, #1
	ldrb r5, [r5]
	and r4, r5
	lsl r4, r4, #7
	orr r4, r0
	mov r0, #7
	and r4, r0
	lsl r4, r4, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r2, [r1, #1]
	and r0, r2
	orr r0, r4
	strb r0, [r1, #1]
	ldr r3, _0804C9D8 @ =0x00001B16
	add r3, sl
	b _0804CA3E
	.align 2, 0
_0804C9C4: .4byte 0x00008008
_0804C9C8: .4byte 0x00001B64
_0804C9CC: .4byte 0x00001B17
_0804C9D0: .4byte 0x00001B18
_0804C9D4: .4byte 0x02018450
_0804C9D8: .4byte 0x00001B16
_0804C9DC:
	mov r0, #1
	sub r4, r0, r7
	add r2, r4, #0
	and r2, r0
	ldr r3, _0804CA58 @ =0x02018450
	mov r9, r3
	ldrb r0, [r3, #1]
	lsl r3, r0, #0x1C
	lsr r0, r3, #0x1D
	mov r1, #0x94
	mov r8, r1
	mov r1, r8
	mul r1, r0
	ldr r0, _0804CA5C @ =0x00000D64
	add r5, r2, #0
	mul r5, r0
	add r1, r1, r5
	ldr r6, _0804CA60 @ =0x0201930C
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804CA3A
	lsr r1, r3, #0x1D
	add r0, r4, #0
	mov r2, #0
	bl sub_08018DC8
	mov r2, r9
	ldrb r2, [r2, #1]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1D
	mov r3, r8
	mul r3, r0
	add r0, r3, #0
	add r0, r0, r5
	add r0, r0, r6
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r7, #0
	bl sub_08019840
	add r0, r4, #0
	bl sub_080467B0
_0804CA3A:
	ldr r4, _0804CA64 @ =0x00001AEA
	add r3, r6, r4
_0804CA3E:
	ldrh r2, [r3]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804CA68 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0804C720
	.align 2, 0
_0804CA58: .4byte 0x02018450
_0804CA5C: .4byte 0x00000D64
_0804CA60: .4byte 0x0201930C
_0804CA64: .4byte 0x00001AEA
_0804CA68: .4byte 0xFFFFFE01
_0804CA6C:
	ldr r5, _0804CB18 @ =0x02018450
	mov r0, #1
	sub r2, r0, r7
	and r2, r0
	ldrb r1, [r5, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804CB1C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0804CB20 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r5, #2]
	ldr r1, _0804CB24 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0804CB28 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	mov r1, #1
	bl sub_08007590
	mov r1, #1
	and r0, r1
	lsl r0, r0, #2
	mov r6, #5
	neg r6, r6
	add r1, r6, #0
	ldrb r3, [r5]
	and r1, r3
	orr r1, r0
	strb r1, [r5]
	ldr r4, _0804CB2C @ =0x000005FA
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _0804CAD0
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _0804CAD8
_0804CAD0:
	add r0, r6, #0
	ldrb r4, [r5]
	and r0, r4
	strb r0, [r5]
_0804CAD8:
	mov r0, #1
	sub r0, r0, r7
	ldr r1, _0804CB30 @ =0x086247AA
	ldrh r1, [r1]
	mov r2, #1
	sub r2, r2, r7
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldr r3, _0804CB18 @ =0x02018450
	ldrb r3, [r3, #1]
	lsl r3, r3, #0x1C
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #3
	bl sub_08017AB4
	add r0, r7, #0
	mov r1, #0
	bl sub_0801D264
	ldr r2, _0804CB34 @ =0x020192E0
	ldr r7, _0804CB38 @ =0x00001B16
	add r2, r2, r7
	ldr r0, _0804CB3C @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #4
_0804CB10:
	orr r0, r1
	strh r0, [r2]
	b _0804C720
	.align 2, 0
_0804CB18: .4byte 0x02018450
_0804CB1C: .4byte 0x00000D64
_0804CB20: .4byte 0x0201930C
_0804CB24: .4byte 0x000007FF
_0804CB28: .4byte gUnk_08622AB4
_0804CB2C: .4byte 0x000005FA
_0804CB30: .4byte gUnk_086247AA
_0804CB34: .4byte 0x020192E0
_0804CB38: .4byte 0x00001B16
_0804CB3C: .4byte 0xFFFFFE01
_0804CB40:
	mov r0, #1
_0804CB42:
	mov r3, #0x80
	lsl r3, r3, #2
	add sp, r3
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804BFF0
	.align 2, 0

