	thumb_func_start DiceScreen_RunPlain
DiceScreen_RunPlain: @ 0x0802615C
	push {r4, lr}
	ldr r1, _08026184 @ =0x08199A70
	ldr r4, _08026188 @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0802618C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802617E
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_0802617E:
	mov r0, #0
	b _0802618E
	.align 2, 0
_08026184: .4byte gDiceScreenPlainSteps
_08026188: .4byte 0x02017A30
_0802618C:
	mov r0, #1
_0802618E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DiceScreen_RunPlain

