	thumb_func_start GetRewardPack
GetRewardPack: @ 0x08063B48
	push {r4, r5, r6, lr}
	ldr r5, _08063B88 @ =0x03000040
	ldr r2, _08063B8C @ =0x00004876
	add r1, r5, r2
	mov r6, #0
	strh r0, [r1]
	ldr r1, _08063B90 @ =0x081A572C
	ldr r0, _08063B94 @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08063BA0
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063B82
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	ldr r1, _08063B98 @ =0x0000485A
	add r0, r5, r1
	strb r6, [r0]
	ldr r2, _08063B9C @ =0x0000485B
	add r0, r5, r2
	strb r6, [r0]
_08063B82:
	mov r0, #0
	b _08063BA2
	.align 2, 0
_08063B88: .4byte 0x03000040
_08063B8C: .4byte 0x00004876
_08063B90: .4byte gGetPackSteps
_08063B94: .4byte 0x00004859
_08063B98: .4byte 0x0000485A
_08063B9C: .4byte 0x0000485B
_08063BA0:
	mov r0, #1
_08063BA2:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end GetRewardPack

