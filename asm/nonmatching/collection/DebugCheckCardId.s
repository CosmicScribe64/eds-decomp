	thumb_func_start DebugCheckCardId
DebugCheckCardId: @ 0x08077468
	push {lr}
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r1, r2, #0
	ldr r3, _08077490 @ =0xFFFFF893
	add r0, r2, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x62
	bls _08077480
	cmp r2, #0
	bne _0807748A
_08077480:
	ldr r0, _08077494 @ =0x08087B80
	bl DebugPrintf
	bl DebugPrintFlush
_0807748A:
	pop {r0}
	bx r0
	.align 2, 0
_08077490: .4byte 0xFFFFF893
_08077494: .4byte gStrErrorIdFmt
	thumb_func_end DebugCheckCardId

