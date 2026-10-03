	thumb_func_start InitSaveData
InitSaveData: @ 0x080770E8
	push {lr}
	ldr r0, _0807710C @ =0x02011C20
	ldr r1, _08077110 @ =0x00002170
	bl MemClear16
	mov r0, #1
	bl SetSeEnabled
	mov r0, #1
	bl SetBgmEnabled
	bl SetTextModeLatin
	bl WriteSaveSignature
	pop {r0}
	bx r0
	.align 2, 0
_0807710C: .4byte 0x02011C20
_08077110: .4byte 0x00002170
	thumb_func_end InitSaveData

