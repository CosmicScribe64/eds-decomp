	thumb_func_start sub_08077498
sub_08077498: @ 0x08077498
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	add r0, r4, #0
	bl sub_08077468
	ldr r3, _080774D8 @ =0x02011C20
	lsl r4, r4, #2
	add r4, r4, r3
	ldrh r2, [r4, #8]
	lsl r1, r2, #0x16
	ldr r0, _080774DC @ =0xFF800000
	cmp r1, r0
	bhi _080774D2
	lsr r1, r1, #0x16
	add r1, #1
	ldr r5, _080774E0 @ =0x000003FF
	add r0, r5, #0
	and r1, r0
	ldr r0, _080774E4 @ =0xFFFFFC00
	and r0, r2
	orr r0, r1
	strh r0, [r4, #8]
	ldr r0, _080774E8 @ =0x000020C6
	add r1, r3, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
_080774D2:
	pop {r4, r5}
	pop {r0}
	bx r0
_080774D8: .4byte 0x02011C20
_080774DC: .4byte 0xFF800000
_080774E0: .4byte 0x000003FF
_080774E4: .4byte 0xFFFFFC00
_080774E8: .4byte 0x000020C6
	thumb_func_end sub_08077498

