	thumb_func_start sub_08038D48
sub_08038D48: @ 0x08038D48
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08038DFE
	mov r7, #0
	mov r2, #0
	mov r8, r2
	mov r4, #0
	mov r5, #1
_08038D64:
	bl sub_08076F9C
	and r0, r5
	cmp r0, #0
	bne _08038D74
	mov r3, #1
	add r8, r3
	b _08038D7A
_08038D74:
	add r0, r5, #0
	lsl r0, r4
	orr r7, r0
_08038D7A:
	add r4, #1
	cmp r4, #2
	ble _08038D64
	mov r4, #1
	add r0, r4, #0
	ldrb r1, [r6, #2]
	and r0, r1
	mov r2, #0xE1
	cmp r0, #0
	beq _08038D90
	ldr r2, _08038E0C @ =0x000080E1
_08038D90:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r4, #0
	ldrb r2, [r6, #2]
	and r0, r2
	mov r1, #0x12
	cmp r0, #0
	beq _08038DAC
	ldr r1, _08038E10 @ =0x00008012
_08038DAC:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r3, r8
	cmp r3, #1
	ble _08038DE0
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	ldrh r2, [r6, #0xC]
	lsr r1, r2, #8
	bl sub_08030028
	ldrb r3, [r6, #2]
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r4, r1
	ldrh r3, [r6, #0xC]
	lsr r2, r3, #8
	bl sub_08046CB0
_08038DE0:
	add r0, r4, #0
	ldrb r1, [r6, #2]
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _08038DEE
	ldr r2, _08038E14 @ =0x00008092
_08038DEE:
	ldrh r6, [r6, #2]
	lsl r1, r6, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08038DFE:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038E0C: .4byte 0x000080E1
_08038E10: .4byte 0x00008012
_08038E14: .4byte 0x00008092
	thumb_func_end sub_08038D48

