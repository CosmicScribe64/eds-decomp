	thumb_func_start sub_080269FC
sub_080269FC: @ 0x080269FC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x20
	ldr r5, _08026ACC @ =0x02020C28
	ldr r0, [r5, #4]
	ldrb r2, [r5, #0xC]
	ldrh r3, [r5, #8]
	ldrh r1, [r5, #0xA]
	str r1, [sp, #0]
	mov r6, #0
	str r6, [sp, #4]
	mov r1, #2
	mov r8, r1
	str r1, [sp, #8]
	str r6, [sp, #0xC]
	str r6, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	ldr r1, _08026AD0 @ =0xFFFFF6E8
	add r7, r5, r1
	str r7, [sp, #0x1C]
	mov r1, #1
	bl sub_08077EF4
	add r4, r5, #0
	add r4, #0x14
	add r0, r4, #0
	mov r1, #1
	add r2, r7, #0
	bl sub_08026220
	add r0, r5, #0
	bl sub_080786D0
	add r0, r4, #0
	bl sub_080786D0
	ldrb r2, [r5, #0xD]
	cmp r2, #1
	bne _08026A52
	mov r0, #0xFF
	strb r0, [r5, #0xE]
_08026A52:
	add r1, r5, #0
	add r1, #0x22
	mov r0, #1
	strb r0, [r1]
	ldr r0, _08026AD4 @ =0x0000020D
	add r2, r5, r0
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	mov r1, r8
	and r0, r1
	cmp r0, #0
	beq _08026A7A
	mov r0, #0x83
	lsl r0, r0, #2
	add r1, r5, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	strb r6, [r2]
_08026A7A:
	ldr r1, _08026AD8 @ =0x08087BA4
	mov r2, #0xFF
	lsl r2, r2, #1
	add r4, r5, r2
	ldrb r0, [r4]
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r1
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #0x80
	lsl r1, r1, #1
	sub r1, r1, r0
	mov r0, #0x90
	bl sub_0807B4D0
	add r0, #0x80
	ldr r2, _08026ADC @ =0xFFFFFD02
	add r1, r5, r2
	strh r0, [r1]
	sub r2, #2
	add r1, r5, r2
	strh r0, [r1]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	add r0, r1, #0
	bl sub_0807B5A0
	add r0, r7, #0
	bl sub_0807A298
	add r0, r7, #0
	bl sub_0807A2EC
	add sp, #0x20
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08026ACC: .4byte 0x02020C28
_08026AD0: .4byte 0xFFFFF6E8
_08026AD4: .4byte 0x0000020D
_08026AD8: .4byte gUnk_08087BA4
_08026ADC: .4byte 0xFFFFFD02
	thumb_func_end sub_080269FC

