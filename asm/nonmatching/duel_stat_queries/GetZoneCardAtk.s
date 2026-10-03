	thumb_func_start GetZoneCardAtk
GetZoneCardAtk: @ 0x0800C894
	push {lr}
	sub sp, #0xC
	mov r2, sp
	bl GetZoneCardStats
	ldr r0, [sp, #4]
	add sp, #0xC
	pop {r1}
	bx r1
	thumb_func_end GetZoneCardAtk
	.align 2, 0

