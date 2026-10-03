	thumb_func_start IsValidEquipTarget
IsValidEquipTarget: @ 0x0800CCCC
	push {r4, r5, r6, lr}
	sub sp, #0x14
	mov r6, sp
	mov r5, #1
	and r5, r0
	mov r4, #0x94
	mul r1, r4
	ldr r4, _0800CD1C @ =0x00000D64
	mul r4, r5
	add r1, r1, r4
	ldr r4, _0800CD20 @ =0x0201930C
	add r1, r1, r4
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	strh r1, [r6]
	mov r5, sp
	mov r1, #1
	and r0, r1
	ldrb r4, [r5, #2]
	mov r1, #2
	neg r1, r1
	and r1, r4
	orr r1, r0
	strb r1, [r5, #2]
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r2, r2, #8
	orr r2, r3
	lsr r2, r2, #0x10
	mov r0, sp
	add r1, r2, #0
	bl EffectEquipTargetCheck
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0800CD1C: .4byte 0x00000D64
_0800CD20: .4byte 0x0201930C
	thumb_func_end IsValidEquipTarget

