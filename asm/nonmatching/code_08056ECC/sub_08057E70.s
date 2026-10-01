	thumb_func_start sub_08057E70
sub_08057E70: @ 0x08057E70
	push {r4, r5, r6, r7, lr}
	mov r5, #0
	ldr r7, _08057ED4 @ =0x0201A070
	sub r6, r7, #2
_08057E78:
	mov r0, #0x94
	mul r0, r5
	add r4, r0, r7
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08057EC6
	ldr r2, _08057ED8 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08057EDC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	bne _08057EC6
	mov r2, #2
	neg r2, r2
	add r0, r2, #0
	ldrb r1, [r4, #6]
	and r0, r1
	mov r1, #2
	orr r0, r1
	strb r0, [r4, #6]
	sub r2, #3
	add r0, r2, #0
	ldrb r1, [r4, #7]
	and r0, r1
	strb r0, [r4, #7]
	mov r0, #1
	lsl r0, r5
	ldrh r2, [r6]
	bic r2, r0
	add r0, r2, #0
	strh r0, [r6]
_08057EC6:
	add r5, #1
	cmp r5, #4
	ble _08057E78
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08057ED4: .4byte 0x0201A070
_08057ED8: .4byte 0x000007FF
_08057EDC: .4byte gUnk_08622AB4
	thumb_func_end sub_08057E70

