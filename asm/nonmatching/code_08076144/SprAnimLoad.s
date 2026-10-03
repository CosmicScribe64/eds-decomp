	thumb_func_start SprAnimLoad
SprAnimLoad: @ 0x0807695C
	push {r4, r5, r6, r7, lr}
	add r3, r0, #0
	add r5, r1, #0
	str r3, [r5]
	str r3, [r5, #4]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	ldr r0, _080769D4 @ =0x050003E0
	add r1, r3, #0
	mov r2, #0x20
	bl MemCopy16
	ldr r0, [r5, #4]
	ldrh r2, [r0, #0x20]
	add r0, #0x22
	strh r2, [r5, #0xC]
	ldr r7, _080769D8 @ =0x06010020
	lsl r1, r2, #2
	add r0, r0, r1
	str r0, [r5, #4]
	mov r6, #0
	cmp r6, r2
	bcs _080769B8
_08076992:
	ldr r1, [r5, #4]
	ldrh r4, [r1]
	add r1, #2
	str r1, [r5, #4]
	lsl r4, r4, #5
	add r0, r7, #0
	add r2, r4, #0
	bl MemCopy16
	add r7, r7, r4
	ldr r0, [r5, #4]
	add r0, r0, r4
	str r0, [r5, #4]
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	ldrh r0, [r5, #0xC]
	cmp r6, r0
	bcc _08076992
_080769B8:
	ldr r0, [r5, #4]
	ldrh r1, [r0]
	add r0, #2
	str r0, [r5, #4]
	mov r0, #0
	strh r1, [r5, #8]
	strh r0, [r5, #0xA]
	add r0, r5, #0
	bl SprAnimRewind
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080769D4: .4byte 0x050003E0
_080769D8: .4byte 0x06010020
	thumb_func_end SprAnimLoad

