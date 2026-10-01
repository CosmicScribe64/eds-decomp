	thumb_func_start sub_08039D8C
sub_08039D8C: @ 0x08039D8C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _08039E1C
	mov r6, #7
	ldrb r0, [r7, #0xA]
	and r6, r0
	cmp r6, #1
	bne _08039E1C
	ldrh r1, [r7, #0xC]
	add r0, r7, #0
	bl sub_0802C334
	cmp r0, #0
	beq _08039E1C
	ldrb r4, [r7, #0xC]
	ldrh r0, [r7, #0xC]
	lsr r1, r0, #8
	add r0, r4, #0
	bl sub_0800C894
	mov r8, r0
	mov r5, #0
	sub r4, r6, r4
	and r6, r4
	ldr r0, _08039E28 @ =0x00000D64
	mul r6, r0
_08039DCC:
	mov r0, #0x94
	mul r0, r5
	add r0, r0, r6
	ldr r1, _08039E2C @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08039E16
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08039E16
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800C8A8
	cmp r0, r8
	bge _08039E16
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0802B28C
	cmp r0, #0
	beq _08039E16
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08030028
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	add r2, r5, #0
	bl sub_08046CB0
_08039E16:
	add r5, #1
	cmp r5, #4
	ble _08039DCC
_08039E1C:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08039E28: .4byte 0x00000D64
_08039E2C: .4byte 0x0201930C
	thumb_func_end sub_08039D8C

