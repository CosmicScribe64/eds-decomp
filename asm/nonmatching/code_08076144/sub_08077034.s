	thumb_func_start sub_08077034
sub_08077034: @ 0x08077034
	push {r4, r5, r6, lr}
	mov r4, #0
	ldr r2, _0807706C @ =0x02011C20
	mov r3, #0
	add r5, r2, #0
	ldr r1, _08077070 @ =0x000010B5
_08077040:
	ldrh r6, [r2]
	add r0, r6, r4
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, r1
	bls _08077040
	ldr r0, _08077074 @ =0x0000216E
	add r1, r5, r0
	mvn r0, r4
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrh r1, [r1]
	cmp r1, r0
	beq _08077078
	mov r0, #0
	b _0807707A
	.align 2, 0
_0807706C: .4byte 0x02011C20
_08077070: .4byte 0x000010B5
_08077074: .4byte 0x0000216E
_08077078:
	mov r0, #1
_0807707A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08077034

