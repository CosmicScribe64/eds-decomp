	thumb_func_start sub_08064768
sub_08064768: @ 0x08064768
	push {r4, r5, lr}
	add r4, r0, #0
	mov r1, #0
	ldr r3, _0806478C @ =0x0202037C
	add r5, r3, #0
	sub r5, #0x40
	ldr r2, _08064790 @ =0x080865DC
_08064776:
	ldrh r0, [r2]
	cmp r0, r4
	bne _08064794
	ldrh r2, [r3]
	lsl r0, r2, #1
	add r0, r0, r5
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	b _0806479C
_0806478C: .4byte 0x0202037C
_08064790: .4byte gUnk_080865DC
_08064794:
	add r2, #0x48
	add r1, #1
	cmp r1, #0x16
	bls _08064776
_0806479C:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08064768
	.align 2, 0

