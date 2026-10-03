	thumb_func_start LineInit
LineInit: @ 0x0807A398
	push {r4, r5, r6, r7, lr}
	ldr r4, [sp, #0x14]
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r2, #0x10
	asr r2, r2, #0x10
	lsr r5, r0, #0x10
	mov ip, r5
	asr r0, r0, #0x10
	sub r2, r2, r0
	strh r2, [r4, #0xC]
	lsr r5, r3, #0x10
	asr r3, r3, #0x10
	lsr r7, r1, #0x10
	asr r1, r1, #0x10
	sub r3, r3, r1
	strh r3, [r4, #0xE]
	lsl r2, r2, #0x10
	cmp r2, #0
	blt _0807A3CA
	mov r0, #1
	strh r0, [r4, #8]
	b _0807A3D6
_0807A3CA:
	ldr r0, _0807A3E4 @ =0x0000FFFF
	strh r0, [r4, #8]
	mov r1, #0xC
	ldsh r0, [r4, r1]
	neg r0, r0
	strh r0, [r4, #0xC]
_0807A3D6:
	mov r2, #0xE
	ldsh r0, [r4, r2]
	cmp r0, #0
	blt _0807A3E8
	mov r0, #1
	strh r0, [r4, #0xA]
	b _0807A3F4
_0807A3E4: .4byte 0x0000FFFF
_0807A3E8:
	ldr r0, _0807A404 @ =0x0000FFFF
	strh r0, [r4, #0xA]
	mov r1, #0xE
	ldsh r0, [r4, r1]
	neg r0, r0
	strh r0, [r4, #0xE]
_0807A3F4:
	mov r2, #0xC
	ldsh r1, [r4, r2]
	mov r2, #0xE
	ldsh r0, [r4, r2]
	cmp r1, r0
	blt _0807A408
	mov r0, #1
	b _0807A40A
_0807A404: .4byte 0x0000FFFF
_0807A408:
	mov r0, #2
_0807A40A:
	strb r0, [r4, #0x12]
	mov r0, #0
	mov r1, ip
	strh r1, [r4]
	strh r7, [r4, #2]
	strh r6, [r4, #4]
	strh r5, [r4, #6]
	strh r0, [r4, #0x10]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end LineInit

