	thumb_func_start CB_LinkBattle
CB_LinkBattle: @ 0x0801AD18
	push {r4, r5, r6, lr}
	ldr r6, _0801ADB0 @ =0x08198E7C
	ldr r4, _0801ADB4 @ =0x03000040
	ldr r0, _0801ADB8 @ =0x00004858
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r6
	ldr r0, [r0]
	cmp r0, #0
	beq _0801ADD8
	ldr r1, _0801ADBC @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0801AD3E
	bl DuelLink_PollMessage
_0801AD3E:
	ldrb r2, [r5]
	lsl r0, r2, #2
	add r0, r0, r6
	ldr r0, [r0]
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801AD76
	ldrb r0, [r5]
	add r0, #1
	mov r2, #0
	strb r0, [r5]
	ldr r3, _0801ADC0 @ =0x0000488A
	add r1, r4, r3
	ldr r0, _0801ADC4 @ =0xFFFFF00F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r1, _0801ADC8 @ =0x00004859
	add r0, r4, r1
	strb r2, [r0]
	ldr r3, _0801ADCC @ =0x0000485A
	add r0, r4, r3
	strb r2, [r0]
	add r1, #2
	add r0, r4, r1
	strb r2, [r0]
_0801AD76:
	ldr r1, _0801ADD0 @ =0x020192E0
	ldr r2, _0801ADD4 @ =0x00001B12
	add r1, r1, r2
	mov r0, #0x20
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801ADAA
	ldrb r0, [r5]
	sub r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #2
	bhi _0801ADAA
	mov r1, #0
	mov r0, #6
	strb r0, [r5]
	ldr r3, _0801ADC8 @ =0x00004859
	add r0, r4, r3
	strb r1, [r0]
	ldr r2, _0801ADCC @ =0x0000485A
	add r0, r4, r2
	strb r1, [r0]
	add r3, #2
	add r0, r4, r3
	strb r1, [r0]
_0801ADAA:
	mov r0, #0
	b _0801ADDA
	.align 2, 0
_0801ADB0: .4byte gLinkBattleSteps
_0801ADB4: .4byte 0x03000040
_0801ADB8: .4byte 0x00004858
_0801ADBC: .4byte 0x02015EE8
_0801ADC0: .4byte 0x0000488A
_0801ADC4: .4byte 0xFFFFF00F
_0801ADC8: .4byte 0x00004859
_0801ADCC: .4byte 0x0000485A
_0801ADD0: .4byte 0x020192E0
_0801ADD4: .4byte 0x00001B12
_0801ADD8:
	mov r0, #1
_0801ADDA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end CB_LinkBattle

