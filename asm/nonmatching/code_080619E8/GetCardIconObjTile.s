	thumb_func_start GetCardIconObjTile
GetCardIconObjTile: @ 0x08062140
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _0806215C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08062160 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08062164 @ =0x00000776
	cmp r1, r0
	bne _08062168
	mov r0, #0xA0
	lsl r0, r0, #1
	b _08062268
_0806215C: .4byte 0x000007FF
_08062160: .4byte gCardIdToNumber
_08062164: .4byte 0x00000776
_08062168:
	cmp r1, r0
	blt _08062178
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08062178
_08062174:
	mov r0, #0xC0
	b _08062268
_08062178:
	ldr r0, _0806219C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080621A0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080621B0
	cmp r0, #0x16
	bgt _080621A4
	cmp r0, #0x15
	beq _080621AA
	b _080621B6
	.align 2, 0
_0806219C: .4byte 0x000007FF
_080621A0: .4byte gCardStats
_080621A4:
	cmp r0, #0x17
	beq _0806225A
	b _080621B6
_080621AA:
	mov r0, #0xE0
	lsl r0, r0, #1
	b _08062268
_080621B0:
	mov r0, #0xC0
	lsl r0, r0, #1
	b _08062268
_080621B6:
	ldr r0, _080621CC @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _080621D0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080621D4 @ =0x00000776
	cmp r1, r0
	bne _080621D8
	mov r0, #3
	b _0806223A
_080621CC: .4byte 0x000007FF
_080621D0: .4byte gCardIdToNumber
_080621D4: .4byte 0x00000776
_080621D8:
	cmp r1, r0
	blt _080621E8
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080621E8
	mov r0, #1
	b _0806223A
_080621E8:
	ldr r0, _0806220C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08062210 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806221A
	cmp r0, #0x16
	bgt _08062214
	cmp r0, #0x15
	beq _0806221E
	b _08062226
	.align 2, 0
_0806220C: .4byte 0x000007FF
_08062210: .4byte gCardStats
_08062214:
	cmp r0, #0x17
	beq _08062222
	b _08062226
_0806221A:
	mov r0, #7
	b _0806223A
_0806221E:
	mov r0, #8
	b _0806223A
_08062222:
	mov r0, #9
	b _0806223A
_08062226:
	ldr r0, _08062248 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0806224C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806223A:
	cmp r0, #1
	beq _08062174
	cmp r0, #1
	bgt _08062250
	cmp r0, #0
	beq _0806225A
	b _08062268
_08062248: .4byte 0x000007FF
_0806224C: .4byte gCardStats
_08062250:
	cmp r0, #2
	beq _0806225E
	cmp r0, #3
	beq _08062264
	b _08062268
_0806225A:
	mov r0, #0x80
	b _08062268
_0806225E:
	mov r0, #0x80
	lsl r0, r0, #1
	b _08062268
_08062264:
	mov r0, #0xA0
	lsl r0, r0, #1
_08062268:
	bx lr
	thumb_func_end GetCardIconObjTile
	.align 2, 0

