	thumb_func_start Password_InitState
Password_InitState: @ 0x0807C46C
	push {r4, lr}
	ldr r4, _0807C4A0 @ =0x0201F7B0
	add r0, r4, #0
	mov r1, #0x18
	bl MemClear16
	mov r2, #0xF
	mov r3, #0
	strb r3, [r4, #0x10]
	mov r0, #0x40
	neg r0, r0
	ldrb r1, [r4, #0x11]
	and r0, r1
	strb r0, [r4, #0x11]
	ldr r0, [r4, #0x10]
	ldr r1, _0807C4A4 @ =0xFFF03FFF
	and r0, r1
	str r0, [r4, #0x10]
	ldrh r0, [r4, #0x12]
	and r2, r0
	strh r2, [r4, #0x12]
	strh r3, [r4, #0x14]
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
_0807C4A0: .4byte 0x0201F7B0
_0807C4A4: .4byte 0xFFF03FFF
	thumb_func_end Password_InitState

