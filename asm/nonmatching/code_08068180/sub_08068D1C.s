	thumb_func_start sub_08068D1C
sub_08068D1C: @ 0x08068D1C
	push {r4, lr}
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r4, r0, #0
	lsl r1, r1, #0x18
	lsr r3, r1, #0x18
	lsl r2, r2, #0x10
	lsr r1, r2, #0x10
	cmp r0, #1
	beq _08068D60
	cmp r0, #1
	bgt _08068D3A
	cmp r0, #0
	beq _08068D40
	b _08068D94
_08068D3A:
	cmp r4, #2
	beq _08068D80
	b _08068D94
_08068D40:
	ldr r2, _08068D58 @ =0x0201DB20
	lsl r0, r1, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08068D5C @ =0x00000644
	add r2, r2, r1
	add r0, r0, r2
	ldrh r0, [r0]
	b _08068D94
	.align 2, 0
_08068D58: .4byte 0x0201DB20
_08068D5C: .4byte 0x00000644
_08068D60:
	ldr r2, _08068D78 @ =0x0201DB20
	lsl r0, r1, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08068D7C @ =0x00000CAE
	add r2, r2, r1
	add r0, r0, r2
	ldrh r0, [r0]
	b _08068D94
	.align 2, 0
_08068D78: .4byte 0x0201DB20
_08068D7C: .4byte 0x00000CAE
_08068D80:
	ldr r2, _08068D9C @ =0x0201DB20
	lsl r0, r1, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08068DA0 @ =0x00000D4E
	add r2, r2, r1
	add r0, r0, r2
	ldrh r0, [r0]
_08068D94:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08068D9C: .4byte 0x0201DB20
_08068DA0: .4byte 0x00000D4E
	thumb_func_end sub_08068D1C

