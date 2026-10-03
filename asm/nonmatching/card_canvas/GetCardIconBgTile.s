	thumb_func_start GetCardIconBgTile
GetCardIconBgTile: @ 0x0806226C
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r2, _080622A0 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _080622A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _080622B0
	cmp r0, #0x16
	beq _080622B6
	lsl r0, r2, #1
	ldr r1, _080622A8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080622AC @ =0x00000776
	cmp r1, r0
	bne _080622BC
	mov r0, #3
	b _0806231E
	.align 2, 0
_080622A0: .4byte 0x000007FF
_080622A4: .4byte gCardStats
_080622A8: .4byte gCardIdToNumber
_080622AC: .4byte 0x00000776
_080622B0:
	mov r0, #0xF8
	lsl r0, r0, #1
	b _08062350
_080622B6:
	mov r0, #0xD8
	lsl r0, r0, #1
	b _08062350
_080622BC:
	cmp r1, r0
	blt _080622CC
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080622CC
	mov r0, #1
	b _0806231E
_080622CC:
	ldr r0, _080622F0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080622F4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080622FE
	cmp r0, #0x16
	bgt _080622F8
	cmp r0, #0x15
	beq _08062302
	b _0806230A
	.align 2, 0
_080622F0: .4byte 0x000007FF
_080622F4: .4byte gCardStats
_080622F8:
	cmp r0, #0x17
	beq _08062306
	b _0806230A
_080622FE:
	mov r0, #7
	b _0806231E
_08062302:
	mov r0, #8
	b _0806231E
_08062306:
	mov r0, #9
	b _0806231E
_0806230A:
	ldr r0, _0806232C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08062330 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806231E:
	cmp r0, #1
	beq _08062342
	cmp r0, #1
	bgt _08062334
	cmp r0, #0
	beq _0806233E
	b _08062350
_0806232C: .4byte 0x000007FF
_08062330: .4byte gCardStats
_08062334:
	cmp r0, #2
	beq _08062346
	cmp r0, #3
	beq _0806234C
	b _08062350
_0806233E:
	mov r0, #0xB0
	b _08062350
_08062342:
	mov r0, #0xF0
	b _08062350
_08062346:
	mov r0, #0x98
	lsl r0, r0, #1
	b _08062350
_0806234C:
	mov r0, #0xB8
	lsl r0, r0, #1
_08062350:
	bx lr
	thumb_func_end GetCardIconBgTile
	.align 2, 0

