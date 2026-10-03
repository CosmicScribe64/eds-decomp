	thumb_func_start ObjAffineApply
ObjAffineApply: @ 0x0807B5A0
	push {r4, r5, lr}
	add r4, r0, #0
	mov r1, #0
	ldsh r0, [r4, r1]
	bl ReciprocalFix8
	ldr r5, _0807B624 @ =0x08087BA4
	ldrh r2, [r4, #4]
	lsr r1, r2, #8
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	ldr r1, [r4, #8]
	strh r0, [r1]
	mov r1, #0
	ldsh r0, [r4, r1]
	bl ReciprocalFix8
	ldrh r2, [r4, #4]
	lsr r1, r2, #8
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	ldr r1, [r4, #0xC]
	strh r0, [r1]
	mov r1, #2
	ldsh r0, [r4, r1]
	bl ReciprocalFix8
	ldrh r2, [r4, #4]
	lsr r1, r2, #8
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	neg r1, r1
	bl MulFix8
	ldr r1, [r4, #0x10]
	strh r0, [r1]
	mov r1, #2
	ldsh r0, [r4, r1]
	bl ReciprocalFix8
	ldrh r2, [r4, #4]
	lsr r1, r2, #8
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	ldr r1, [r4, #0x14]
	strh r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807B624: .4byte gSineTable
	thumb_func_end ObjAffineApply

