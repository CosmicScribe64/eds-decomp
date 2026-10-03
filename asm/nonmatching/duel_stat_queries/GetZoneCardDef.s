	thumb_func_start GetZoneCardDef
GetZoneCardDef: @ 0x0800C8A8
	push {lr}
	sub sp, #0xC
	mov r2, sp
	bl GetZoneCardStats
	ldr r0, [sp, #8]
	add sp, #0xC
	pop {r1}
	bx r1
	thumb_func_end GetZoneCardDef
	.align 2, 0

