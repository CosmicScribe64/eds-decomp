	thumb_func_start Bustup_MarkBoxDirty
Bustup_MarkBoxDirty: @ 0x0800093C
	mov r1, #1
	strh r1, [r0, #0x22]
	bx lr
	thumb_func_end Bustup_MarkBoxDirty
	.align 2, 0

