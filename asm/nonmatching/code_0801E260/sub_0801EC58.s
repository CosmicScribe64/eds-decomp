	thumb_func_start sub_0801EC58
sub_0801EC58: @ 0x0801EC58
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	ldr r2, _0801ECA0 @ =0x020185C0
	ldr r0, _0801ECA4 @ =0x00000808
	add r1, r2, r0
	ldrh r7, [r1]
	cmp r7, #0xFF
	bhi _0801EC9A
	lsl r0, r7, #3
	add r0, r0, r2
	strh r4, [r0, #8]
	ldrh r4, [r1]
	lsl r0, r4, #3
	add r0, r0, r2
	strh r5, [r0, #0xA]
	ldrh r7, [r1]
	lsl r0, r7, #3
	add r0, r0, r2
	strh r6, [r0, #0xC]
	ldrh r4, [r1]
	lsl r0, r4, #3
	add r0, r0, r2
	strh r3, [r0, #0xE]
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
_0801EC9A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801ECA0: .4byte 0x020185C0
_0801ECA4: .4byte 0x00000808
	thumb_func_end sub_0801EC58

