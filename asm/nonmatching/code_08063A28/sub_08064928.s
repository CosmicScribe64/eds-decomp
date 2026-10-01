	thumb_func_start sub_08064928
sub_08064928: @ 0x08064928
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl sub_080648D4
	add r1, r0, #0
	mov r0, #0x62
	mul r0, r4
	lsl r0, r0, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xD
	add r0, r0, r2
	lsr r0, r0, #0x10
	cmp r1, #0
	beq _08064972
	ldr r2, _08064978 @ =0x040000D4
	str r1, [r2]
	lsl r0, r0, #6
	ldr r1, _0806497C @ =0x0202037E
	add r0, r0, r1
	str r0, [r2, #4]
	ldr r0, _08064980 @ =0x80000C40
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r0, [r2, #8]
	mov r1, #0x80
	lsl r1, r1, #0x18
	cmp r0, #0
	bge _08064972
_0806496A:
	ldr r0, [r2, #8]
	and r0, r1
	cmp r0, #0
	bne _0806496A
_08064972:
	pop {r4}
	pop {r0}
	bx r0
_08064978: .4byte 0x040000D4
_0806497C: .4byte 0x0202037E
_08064980: .4byte 0x80000C40
	thumb_func_end sub_08064928

