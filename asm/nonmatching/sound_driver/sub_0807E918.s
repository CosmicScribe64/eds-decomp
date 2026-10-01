	thumb_func_start sub_0807E918
sub_0807E918: @ 0x0807E918
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	ldr r0, _0807E938 @ =0x0000CFFF
	and r4, r0
	mov r0, #0x80
	lsl r0, r0, #8
	and r0, r4
	cmp r0, #0
	beq _0807E944
	ldr r1, _0807E93C @ =0x08088A20
	ldr r0, _0807E940 @ =0x00003FFF
	and r0, r4
	lsl r0, r0, #2
	b _0807E948
_0807E938: .4byte 0x0000CFFF
_0807E93C: .4byte gUnk_08088A20
_0807E940: .4byte 0x00003FFF
_0807E944:
	ldr r1, _0807E984 @ =0x0811B420
	lsl r0, r4, #2
_0807E948:
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r3, #1
	ldr r1, _0807E988 @ =0x081A960C
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, [r2]
	mul r0, r1
	asr r3, r0, #0xC
	str r3, [r5, #8]
	ldr r0, [r2, #4]
	str r0, [r5, #4]
	add r0, r2, #0
	add r0, #0xC
	str r0, [r5]
	ldr r1, _0807E98C @ =0xFFFF8FFF
	add r0, r1, #0
	and r4, r0
	strh r4, [r5, #0xC]
	strb r6, [r5, #0xF]
	mov r0, #0x80
	strb r0, [r5, #0xE]
	ldr r0, [r2, #8]
	cmp r0, #0
	blt _0807E97E
	mov r0, #0xC0
	strb r0, [r5, #0xE]
_0807E97E:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0807E984: .4byte gUnk_0811B420
_0807E988: .4byte gUnk_081A960C
_0807E98C: .4byte 0xFFFF8FFF
	thumb_func_end sub_0807E918

