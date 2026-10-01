	thumb_func_start sub_08062420
sub_08062420: @ 0x08062420
	push {r4, lr}
	ldr r3, _0806244C @ =0x0500001E
	ldr r2, _08062450 @ =0x081A451C
	ldr r0, _08062454 @ =0x04000006
	ldrh r0, [r0]
	ldr r1, _08062458 @ =0x02015160
	mov r4, #0x8D
	lsl r4, r4, #1
	add r1, r1, r4
	ldrh r1, [r1]
	lsr r1, r1, #1
	add r0, r0, r1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	strh r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0806244C: .4byte 0x0500001E
_08062450: .4byte gUnk_081A451C
_08062454: .4byte 0x04000006
_08062458: .4byte 0x02015160
	thumb_func_end sub_08062420

