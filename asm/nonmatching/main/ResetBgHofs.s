	thumb_func_start ResetBgHofs
ResetBgHofs: @ 0x08075744
	ldr r0, _0807576C @ =0x03000040
	mov r2, #0
	mov r1, #3
	ldr r3, _08075770 @ =0x0000442E
	add r0, r0, r3
_0807574E:
	strh r2, [r0]
	sub r0, #2
	sub r1, #1
	cmp r1, #0
	bge _0807574E
	mov r1, #0
	ldr r0, _08075774 @ =0x04000010
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	bx lr
_0807576C: .4byte 0x03000040
_08075770: .4byte 0x0000442E
_08075774: .4byte 0x04000010
	thumb_func_end ResetBgHofs

