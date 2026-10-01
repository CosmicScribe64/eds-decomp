	thumb_func_start sub_08054198
sub_08054198: @ 0x08054198
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r5, #1
	mov r7, #0
	ldr r2, _080541F0 @ =0x020192E4
	and r0, r5
	ldr r1, _080541F4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	lsr r4, r0, #0x1F
	sub r0, r5, r6
	ldr r1, _080541F8 @ =0x000005E7
	bl sub_08008524
	cmp r0, #0
	ble _080541CC
	cmp r4, #0
	bne _080541CC
	b _08054378
_080541CC:
	ldr r0, _080541FC @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08054200 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r1, _08054204 @ =0xFFFFFA16
	add r0, r0, r1
	cmp r0, #5
	bls _080541E4
	b _080542E0
_080541E4:
	lsl r0, r0, #2
	ldr r1, _08054208 @ =0x0805420C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080541F0: .4byte 0x020192E4
_080541F4: .4byte 0x00000D64
_080541F8: .4byte 0x000005E7
_080541FC: .4byte 0x000007FF
_08054200: .4byte gUnk_08622AB4
_08054204: .4byte 0xFFFFFA16
_08054208: .4byte 0x0805420C
_0805420C:
	.4byte _08054224
	.4byte _080542CC
	.4byte _080542D2
	.4byte _080542D6
	.4byte _080542DA
	.4byte _080542DE
_08054224:
	mov r5, #3
	cmp r4, #0
	beq _0805426C
	mov r4, #0
	mov r0, #1
	and r0, r6
	ldr r1, _08054264 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_08054236:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08054268 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0805425A
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C8BC
	cmp r0, #3
	bne _0805425A
	sub r5, #1
	cmp r5, #0
	beq _08054328
_0805425A:
	add r4, #1
	cmp r4, #4
	ble _08054236
	b _08054378
	.align 2, 0
_08054264: .4byte 0x00000D64
_08054268: .4byte 0x0201930C
_0805426C:
	mov r4, #0
	ldr r0, _080542B8 @ =0x020192E4
	mov r1, #1
	and r1, r6
	ldr r2, _080542BC @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r0, [r0, #4]
	cmp r4, r0
	bge _08054378
	ldr r6, _080542C0 @ =0x000007FF
	add r2, r1, #0
	add r3, r0, #0
_08054286:
	ldr r0, _080542C4 @ =0x02019BE8
	add r0, r2, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _080542C8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #3
	bne _080542AC
	sub r5, #1
	cmp r5, #0
	beq _08054328
_080542AC:
	add r2, #4
	add r4, #1
	cmp r4, r3
	blt _08054286
	b _08054378
	.align 2, 0
_080542B8: .4byte 0x020192E4
_080542BC: .4byte 0x00000D64
_080542C0: .4byte 0x000007FF
_080542C4: .4byte 0x02019BE8
_080542C8: .4byte gUnk_08621DE0
_080542CC:
	mov r5, #2
	mov r7, #1
	b _080542E0
_080542D2:
	mov r7, #4
	b _080542E0
_080542D6:
	mov r7, #3
	b _080542E0
_080542DA:
	mov r7, #5
	b _080542E0
_080542DE:
	mov r7, #6
_080542E0:
	cmp r4, #0
	beq _0805432C
	mov r4, #0
	mov r0, #1
	and r0, r6
	ldr r1, _08054320 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r8, r2
_080542F2:
	mov r0, #0x94
	mul r0, r4
	add r0, r8
	ldr r1, _08054324 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08054316
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800CAF0
	cmp r0, r7
	bne _08054316
	sub r5, #1
	cmp r5, #0
	beq _08054328
_08054316:
	add r4, #1
	cmp r4, #4
	ble _080542F2
	b _08054378
	.align 2, 0
_08054320: .4byte 0x00000D64
_08054324: .4byte 0x0201930C
_08054328:
	mov r0, #1
	b _0805437A
_0805432C:
	mov r4, #0
	ldr r0, _08054384 @ =0x020192E4
	mov r1, #1
	and r1, r6
	ldr r2, _08054388 @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r0, [r0, #4]
	cmp r4, r0
	bge _08054378
	ldr r6, _0805438C @ =0x000007FF
	add r3, r0, #0
_08054344:
	ldr r0, _08054390 @ =0x02019BE8
	add r0, r1, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _08054394 @ =0x08621DE0
	add r0, r0, r2
	ldr r2, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08054370
	lsr r0, r2, #0x1D
	cmp r0, r7
	bne _08054370
	sub r5, #1
	cmp r5, #0
	beq _08054328
_08054370:
	add r1, #4
	add r4, #1
	cmp r4, r3
	blt _08054344
_08054378:
	mov r0, #0
_0805437A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08054384: .4byte 0x020192E4
_08054388: .4byte 0x00000D64
_0805438C: .4byte 0x000007FF
_08054390: .4byte 0x02019BE8
_08054394: .4byte gUnk_08621DE0
	thumb_func_end sub_08054198

