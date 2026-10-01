	thumb_func_start sub_08009B48
sub_08009B48: @ 0x08009B48
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r7, r1, #0
	mov r3, #0
	ldr r2, _08009B8C @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08009B90 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r5, [r1, #4]
	cmp r3, r5
	bge _08009BA0
	ldr r5, _08009B94 @ =0x00000904
	add r5, r5, r2
	mov ip, r5
	add r6, r0, #0
	add r5, r1, #0
_08009B6C:
	mov r0, ip
	add r1, r6, r0
	lsl r0, r3, #2
	add r1, r1, r0
	ldr r2, [r7]
	ldr r0, [r1]
	cmp r2, r0
	bne _08009B98
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08009A68
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08009BA2
	.align 2, 0
_08009B8C: .4byte 0x020192E4
_08009B90: .4byte 0x00000D64
_08009B94: .4byte 0x00000904
_08009B98:
	add r3, #1
	ldrb r0, [r5, #4]
	cmp r3, r0
	blt _08009B6C
_08009BA0:
	mov r0, #0
_08009BA2:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08009B48

