	thumb_func_start IncrementChampionshipWins
IncrementChampionshipWins: @ 0x08077A28
	ldr r0, _08077A3C @ =0x02011C20
	ldr r2, _08077A40 @ =0x00002162
	add r1, r0, r2
	ldrb r0, [r1]
	cmp r0, #0xFE
	bhi _08077A38
	add r0, #1
	strb r0, [r1]
_08077A38:
	bx lr
	.align 2, 0
_08077A3C: .4byte 0x02011C20
_08077A40: .4byte 0x00002162
	thumb_func_end IncrementChampionshipWins

