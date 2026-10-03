	thumb_func_start RemoveZoneLink
RemoveZoneLink: @ 0x08009424
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r1, r0, #8
	lsr r1, r1, #0x18
	mov r8, r1
	lsr r0, r0, #0x18
	mov ip, r0
	mov r6, #0
	mov r1, #1
	mov r0, r8
	and r1, r0
	mov r0, #0x94
	mov r5, ip
	mul r5, r0
	ldr r7, _080094B0 @ =0x00000D64
	add r0, r1, #0
	mul r0, r7
	add r0, r5, r0
	ldr r3, _080094B4 @ =0x0201930C
	add r0, r0, r3
	add r0, #0x8A
	mov r9, r3
	ldrh r0, [r0]
	cmp r6, r0
	bge _080094D0
	add r4, r1, #0
	add r1, r5, #0
	mov r5, #0
_0800946E:
	add r0, r4, #0
	mul r0, r7
	add r0, r1, r0
	add r0, r0, r3
	add r0, #0x4A
	add r0, r0, r5
	ldrb r0, [r0]
	cmp r0, r2
	bne _080094B8
	cmp r2, #4
	blt _08009490
	cmp r2, #7
	ble _080094A2
	cmp r2, #0xC
	bgt _08009490
	cmp r2, #9
	bge _080094A2
_08009490:
	ldr r0, _080094B0 @ =0x00000D64
	mul r0, r4
	add r0, r1, r0
	add r0, r9
	add r0, #0xA
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, sl
	bne _080094B8
_080094A2:
	mov r0, r8
	mov r1, ip
	add r2, r6, #0
	bl RemoveZoneLinkAt
	b _080094D0
	.align 2, 0
_080094B0: .4byte 0x00000D64
_080094B4: .4byte 0x0201930C
_080094B8:
	add r5, #2
	add r6, #1
	ldr r7, _080094E0 @ =0x00000D64
	add r0, r4, #0
	mul r0, r7
	add r0, r1, r0
	mov r3, r9
	add r0, r0, r3
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	blt _0800946E
_080094D0:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080094E0: .4byte 0x00000D64
	thumb_func_end RemoveZoneLink

