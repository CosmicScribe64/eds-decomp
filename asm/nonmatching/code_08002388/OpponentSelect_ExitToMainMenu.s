	thumb_func_start OpponentSelect_ExitToMainMenu
OpponentSelect_ExitToMainMenu: @ 0x08002FBC
	push {lr}
	ldr r0, _08002FCC @ =0x08003AA5
	bl SetMainCallback
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08002FCC: .4byte CB_MainMenu
	thumb_func_end OpponentSelect_ExitToMainMenu

