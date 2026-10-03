	thumb_func_start SetBgmEnabled
SetBgmEnabled: @ 0x08077AB0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077AD0
	ldr r0, _08077AC8 @ =0x02011C20
	ldr r1, _08077ACC @ =0x00002152
	add r0, r0, r1
	mov r1, #2
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
	b _08077ADE
	.align 2, 0
_08077AC8: .4byte 0x02011C20
_08077ACC: .4byte 0x00002152
_08077AD0:
	ldr r1, _08077AE0 @ =0x02011C20
	ldr r0, _08077AE4 @ =0x00002152
	add r1, r1, r0
	ldr r0, _08077AE8 @ =0x0000FFFD
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_08077ADE:
	bx lr
_08077AE0: .4byte 0x02011C20
_08077AE4: .4byte 0x00002152
_08077AE8: .4byte 0x0000FFFD
	thumb_func_end SetBgmEnabled

