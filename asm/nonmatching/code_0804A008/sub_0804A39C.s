	thumb_func_start sub_0804A39C
sub_0804A39C: @ 0x0804A39C
	push {r4, r5, r6, lr}
	mov r4, #1
	and r0, r4
	mov r2, #0x94
	add r3, r1, #0
	mul r3, r2
	ldr r2, _0804A3D0 @ =0x00000D64
	add r5, r0, #0
	mul r5, r2
	add r3, r3, r5
	ldr r2, _0804A3D4 @ =0x0201930C
	add r3, r3, r2
	mov r0, #4
	ldrb r6, [r3, #7]
	orr r0, r6
	strb r0, [r3, #7]
	sub r2, #0x28
	add r5, r5, r2
	lsl r4, r1
	ldrh r0, [r5, #0x26]
	orr r4, r0
	strh r4, [r5, #0x26]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0804A3D0: .4byte 0x00000D64
_0804A3D4: .4byte 0x0201930C
	thumb_func_end sub_0804A39C

