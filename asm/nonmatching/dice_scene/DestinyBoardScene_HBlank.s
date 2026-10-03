	thumb_func_start DestinyBoardScene_HBlank
DestinyBoardScene_HBlank: @ 0x08026CC8
	push {r4, r5, lr}
	ldr r5, _08026D18 @ =0x080823C4
	ldr r0, _08026D1C @ =0x02020310
	ldr r1, _08026D20 @ =0x00000B06
	add r4, r0, r1
	ldrb r2, [r4]
	ldr r3, _08026D24 @ =0x04000006
	ldrh r0, [r3]
	lsr r0, r0, #1
	add r1, r2, r0
	add r0, r1, #0
	asr r0, r0, #6
	lsl r0, r0, #6
	sub r0, r1, r0
	add r0, r0, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	add r0, r0, r2
	ldr r1, _08026D28 @ =0x04000010
	strh r0, [r1]
	ldrb r2, [r4]
	ldrh r0, [r3]
	lsr r0, r0, #2
	add r1, r2, r0
	add r0, r1, #0
	asr r0, r0, #6
	lsl r0, r0, #6
	sub r0, r1, r0
	add r0, r0, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	sub r0, r2, r0
	ldr r1, _08026D2C @ =0x0400001C
	strh r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026D18: .4byte gDestinyBoardWaveTable
_08026D1C: .4byte 0x02020310
_08026D20: .4byte 0x00000B06
_08026D24: .4byte 0x04000006
_08026D28: .4byte 0x04000010
_08026D2C: .4byte 0x0400001C
	thumb_func_end DestinyBoardScene_HBlank

