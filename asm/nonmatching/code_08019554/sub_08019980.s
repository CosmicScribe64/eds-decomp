	thumb_func_start sub_08019980
sub_08019980: @ 0x08019980
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r1, #0
	ldr r7, _080199D4 @ =0x0000059A
	add r1, r7, #0
	bl sub_080086CC
	add r4, r0, #0
	cmp r6, #0
	beq _080199CC
	mov r0, #0x42
	cmp r5, #0
	beq _0801999C
	ldr r0, _080199D8 @ =0x00008042
_0801999C:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	cmp r4, #0
	ble _080199CC
	lsl r0, r7, #1
	ldr r1, _080199DC @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #1
	sub r0, r0, r5
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	bl sub_08019860
_080199CC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080199D4: .4byte 0x0000059A
_080199D8: .4byte 0x00008042
_080199DC: .4byte gUnk_08623DF4
	thumb_func_end sub_08019980

