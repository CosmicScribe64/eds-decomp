	thumb_func_start sub_08000854
sub_08000854: @ 0x08000854
	push {lr}
	ldrh r1, [r0, #0x22]
	cmp r1, #1
	bne _08000874
	ldr r3, [r0, #0xC]
	add r0, #0x20
	ldr r1, _08000878 @ =0x0600FA00
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800086A
	ldr r1, _0800087C @ =0x06005A00
_0800086A:
	mov r2, #0xF0
	lsl r2, r2, #5
	add r0, r3, #0
	bl CpuSet
_08000874:
	pop {r0}
	bx r0
_08000878: .4byte 0x0600FA00
_0800087C: .4byte 0x06005A00
	thumb_func_end sub_08000854

