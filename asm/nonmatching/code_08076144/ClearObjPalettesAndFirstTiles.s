	thumb_func_start ClearObjPalettesAndFirstTiles
ClearObjPalettesAndFirstTiles: @ 0x080768F0
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08076948 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _0807694C @ =0x05000200
	str r0, [r1, #4]
	ldr r0, _08076950 @ =0x81000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _0807691A
_08076912:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _08076912
_0807691A:
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08076948 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _08076954 @ =0x06010000
	str r0, [r1, #4]
	ldr r0, _08076958 @ =0x81000040
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _08076942
_0807693A:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _0807693A
_08076942:
	add sp, #4
	bx lr
	.align 2, 0
_08076948: .4byte 0x040000D4
_0807694C: .4byte 0x05000200
_08076950: .4byte 0x81000100
_08076954: .4byte 0x06010000
_08076958: .4byte 0x81000040
	thumb_func_end ClearObjPalettesAndFirstTiles

