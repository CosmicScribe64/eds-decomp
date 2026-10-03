	thumb_func_start RecordDuelWin
RecordDuelWin: @ 0x08077948
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r5, _08077980 @ =0x02011C20
	lsl r0, r4, #2
	add r0, r0, r5
	ldr r1, _08077984 @ =0x000020D0
	add r3, r0, r1
	ldrh r2, [r3]
	lsl r0, r2, #0x15
	lsr r1, r0, #0x15
	ldr r0, _08077988 @ =0x000007FE
	cmp r1, r0
	bgt _08077972
	add r1, #1
	ldr r6, _0807798C @ =0x000007FF
	add r0, r6, #0
	and r1, r0
	ldr r0, _08077990 @ =0xFFFFF800
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_08077972:
	ldr r1, _08077994 @ =0x00002158
	add r0, r5, r1
	strh r4, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08077980: .4byte 0x02011C20
_08077984: .4byte 0x000020D0
_08077988: .4byte 0x000007FE
_0807798C: .4byte 0x000007FF
_08077990: .4byte 0xFFFFF800
_08077994: .4byte 0x00002158
	thumb_func_end RecordDuelWin

