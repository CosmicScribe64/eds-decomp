	thumb_func_start sub_0805ED9C
sub_0805ED9C: @ 0x0805ED9C
	push {r4, r5, lr}
	ldr r5, _0805EDD4 @ =0x0201CFB8
	mov r4, #0x3F
_0805EDA2:
	mov r0, #0x20
	add r1, r5, #0
	mov r2, #0
	mov r3, #9
	bl sub_08072778
	add r5, #0x20
	sub r4, #1
	cmp r4, #0
	bge _0805EDA2
	ldr r0, _0805EDD8 @ =0x0201CFB0
	ldr r1, _0805EDDC @ =0x00000808
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	orr r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	bl sub_0805ED78
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0805EDD4: .4byte 0x0201CFB8
_0805EDD8: .4byte 0x0201CFB0
_0805EDDC: .4byte 0x00000808
	thumb_func_end sub_0805ED9C

