	thumb_func_start ClearZoneCardStatusFlags
ClearZoneCardStatusFlags: @ 0x0800747C
	push {lr}
	mov r2, #1
	and r2, r0
	ldr r0, _08007498 @ =0x00000D64
	mul r0, r2
	ldr r2, _0800749C @ =0x0201930C
	add r0, r0, r2
	mov r2, #0x94
	mul r1, r2
	add r0, r0, r1
	bl ClearCardStatusFlags
	pop {r0}
	bx r0
_08007498: .4byte 0x00000D64
_0800749C: .4byte 0x0201930C
	thumb_func_end ClearZoneCardStatusFlags

