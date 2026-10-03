	thumb_func_start GetDialogueEventId
GetDialogueEventId: @ 0x08001BC8
	add r2, r0, #0
	mov r0, #0xF5
	lsl r0, r0, #1
	cmp r2, r0
	bls _08001BD6
	mov r0, #0
	b _08001BE6
_08001BD6:
	ldr r0, _08001BE8 @ =0x0813ADF4
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #6
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r0, [r1]
_08001BE6:
	bx lr
_08001BE8: .4byte gDialogueTable
	thumb_func_end GetDialogueEventId

