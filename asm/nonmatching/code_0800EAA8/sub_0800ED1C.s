	thumb_func_start sub_0800ED1C
sub_0800ED1C: @ 0x0800ED1C
	ldr r2, _0800ED48 @ =0x020185C0
	ldrh r0, [r2]
	lsr r3, r0, #0xF
	mov r0, #0x94
	ldrh r1, [r2, #2]
	mul r0, r1
	ldr r1, _0800ED4C @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _0800ED50 @ =0x0201930C
	add r0, r0, r1
	add r0, #0x8A
	mov r1, #0
	strh r1, [r0]
	ldr r0, _0800ED54 @ =0x0000080D
	add r2, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	bx lr
_0800ED48: .4byte 0x020185C0
_0800ED4C: .4byte 0x00000D64
_0800ED50: .4byte 0x0201930C
_0800ED54: .4byte 0x0000080D
	thumb_func_end sub_0800ED1C

