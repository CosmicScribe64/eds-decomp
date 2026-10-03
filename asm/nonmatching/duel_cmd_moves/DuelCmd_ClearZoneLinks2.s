	thumb_func_start DuelCmd_ClearZoneLinks2
DuelCmd_ClearZoneLinks2: @ 0x08013154
	ldr r2, _08013180 @ =0x020185C0
	ldrh r0, [r2]
	lsr r3, r0, #0xF
	mov r0, #0x94
	ldrh r1, [r2, #2]
	mul r0, r1
	ldr r1, _08013184 @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08013188 @ =0x0201930C
	add r0, r0, r1
	add r0, #0x8A
	mov r1, #0
	strh r1, [r0]
	ldr r0, _0801318C @ =0x0000080D
	add r2, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	bx lr
_08013180: .4byte 0x020185C0
_08013184: .4byte 0x00000D64
_08013188: .4byte 0x0201930C
_0801318C: .4byte 0x0000080D
	thumb_func_end DuelCmd_ClearZoneLinks2

