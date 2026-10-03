	thumb_func_start DiceScreen_Update
DiceScreen_Update: @ 0x08025F50
	push {r4, lr}
	ldr r4, _08025F6C @ =0x020202DC
	add r0, r4, #0
	bl FadeTick
	ldrb r0, [r4, #6]
	cmp r0, #2
	bne _08025F74
	ldr r1, _08025F70 @ =0x02017A30
	ldrb r0, [r4, #0xE]
	strh r0, [r1, #8]
	mov r0, #1
	b _08025FB6
	.align 2, 0
_08025F6C: .4byte 0x020202DC
_08025F70: .4byte 0x02017A30
_08025F74:
	add r0, r4, #0
	add r0, #0x2E
	ldrb r0, [r0]
	cmp r0, #2
	bne _08025F88
	ldr r1, _08025F84 @ =0x08199A28
	b _08025F8A
	.align 2, 0
_08025F84: .4byte gPlainDieScreenSteps
_08025F88:
	ldr r1, _08025FBC @ =0x08199A10
_08025F8A:
	ldrb r2, [r4, #8]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08025FA6
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08025FA6
	ldrb r0, [r4, #8]
	add r0, #1
	strb r0, [r4, #8]
_08025FA6:
	ldr r4, _08025FC0 @ =0x0201F820
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	mov r0, #0
_08025FB6:
	pop {r4}
	pop {r1}
	bx r1
_08025FBC: .4byte gDiceScreenSteps
_08025FC0: .4byte 0x0201F820
	thumb_func_end DiceScreen_Update

