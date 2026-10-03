	thumb_func_start ClearZone
ClearZone: @ 0x08008278
	push {lr}
	mov r2, #1
	and r2, r0
	ldr r0, _08008298 @ =0x00000D64
	mul r0, r2
	ldr r2, _0800829C @ =0x0201930C
	add r0, r0, r2
	mov r2, #0x94
	mul r1, r2
	add r0, r0, r1
	mov r1, #0x94
	bl MemClear16
	pop {r0}
	bx r0
	.align 2, 0
_08008298: .4byte 0x00000D64
_0800829C: .4byte 0x0201930C
	thumb_func_end ClearZone

