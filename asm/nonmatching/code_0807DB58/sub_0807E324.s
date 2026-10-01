	thumb_func_start sub_0807E324
sub_0807E324: @ 0x0807E324
	push {r4, r5, lr}
	ldr r5, _0807E388 @ =0x0300540C
	ldrh r0, [r5]
	add r1, r0, #0
	add r1, #0x10
	ldr r0, _0807E38C @ =0x000002BF
	cmp r1, r0
	ble _0807E37C
	ldr r3, _0807E390 @ =0x040000BC
	ldrh r1, [r3, #0xA]
	ldr r4, _0807E394 @ =0x0000C5FF
	add r0, r4, #0
	and r0, r1
	strh r0, [r3, #0xA]
	ldrh r1, [r3, #0xA]
	ldr r2, _0807E398 @ =0x00007FFF
	add r0, r2, #0
	and r0, r1
	strh r0, [r3, #0xA]
	ldrh r0, [r3, #0xA]
	ldr r1, _0807E39C @ =0x040000C8
	ldrh r0, [r1, #0xA]
	and r4, r0
	strh r4, [r1, #0xA]
	ldrh r0, [r1, #0xA]
	and r2, r0
	strh r2, [r1, #0xA]
	ldrh r0, [r1, #0xA]
	ldr r2, _0807E3A0 @ =0x03005414
	str r2, [r3]
	ldr r0, _0807E3A4 @ =0x040000A0
	str r0, [r3, #4]
	ldr r4, _0807E3A8 @ =0xF6000004
	str r4, [r3, #8]
	ldr r0, [r3, #8]
	mov r0, #0xC8
	lsl r0, r0, #2
	add r2, r2, r0
	str r2, [r1]
	ldr r0, _0807E3AC @ =0x040000A4
	str r0, [r1, #4]
	str r4, [r1, #8]
	ldr r0, [r1, #8]
	mov r1, #0
_0807E37C:
	strh r1, [r5, #4]
	strh r1, [r5]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807E388: .4byte 0x0300540C
_0807E38C: .4byte 0x000002BF
_0807E390: .4byte 0x040000BC
_0807E394: .4byte 0x0000C5FF
_0807E398: .4byte 0x00007FFF
_0807E39C: .4byte 0x040000C8
_0807E3A0: .4byte 0x03005414
_0807E3A4: .4byte 0x040000A0
_0807E3A8: .4byte 0xF6000004
_0807E3AC: .4byte 0x040000A4
	thumb_func_end sub_0807E324

