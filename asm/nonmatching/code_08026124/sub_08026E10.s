	thumb_func_start sub_08026E10
sub_08026E10: @ 0x08026E10
	push {r4, lr}
	ldr r0, _08026E64 @ =0x04000010
	mov r3, #0
	strh r3, [r0]
	ldr r2, _08026E68 @ =0x04000012
	ldr r1, _08026E6C @ =0x02020310
	ldr r4, _08026E70 @ =0x00000ABA
	add r0, r1, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x10
	asr r0, r0, #0x14
	strh r0, [r2]
	ldr r0, _08026E74 @ =0x04000014
	strh r3, [r0]
	add r2, #4
	add r4, #0x14
	add r0, r1, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x10
	asr r0, r0, #0x14
	strh r0, [r2]
	ldr r0, _08026E78 @ =0x04000018
	strh r3, [r0]
	add r2, #4
	add r4, #0x14
	add r0, r1, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x10
	asr r0, r0, #0x14
	strh r0, [r2]
	ldr r0, _08026E7C @ =0x0400001C
	strh r3, [r0]
	add r2, #4
	ldr r0, _08026E80 @ =0x00000AF6
	add r1, r1, r0
	ldrh r1, [r1]
	lsl r0, r1, #0x10
	asr r0, r0, #0x14
	strh r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
_08026E64: .4byte 0x04000010
_08026E68: .4byte 0x04000012
_08026E6C: .4byte 0x02020310
_08026E70: .4byte 0x00000ABA
_08026E74: .4byte 0x04000014
_08026E78: .4byte 0x04000018
_08026E7C: .4byte 0x0400001C
_08026E80: .4byte 0x00000AF6
	thumb_func_end sub_08026E10

