	thumb_func_start sub_08008860
sub_08008860: @ 0x08008860
	push {r4, r5, lr}
	mov r3, #0
	mov r2, #0
	mov r1, #1
	and r1, r0
	ldr r0, _0800889C @ =0x00000D64
	mul r1, r0
	mov r5, #0x94
	ldr r4, _080088A0 @ =0x0201930C
_08008872:
	add r0, r2, #0
	mul r0, r5
	add r0, r0, r1
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08008888
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
_08008888:
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #4
	bls _08008872
	add r0, r3, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0800889C: .4byte 0x00000D64
_080088A0: .4byte 0x0201930C
	thumb_func_end sub_08008860

