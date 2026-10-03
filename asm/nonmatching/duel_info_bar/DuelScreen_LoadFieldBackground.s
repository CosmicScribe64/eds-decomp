	thumb_func_start DuelScreen_LoadFieldBackground
DuelScreen_LoadFieldBackground: @ 0x0806044C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	ldr r1, _08060564 @ =0x08086550
	lsl r0, r4, #2
	add r0, r0, r1
	ldr r3, [r0]
	mov r0, #0
	mov r1, #0x50
	mov r2, #0x60
	bl LoadBgImage4bpp
	mov r0, #0
	ldr r1, _08060568 @ =0x0201CFB0
	mov r8, r1
	lsl r4, r4, #4
	mov ip, r4
	ldr r3, _0806056C @ =0x0300045C
	ldr r7, _08060570 @ =0x00005060
_0806047A:
	mov r4, #0
	lsl r5, r0, #5
	add r6, r0, #4
_08060480:
	add r2, r5, r4
	lsl r0, r2, #1
	add r0, r0, r3
	strh r7, [r0]
	add r0, r2, #1
	lsl r0, r0, #1
	add r0, r0, r3
	ldr r1, _08060574 @ =0x00005061
	strh r1, [r0]
	add r0, r2, #2
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #3
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x20
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x21
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x22
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x23
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x41
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x42
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x43
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x60
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x61
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x62
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x63
	lsl r0, r0, #1
	add r0, r0, r3
	add r1, #1
	strh r1, [r0]
	add r0, r4, #4
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0x1F
	bls _08060480
	lsl r0, r6, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x1F
	bls _0806047A
	mov r0, #0xF
	mov r1, r8
	ldrb r1, [r1, #7]
	and r0, r1
	mov r2, ip
	orr r0, r2
	mov r1, r8
	strb r0, [r1, #7]
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08060564: .4byte gFieldBackgroundImages
_08060568: .4byte 0x0201CFB0
_0806056C: .4byte 0x0300045C
_08060570: .4byte 0x00005060
_08060574: .4byte 0x00005061
	thumb_func_end DuelScreen_LoadFieldBackground

