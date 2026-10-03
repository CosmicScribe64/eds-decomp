	thumb_func_start CopyObjTileBlock4x4
CopyObjTileBlock4x4: @ 0x08026388
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r6, #3
_08026390:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x80
	bl MemCopy16
	mov r0, #0x80
	lsl r0, r0, #3
	add r4, r4, r0
	add r5, #0x80
	sub r6, #1
	cmp r6, #0
	bge _08026390
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end CopyObjTileBlock4x4
	.align 2, 0

