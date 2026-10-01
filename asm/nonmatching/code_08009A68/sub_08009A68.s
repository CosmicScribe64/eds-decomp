	thumb_func_start sub_08009A68
sub_08009A68: @ 0x08009A68
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r3, r1, #0
	ldr r6, _08009AB8 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009ABC @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
	add r2, r5, r6
	ldrb r0, [r2, #4]
	cmp r3, r0
	bge _08009AC4
	sub r0, #1
	strb r0, [r2, #4]
	add r4, r3, #0
	cmp r4, r0
	bge _08009AB2
	ldr r0, _08009AC0 @ =0x00000904
	add r1, r6, r0
	mov r8, r2
	lsl r0, r4, #2
	add r6, r0, #4
	add r7, r5, r1
	add r5, r0, r7
_08009A9C:
	add r1, r7, r6
	add r0, r5, #0
	bl sub_08007558
	add r6, #4
	add r5, #4
	add r4, #1
	mov r1, r8
	ldrb r1, [r1, #4]
	cmp r4, r1
	blt _08009A9C
_08009AB2:
	mov r0, #1
	b _08009AC6
	.align 2, 0
_08009AB8: .4byte 0x020192E4
_08009ABC: .4byte 0x00000D64
_08009AC0: .4byte 0x00000904
_08009AC4:
	mov r0, #0
_08009AC6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08009A68

