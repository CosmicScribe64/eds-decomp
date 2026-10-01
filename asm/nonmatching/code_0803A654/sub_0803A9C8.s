	thumb_func_start sub_0803A9C8
sub_0803A9C8: @ 0x0803A9C8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	mov r2, #7
	ldrb r0, [r0, #0xA]
	and r2, r0
	cmp r2, #1
	bne _0803AA72
	mov r1, r8
	ldrb r5, [r1, #0xC]
	ldrh r0, [r1, #0xC]
	lsr r7, r0, #8
	and r2, r5
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _0803AA58 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803AA5C @ =0x0201930C
	add r6, r1, r0
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r4, r0, #0
	mov r9, r4
	cmp r4, #0
	ble _0803AA72
	mov r0, #3
	ldrb r1, [r6, #6]
	and r0, r1
	cmp r0, #1
	bne _0803AA72
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #0
	bl sub_08018DC8
	add r0, r4, #0
	bl sub_08007730
	cmp r0, #0
	beq _0803AA60
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08019800
	mov r0, #0x40
	ldrb r2, [r6, #1]
	orr r0, r2
	strb r0, [r6, #1]
	add r0, r5, #0
	add r1, r7, #0
	bl sub_08030028
	mov r1, r8
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r7, #0
	bl sub_08046CB0
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r6, #1]
	and r0, r2
	strb r0, [r6, #1]
	b _0803AA72
	.align 2, 0
_0803AA58: .4byte 0x00000D64
_0803AA5C: .4byte 0x0201930C
_0803AA60:
	add r0, r5, #0
	mov r1, r9
	bl sub_08019840
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #0
	bl sub_08018DC8
_0803AA72:
	mov r0, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803A9C8

