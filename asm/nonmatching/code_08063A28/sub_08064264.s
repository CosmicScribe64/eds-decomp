	thumb_func_start sub_08064264
sub_08064264: @ 0x08064264
	push {r4, r5, lr}
	mov r5, #0
	ldr r4, _0806428C @ =0x081A5758
_0806426A:
	ldrh r0, [r4]
	bl sub_08063EDC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806427C
	ldrh r0, [r4]
	bl sub_08064768
_0806427C:
	add r4, #2
	add r5, #1
	cmp r5, #0x1A
	bls _0806426A
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0806428C: .4byte gUnk_081A5758
	thumb_func_end sub_08064264

