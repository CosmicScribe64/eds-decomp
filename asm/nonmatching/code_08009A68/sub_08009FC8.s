	thumb_func_start sub_08009FC8
sub_08009FC8: @ 0x08009FC8
	push {r4, lr}
	ldr r3, _08009FF4 @ =0x020192E4
	mov r2, #1
	and r2, r0
	ldr r0, _08009FF8 @ =0x00000D64
	mul r0, r2
	add r4, r0, r3
	ldr r2, _08009FFC @ =0x00000684
	add r3, r3, r2
	add r0, r0, r3
	lsl r2, r1, #2
	add r2, r0, r2
	ldrb r4, [r4, #2]
	cmp r1, r4
	bge _08009FEE
	ldr r0, _0800A000 @ =0xFFFFF000
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
_08009FEE:
	pop {r4}
	pop {r0}
	bx r0
_08009FF4: .4byte 0x020192E4
_08009FF8: .4byte 0x00000D64
_08009FFC: .4byte 0x00000684
_0800A000: .4byte 0xFFFFF000
	thumb_func_end sub_08009FC8

