	thumb_func_start WriteSaveSignature
WriteSaveSignature: @ 0x0807701C
	push {lr}
	ldr r0, _0807702C @ =0x02013D86
	ldr r1, _08077030 @ =0x081A78A8
	bl strcpy
	pop {r0}
	bx r0
	.align 2, 0
_0807702C: .4byte 0x02013D86
_08077030: .4byte gSaveSignature
	thumb_func_end WriteSaveSignature

