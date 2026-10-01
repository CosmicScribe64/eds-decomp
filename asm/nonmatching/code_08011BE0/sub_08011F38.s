	thumb_func_start sub_08011F38
sub_08011F38: @ 0x08011F38
	push {r4, r5, r6, lr}
	ldr r0, _08011F64 @ =0x020185C0
	ldrh r1, [r0]
	lsr r5, r1, #0xF
	ldrh r4, [r0, #2]
	ldrh r3, [r0, #4]
	mov r1, #0xFA
	lsl r1, r1, #1
	add r6, r0, #0
	cmp r3, r1
	bne _08011F70
	ldr r2, _08011F68 @ =0x020192E4
	lsl r0, r4, #1
	ldr r1, _08011F6C @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	add r2, #0xE
	add r0, r0, r2
	ldrh r2, [r0]
	add r1, r3, r2
	strh r1, [r0]
	b _08011F80
_08011F64: .4byte 0x020185C0
_08011F68: .4byte 0x020192E4
_08011F6C: .4byte 0x00000D64
_08011F70:
	ldr r2, _08011F94 @ =0x020192E4
	lsl r0, r4, #1
	ldr r1, _08011F98 @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	add r2, #0xE
	add r0, r0, r2
	strh r3, [r0]
_08011F80:
	ldr r0, _08011F9C @ =0x0000080D
	add r1, r6, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08011F94: .4byte 0x020192E4
_08011F98: .4byte 0x00000D64
_08011F9C: .4byte 0x0000080D
	thumb_func_end sub_08011F38

