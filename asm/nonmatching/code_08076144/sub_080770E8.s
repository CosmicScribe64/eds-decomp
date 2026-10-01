	thumb_func_start sub_080770E8
sub_080770E8: @ 0x080770E8
	push {lr}
	ldr r0, _0807710C @ =0x02011C20
	ldr r1, _08077110 @ =0x00002170
	bl sub_08075278
	mov r0, #1
	bl sub_08077A74
	mov r0, #1
	bl sub_08077AB0
	bl sub_080770DC
	bl sub_0807701C
	pop {r0}
	bx r0
	.align 2, 0
_0807710C: .4byte 0x02011C20
_08077110: .4byte 0x00002170
	thumb_func_end sub_080770E8

