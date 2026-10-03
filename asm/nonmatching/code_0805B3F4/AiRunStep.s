	thumb_func_start AiRunStep
AiRunStep: @ 0x0805BBE0
	push {r4, lr}
	ldr r1, _0805BC14 @ =0x0819DD6C
	ldr r4, _0805BC18 @ =0x02015EF0
	ldrb r2, [r4, #1]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0805BC1C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805BC10
	mov r0, #0
	strb r0, [r4, #2]
	strb r0, [r4, #3]
	strb r0, [r4, #4]
	strb r0, [r4, #5]
	strb r0, [r4, #0xA]
	strb r0, [r4, #6]
	ldrb r0, [r4, #1]
	add r0, #1
	strb r0, [r4, #1]
_0805BC10:
	mov r0, #0
	b _0805BC1E
_0805BC14: .4byte gAiSteps
_0805BC18: .4byte 0x02015EF0
_0805BC1C:
	mov r0, #1
_0805BC1E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end AiRunStep

