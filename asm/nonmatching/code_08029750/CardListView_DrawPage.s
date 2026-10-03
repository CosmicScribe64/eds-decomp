	thumb_func_start CardListView_DrawPage
CardListView_DrawPage: @ 0x0802A47C
	push {r4, lr}
	ldr r3, _0802A4A0 @ =0x0201D810
	ldrh r1, [r3, #6]
	lsl r0, r1, #2
	add r2, r3, #0
	add r2, #0xC
	add r0, r0, r2
	mov r4, #0xC3
	lsl r4, r4, #2
	add r2, r3, r4
	ldrh r2, [r2]
	sub r1, r2, r1
	bl CardListView_DrawNames
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802A4A0: .4byte 0x0201D810
	thumb_func_end CardListView_DrawPage

