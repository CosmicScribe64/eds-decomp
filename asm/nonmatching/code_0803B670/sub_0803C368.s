	thumb_func_start sub_0803C368
sub_0803C368: @ 0x0803C368
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C412
	mov r5, #7
	ldrb r0, [r4, #0xA]
	and r5, r0
	cmp r5, #1
	bne _0803C412
	ldrb r1, [r4, #0xC]
	mov r8, r1
	ldrh r0, [r4, #0xC]
	lsr r7, r0, #8
	and r1, r5
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	ldr r0, _0803C420 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803C424 @ =0x0201930C
	add r6, r2, r0
	mov r0, #2
	ldrb r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _0803C412
	ldr r0, [r6]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803C412
	ldrh r1, [r4, #6]
	ldrb r0, [r4, #6]
	mov r2, #0x35
	cmp r0, #0
	beq _0803C3BC
	ldr r2, _0803C428 @ =0x00008035
_0803C3BC:
	lsr r1, r1, #8
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803C3D6
	ldr r3, _0803C42C @ =0x00008008
_0803C3D6:
	ldrb r1, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r2, r0, #8
	lsl r2, r2, #8
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r6, [r6, #6]
	and r0, r6
	cmp r0, #0
	beq _0803C3FC
	mov r0, r8
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_0803C3FC:
	ldrb r1, [r4, #2]
	and r5, r1
	mov r0, #0x39
	cmp r5, #0
	beq _0803C408
	ldr r0, _0803C430 @ =0x00008039
_0803C408:
	ldrh r1, [r4, #0xC]
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0803C412:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803C420: .4byte 0x00000D64
_0803C424: .4byte 0x0201930C
_0803C428: .4byte 0x00008035
_0803C42C: .4byte 0x00008008
_0803C430: .4byte 0x00008039
	thumb_func_end sub_0803C368

