	thumb_func_start sub_08009BA8
sub_08009BA8: @ 0x08009BA8
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r2, #0
	ldr r5, _08009BE8 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08009BEC @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	add r1, r3, r5
	ldrb r0, [r1, #4]
	cmp r2, r0
	bge _08009BFE
	ldr r7, _08009BF0 @ =0x00000904
	add r0, r5, r7
	add r5, r1, #0
	add r1, r3, r0
_08009BCE:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, r6
	bne _08009BF4
	add r0, r4, #0
	add r1, r2, #0
	bl sub_08009A68
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08009C00
	.align 2, 0
_08009BE8: .4byte 0x020192E4
_08009BEC: .4byte 0x00000D64
_08009BF0: .4byte 0x00000904
_08009BF4:
	add r1, #4
	add r2, #1
	ldrb r0, [r5, #4]
	cmp r2, r0
	blt _08009BCE
_08009BFE:
	mov r0, #0
_08009C00:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08009BA8
	.align 2, 0

