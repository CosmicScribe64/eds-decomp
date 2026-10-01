	thumb_func_start sub_0807761C
sub_0807761C: @ 0x0807761C
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	add r0, r4, #0
	bl sub_08077468
	ldr r3, _0807765C @ =0x02011C20
	lsl r4, r4, #2
	add r4, r4, r3
	ldrh r2, [r4, #8]
	lsl r1, r2, #0x16
	cmp r1, #0
	beq _08077654
	lsr r1, r1, #0x16
	sub r1, #1
	ldr r5, _08077660 @ =0x000003FF
	add r0, r5, #0
	and r1, r0
	ldr r0, _08077664 @ =0xFFFFFC00
	and r0, r2
	orr r0, r1
	strh r0, [r4, #8]
	ldr r0, _08077668 @ =0x000020C6
	add r1, r3, r0
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
_08077654:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807765C: .4byte 0x02011C20
_08077660: .4byte 0x000003FF
_08077664: .4byte 0xFFFFFC00
_08077668: .4byte 0x000020C6
	thumb_func_end sub_0807761C

