	thumb_func_start sub_080261E0
sub_080261E0: @ 0x080261E0
	push {r4, lr}
	ldr r3, _0802620C @ =0x04000010
	ldr r2, _08026210 @ =0x08087BA4
	ldr r0, _08026214 @ =0x04000006
	ldrh r1, [r0]
	ldr r0, _08026218 @ =0x02020310
	ldr r4, _0802621C @ =0x00000B24
	add r0, r0, r4
	ldrb r0, [r0]
	add r1, r0, r1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	add r1, r1, r2
	ldrh r1, [r1]
	lsl r0, r1, #0x10
	asr r0, r0, #0x15
	strh r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802620C: .4byte 0x04000010
_08026210: .4byte gUnk_08087BA4
_08026214: .4byte 0x04000006
_08026218: .4byte 0x02020310
_0802621C: .4byte 0x00000B24
	thumb_func_end sub_080261E0

