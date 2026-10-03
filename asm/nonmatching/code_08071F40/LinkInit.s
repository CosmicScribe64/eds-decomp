	thumb_func_start LinkInit
LinkInit: @ 0x08072510
	push {r4, lr}
	bl LinkSioStop
	ldr r4, _08072548 @ =0x030049D0
	mov r1, #0x84
	lsl r1, r1, #4
	add r0, r4, #0
	bl MemClear16
	ldr r0, _0807254C @ =0x03000000
	add r1, r0, #0
	add r1, #0x1C
	bl LinkSioInit
	ldr r0, _08072550 @ =0x0000051C
	add r4, r4, r0
	mov r0, #0xF
	strh r0, [r4]
	ldr r0, _08072554 @ =0x03000040
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r0, r1
	ldr r1, _08072558 @ =0x08072055
	str r1, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08072548: .4byte 0x030049D0
_0807254C: .4byte 0x03000000
_08072550: .4byte 0x0000051C
_08072554: .4byte 0x03000040
_08072558: .4byte LinkVBlankHook
	thumb_func_end LinkInit

