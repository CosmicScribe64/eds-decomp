	thumb_func_start sub_08014804
sub_08014804: @ 0x08014804
	ldr r3, _08014834 @ =0x020192E4
	ldr r2, _08014838 @ =0x020185C0
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0
	cmp r0, #0
	beq _08014818
	ldr r1, _0801483C @ =0x00000D64
_08014818:
	add r1, r1, r3
	mov r0, #4
	ldrb r3, [r1, #9]
	orr r0, r3
	strb r0, [r1, #9]
	ldr r0, _08014840 @ =0x0000080D
	add r1, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08014834: .4byte 0x020192E4
_08014838: .4byte 0x020185C0
_0801483C: .4byte 0x00000D64
_08014840: .4byte 0x0000080D
	thumb_func_end sub_08014804

