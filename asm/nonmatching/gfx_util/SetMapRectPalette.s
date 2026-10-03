	thumb_func_start SetMapRectPalette
SetMapRectPalette: @ 0x0807AC00
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	lsl r1, r1, #0x18
	lsr r5, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r8, r2
	lsl r3, r3, #0x1C
	lsr r6, r3, #0x10
	mov r2, #0
	cmp r2, r8
	bcs _0807AC4E
	mov r0, #0x20
	sub r0, r0, r5
	lsl r0, r0, #1
	mov ip, r0
_0807AC24:
	mov r1, #0
	add r2, #1
	cmp r1, r5
	bcs _0807AC44
	ldr r3, _0807AC58 @ =0x00000FFF
_0807AC2E:
	add r0, r3, #0
	ldrh r7, [r4]
	and r0, r7
	orr r0, r6
	strh r0, [r4]
	add r4, #2
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, r5
	bcc _0807AC2E
_0807AC44:
	add r4, ip
	lsl r0, r2, #0x18
	lsr r2, r0, #0x18
	cmp r2, r8
	bcc _0807AC24
_0807AC4E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807AC58: .4byte 0x00000FFF
	thumb_func_end SetMapRectPalette

