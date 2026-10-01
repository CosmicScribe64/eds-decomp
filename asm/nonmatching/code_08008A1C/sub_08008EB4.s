	thumb_func_start sub_08008EB4
sub_08008EB4: @ 0x08008EB4
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	mov r0, #1
	and r0, r5
	ldr r1, _08008EE8 @ =0x00000D64
	mul r1, r0
	ldr r0, _08008EEC @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r6
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl sub_08007994
	cmp r0, #0
	beq _08008EF0
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08007D18
	b _08008EFC
_08008EE8: .4byte 0x00000D64
_08008EEC: .4byte 0x0201930C
_08008EF0:
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08009EAC
_08008EFC:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08008D3C
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08008278
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_08008EB4
	.align 2, 0

