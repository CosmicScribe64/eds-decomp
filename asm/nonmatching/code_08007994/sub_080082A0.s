	thumb_func_start sub_080082A0
sub_080082A0: @ 0x080082A0
	push {r4, lr}
	add r3, r2, #0
	mov r2, #1
	and r2, r0
	ldr r0, _080082F8 @ =0x00000D64
	mul r2, r0
	ldr r0, _080082FC @ =0x0201930C
	add r2, r2, r0
	mov r0, #0x94
	mul r0, r1
	add r2, r2, r0
	mov r0, #0x8A
	add r0, r0, r2
	mov ip, r0
	ldrh r0, [r0]
	cmp r3, r0
	bge _080082F0
	sub r0, #1
	mov r1, ip
	strh r0, [r1]
	add r4, r0, #0
	cmp r3, r4
	bge _080082F0
	lsl r0, r3, #1
	add r1, r0, #0
	add r1, #0x4A
	add r1, r1, r2
	add r0, #0xA
	add r2, r0, r2
_080082DA:
	ldrh r0, [r2, #2]
	strh r0, [r2]
	ldrh r0, [r1, #2]
	strh r0, [r1]
	add r1, #2
	add r2, #2
	add r3, #1
	mov r0, ip
	ldrh r0, [r0]
	cmp r3, r0
	blt _080082DA
_080082F0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080082F8: .4byte 0x00000D64
_080082FC: .4byte 0x0201930C
	thumb_func_end sub_080082A0

