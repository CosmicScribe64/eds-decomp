	thumb_func_start sub_0806CDD4
sub_0806CDD4: @ 0x0806CDD4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	mov r4, #0
	str r4, [sp, #0x18]
	mov r1, #0xC0
	lsl r1, r1, #0x13
	ldr r2, _0806CF90 @ =0x01004000
	add r0, sp, #0x18
	bl CpuFastSet
	str r4, [sp, #0x1C]
	add r0, sp, #0x1C
	ldr r1, _0806CF94 @ =0x06010000
	ldr r2, _0806CF98 @ =0x01002000
	bl CpuFastSet
	mov r0, #0
	mov r7, #0
_0806CE00:
	mov r4, #0
	lsl r5, r0, #8
	add r6, r0, #1
_0806CE06:
	lsl r1, r4, #3
	add r1, r1, r5
	lsl r1, r1, #1
	ldr r0, _0806CF9C @ =0x0600F000
	add r1, r1, r0
	mov r0, #8
	str r0, [sp, #0]
	str r7, [sp, #4]
	str r7, [sp, #8]
	ldr r0, _0806CFA0 @ =0x08701B24
	mov r2, #8
	mov r3, #8
	bl sub_0807A9C0
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #3
	bls _0806CE06
	lsl r0, r6, #0x10
	lsr r0, r0, #0x10
	cmp r0, #3
	bls _0806CE00
	ldr r0, _0806CFA4 @ =0x08701BA4
	ldr r1, _0806CFA8 @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl sub_0807A908
	ldr r0, _0806CFAC @ =0x08702054
	ldr r5, _0806CFB0 @ =0x0600D000
	add r1, r5, #0
	mov r2, #0x1E
	mov r3, #0x14
	bl sub_0807A908
	ldr r0, _0806CFB4 @ =0x086FCA40
	ldr r7, _0806CFB8 @ =0x0600C000
	add r1, r7, #0
	mov r2, #0x1E
	mov r3, #0x14
	bl sub_0807A908
	ldr r1, _0806CFBC @ =0x08702504
	mov sl, r1
	ldr r2, _0806CFC0 @ =0x0201DB20
	ldr r0, _0806CFC4 @ =0x00001C1C
	add r4, r2, r0
	ldrb r1, [r4]
	lsl r2, r1, #2
	add r2, r2, r1
	str r5, [sp, #0]
	mov r0, #0x14
	mov r9, r0
	str r0, [sp, #4]
	mov r5, #0
	str r5, [sp, #8]
	mov r1, #7
	mov r8, r1
	str r1, [sp, #0xC]
	mov r6, #5
	str r6, [sp, #0x10]
	str r5, [sp, #0x14]
	mov r0, sl
	mov r1, #0
	mov r3, #7
	bl sub_0807ADE8
	ldrb r0, [r4]
	lsl r2, r0, #2
	add r2, r2, r0
	str r7, [sp, #0]
	mov r1, r9
	str r1, [sp, #4]
	str r5, [sp, #8]
	mov r0, r8
	str r0, [sp, #0xC]
	str r6, [sp, #0x10]
	str r5, [sp, #0x14]
	mov r0, sl
	mov r1, #0
	mov r3, #7
	bl sub_0807ADE8
	ldr r0, _0806CFC8 @ =0x086FDB24
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806CFCC @ =0x086FFB24
	ldr r1, _0806CFD0 @ =0x06002000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806CFD4 @ =0x086E5030
	ldr r1, _0806CF94 @ =0x06010000
	mov r2, #0x10
	bl sub_08077CEC
	ldr r0, _0806CFD8 @ =0x086FD924
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x80
	bl CpuFastSet
	ldr r0, _0806CFDC @ =0x086ED1B0
	ldr r1, _0806CFE0 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
	ldr r1, _0806CFE4 @ =0x04000008
	mov r2, #0xC0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806CFE8 @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806CFEC @ =0x00001C02
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806CFF0 @ =0x00001E02
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _0806CFF4 @ =0xFFFFFE80
	ldr r0, _0806CFC0 @ =0x0201DB20
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r0, r2
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	ldr r0, _0806CFF8 @ =0x04000010
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	add r0, #2
	strh r5, [r0]
	mov r0, #0x10
	ldr r1, _0806CFFC @ =0x0201F770
	strb r0, [r1]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0xD0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	bl sub_0806CB68
	ldr r2, _0806D000 @ =0x00000439
	ldr r0, _0806D004 @ =0x08623326
	ldrh r1, [r0]
	ldr r0, _0806D008 @ =0x00000776
	cmp r1, r0
	beq _0806CF7C
	cmp r1, r0
	blt _0806CF6C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	ble _0806CF7C
_0806CF6C:
	lsl r0, r2, #2
	ldr r1, _0806D00C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
_0806CF7C:
	mov r0, #1
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0806CF90: .4byte 0x01004000
_0806CF94: .4byte 0x06010000
_0806CF98: .4byte 0x01002000
_0806CF9C: .4byte 0x0600F000
_0806CFA0: .4byte gUnk_08701B24
_0806CFA4: .4byte gUnk_08701BA4
_0806CFA8: .4byte 0x0600E000
_0806CFAC: .4byte gUnk_08702054
_0806CFB0: .4byte 0x0600D000
_0806CFB4: .4byte gUnk_086FCA40
_0806CFB8: .4byte 0x0600C000
_0806CFBC: .4byte gUnk_08702504
_0806CFC0: .4byte 0x0201DB20
_0806CFC4: .4byte 0x00001C1C
_0806CFC8: .4byte gUnk_086FDB24
_0806CFCC: .4byte gUnk_086FFB24
_0806CFD0: .4byte 0x06002000
_0806CFD4: .4byte gUnk_086E5030
_0806CFD8: .4byte gUnk_086FD924
_0806CFDC: .4byte gUnk_086ED1B0
_0806CFE0: .4byte 0x05000200
_0806CFE4: .4byte 0x04000008
_0806CFE8: .4byte 0x00001A01
_0806CFEC: .4byte 0x00001C02
_0806CFF0: .4byte 0x00001E02
_0806CFF4: .4byte 0xFFFFFE80
_0806CFF8: .4byte 0x04000010
_0806CFFC: .4byte 0x0201F770
_0806D000: .4byte 0x00000439
_0806D004: .4byte gUnk_08623326
_0806D008: .4byte 0x00000776
_0806D00C: .4byte gUnk_08621DE0
	thumb_func_end sub_0806CDD4

