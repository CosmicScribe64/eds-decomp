	thumb_func_start sub_0800C8BC
sub_0800C8BC: @ 0x0800C8BC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #0]
	mov r8, r1
	mov r0, #1
	ldr r1, [sp, #0]
	and r0, r1
	ldr r5, _0800C8F8 @ =0x00000D64
	add r4, r0, #0
	mul r4, r5
	ldr r1, _0800C8FC @ =0x0201930C
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	mov r3, #0
	mov sl, r3
	add r0, r0, r4
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	bne _0800C900
	mov r0, #0
	b _0800CAB2
_0800C8F8: .4byte 0x00000D64
_0800C8FC: .4byte 0x0201930C
_0800C900:
	ldr r0, _0800CAC4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0800CAC8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r7, r0, #0x14
	mov r2, r8
	cmp r2, #4
	ble _0800C91C
	b _0800CAB0
_0800C91C:
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	bne _0800C928
	b _0800CAB0
_0800C928:
	mov r6, #0
	mov ip, r5
	str r4, [sp, #4]
	mov r3, #0x94
	mov r9, r3
_0800C932:
	mov r0, r9
	mul r0, r6
	ldr r4, [sp, #4]
	add r0, r0, r4
	ldr r1, _0800CACC @ =0x0201930C
	add r2, r0, r1
	ldr r3, [r2]
	lsl r0, r3, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0800C976
	ldr r0, _0800CAC4 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r4, _0800CAD0 @ =0x08622AB4
	add r0, r0, r4
	ldrh r0, [r0]
	ldr r1, _0800CAD4 @ =0x000002FA
	cmp r0, r1
	bne _0800C976
	lsl r0, r3, #0xE
	cmp r0, #0
	bge _0800C976
	mov r0, #2
	ldrb r3, [r2, #6]
	and r0, r3
	cmp r0, #0
	beq _0800C976
	ldrh r4, [r2, #4]
	cmp r4, sl
	bls _0800C976
	ldrh r2, [r2, #4]
	mov sl, r2
	mov r7, #0xA
_0800C976:
	mov r3, #0
	add r5, r6, #1
	mov r4, r9
	mul r4, r6
	ldr r6, _0800CAD8 @ =0x00000479
_0800C980:
	add r0, r3, #0
	mov r1, #1
	and r0, r1
	mov r2, ip
	mul r2, r0
	add r0, r2, #0
	add r0, r0, r4
	ldr r1, _0800CADC @ =0x020195F0
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0800C9D8
	ldr r0, _0800CAC4 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0800CAD0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _0800C9D8
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _0800C9D8
	add r1, r2, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800C9D8
	ldrh r0, [r2, #4]
	cmp r0, sl
	bls _0800C9D8
	ldrh r1, [r2, #4]
	mov sl, r1
	add r0, r2, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r7, r0, #0x1B
_0800C9D8:
	add r3, #1
	cmp r3, #1
	ble _0800C980
	add r6, r5, #0
	cmp r6, #4
	ble _0800C932
	mov r6, #0
	mov r2, #1
	ldr r3, [sp, #0]
	and r2, r3
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r3, _0800CAE0 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r1, r0
	ldr r4, _0800CACC @ =0x0201930C
	add r0, r0, r4
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	bge _0800CAB0
	mov r9, r2
	mov r8, r1
_0800CA0A:
	ldr r0, _0800CAE0 @ =0x00000D64
	mov r2, r9
	mul r2, r0
	add r2, r8
	ldr r1, _0800CACC @ =0x0201930C
	add r2, r2, r1
	lsl r0, r6, #1
	add r1, r2, #0
	add r1, #0xA
	add r1, r1, r0
	add r2, #0x4A
	add r2, r2, r0
	ldrh r4, [r1]
	lsr r3, r4, #8
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	mov r4, #0x94
	add r1, r3, #0
	mul r1, r4
	ldr r3, _0800CAE0 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r4, _0800CACC @ =0x0201930C
	add r5, r1, r4
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrb r2, [r2]
	cmp r2, #1
	bne _0800CA9A
	cmp r4, #0
	beq _0800CA9A
	add r1, r5, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800CA9A
	mov r0, #0
	ldr r1, _0800CAE4 @ =0x00000601
	bl sub_08008524
	cmp r0, #0
	bne _0800CA9A
	mov r0, #1
	ldr r1, _0800CAE4 @ =0x00000601
	bl sub_08008524
	cmp r0, #0
	bne _0800CA9A
	mov r0, #3
	ldr r1, _0800CAE8 @ =0x0201ADAD
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800CA9A
	ldr r2, _0800CAC4 @ =0x000007FF
	add r0, r2, #0
	and r4, r0
	lsl r0, r4, #1
	ldr r3, _0800CAD0 @ =0x08622AB4
	add r0, r0, r3
	ldr r1, _0800CAEC @ =0x0000060E
	ldrh r0, [r0]
	cmp r0, r1
	bne _0800CA9A
	ldrh r5, [r5, #4]
	cmp r5, sl
	bls _0800CA9A
	mov r7, #1
_0800CA9A:
	add r6, #1
	ldr r3, _0800CAE0 @ =0x00000D64
	mov r0, r9
	mul r0, r3
	add r0, r8
	ldr r4, _0800CACC @ =0x0201930C
	add r0, r0, r4
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	blt _0800CA0A
_0800CAB0:
	add r0, r7, #0
_0800CAB2:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800CAC4: .4byte 0x000007FF
_0800CAC8: .4byte gUnk_08621DE0
_0800CACC: .4byte 0x0201930C
_0800CAD0: .4byte gUnk_08622AB4
_0800CAD4: .4byte 0x000002FA
_0800CAD8: .4byte 0x00000479
_0800CADC: .4byte 0x020195F0
_0800CAE0: .4byte 0x00000D64
_0800CAE4: .4byte 0x00000601
_0800CAE8: .4byte 0x0201ADAD
_0800CAEC: .4byte 0x0000060E
	thumb_func_end sub_0800C8BC

