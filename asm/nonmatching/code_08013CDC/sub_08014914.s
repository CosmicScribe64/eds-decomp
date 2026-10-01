	thumb_func_start sub_08014914
sub_08014914: @ 0x08014914
	push {r4, r5, lr}
	ldr r1, _08014954 @ =0x020192E4
	ldr r4, _08014958 @ =0x020185C0
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r2, [r4]
	and r0, r2
	mov r3, #0
	cmp r0, #0
	beq _0801492A
	ldr r3, _0801495C @ =0x00000D64
_0801492A:
	add r3, r3, r1
	mov r1, #1
	ldrb r5, [r4, #2]
	and r1, r5
	lsl r1, r1, #5
	mov r2, #0x21
	neg r2, r2
	add r0, r2, #0
	ldrb r5, [r3, #7]
	and r0, r5
	orr r0, r1
	strb r0, [r3, #7]
	ldr r1, _08014960 @ =0x0000080D
	add r0, r4, r1
	ldrb r5, [r0]
	and r2, r5
	strb r2, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08014954: .4byte 0x020192E4
_08014958: .4byte 0x020185C0
_0801495C: .4byte 0x00000D64
_08014960: .4byte 0x0000080D
	thumb_func_end sub_08014914

