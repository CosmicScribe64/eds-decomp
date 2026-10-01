	thumb_func_start sub_08056D98
sub_08056D98: @ 0x08056D98
	push {r4, r5, r6, r7, lr}
	add r4, r2, #0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r3, #0
	ldr r5, _08056DDC @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08056DE0 @ =0x00000D64
	add r2, r1, #0
	mul r2, r0
	add r0, r2, r5
	ldrb r1, [r0, #3]
	cmp r3, r1
	bge _08056DF8
	cmp r3, r4
	bge _08056DF8
	ldr r7, _08056DE4 @ =0x000007C4
	add r0, r5, r7
	add r5, r1, #0
	add r1, r2, r0
	add r7, #0x3B
	ldr r2, _08056DE8 @ =0x08622AB4
_08056DC6:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r6
	bne _08056DEC
	add r0, r3, #0
	b _08056DFC
_08056DDC: .4byte 0x020192E4
_08056DE0: .4byte 0x00000D64
_08056DE4: .4byte 0x000007C4
_08056DE8: .4byte gUnk_08622AB4
_08056DEC:
	add r1, #4
	add r3, #1
	cmp r3, r5
	bge _08056DF8
	cmp r3, r4
	blt _08056DC6
_08056DF8:
	mov r0, #1
	neg r0, r0
_08056DFC:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08056D98
	.align 2, 0

