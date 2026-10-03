	thumb_func_start AiRiskSetCounter
AiRiskSetCounter: @ 0x08059A30
	push {lr}
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0
	bl CountActivatableSetCards
	cmp r0, #0
	beq _08059A70
	ldr r0, _08059A60 @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _08059A64
	bl Random
	mov r1, #3
	and r1, r0
	cmp r1, #0
	beq _08059A70
_08059A5A:
	mov r0, #0
	b _08059A72
	.align 2, 0
_08059A60: .4byte 0x02015EE8
_08059A64:
	bl Random
	mov r1, #7
	and r1, r0
	cmp r1, #0
	beq _08059A5A
_08059A70:
	mov r0, #1
_08059A72:
	pop {r1}
	bx r1
	thumb_func_end AiRiskSetCounter
	.align 2, 0

