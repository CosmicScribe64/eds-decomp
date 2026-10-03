	thumb_func_start DuelScene_Run
DuelScene_Run: @ 0x0801EAD8
	push {r4, lr}
	ldr r1, _0801EB08 @ =0x08198F14
	ldr r4, _0801EB0C @ =0x02017A30
	ldrb r2, [r4, #0xA]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0801EB10
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801EB02
	ldrb r1, [r4, #0xA]
	add r1, #1
	mov r0, #0
	strb r1, [r4, #0xA]
	strb r0, [r4, #0xB]
	strb r0, [r4, #0xC]
	strb r0, [r4, #0xD]
_0801EB02:
	mov r0, #0
	b _0801EB14
	.align 2, 0
_0801EB08: .4byte gDuelSceneRunnerSteps
_0801EB0C: .4byte 0x02017A30
_0801EB10:
	strb r0, [r4, #0xA]
	mov r0, #1
_0801EB14:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DuelScene_Run
	.align 2, 0

