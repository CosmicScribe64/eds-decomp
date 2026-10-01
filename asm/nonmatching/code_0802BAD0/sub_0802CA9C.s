	thumb_func_start sub_0802CA9C
sub_0802CA9C: @ 0x0802CA9C
	push {r4, lr}
	ldrb r3, [r0, #2]
	mov r1, #1
	add r0, r1, #0
	and r0, r3
	mov r4, #0x43
	cmp r0, #0
	beq _0802CAAE
	ldr r4, _0802CADC @ =0x00008043
_0802CAAE:
	ldr r2, _0802CAE0 @ =0x020192E4
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	and r1, r0
	ldr r0, _0802CAE4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_080754A4
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CADC: .4byte 0x00008043
_0802CAE0: .4byte 0x020192E4
_0802CAE4: .4byte 0x00000D64
	thumb_func_end sub_0802CA9C

