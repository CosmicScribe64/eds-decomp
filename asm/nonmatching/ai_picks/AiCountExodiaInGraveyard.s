	thumb_func_start AiCountExodiaInGraveyard
AiCountExodiaInGraveyard: @ 0x08057854
	push {r4, r5, r6, lr}
	mov r2, #0
	mov r3, #0
	ldr r1, _08057898 @ =0x020192E4
	ldr r4, _0805789C @ =0x00000D68
	add r0, r1, r4
	ldrb r0, [r0]
	cmp r3, r0
	bge _08057890
	add r4, r0, #0
	ldr r5, _080578A0 @ =0x000007FF
	ldr r6, _080578A4 @ =0x00001668
	add r1, r1, r6
_0805786E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	ldr r6, _080578A8 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, #0x14
	bgt _08057888
	cmp r0, #0x10
	blt _08057888
	add r3, #1
_08057888:
	add r1, #4
	add r2, #1
	cmp r2, r4
	blt _0805786E
_08057890:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08057898: .4byte 0x020192E4
_0805789C: .4byte 0x00000D68
_080578A0: .4byte 0x000007FF
_080578A4: .4byte 0x00001668
_080578A8: .4byte gCardIdToNumber
	thumb_func_end AiCountExodiaInGraveyard

