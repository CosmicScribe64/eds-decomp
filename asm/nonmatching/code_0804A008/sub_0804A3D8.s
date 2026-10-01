	thumb_func_start sub_0804A3D8
sub_0804A3D8: @ 0x0804A3D8
	push {r4, lr}
	add r3, r0, #0
	mov r4, #1
	add r2, r3, #0
	and r2, r4
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0804A418 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0804A41C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804A420 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804A424 @ =0x000001E7
	cmp r1, r0
	beq _0804A464
	cmp r1, r0
	bgt _0804A430
	sub r0, #0x5A
	cmp r1, r0
	bgt _0804A428
	sub r0, #1
	cmp r1, r0
	bge _0804A464
	sub r0, #0xA
	b _0804A440
	.align 2, 0
_0804A418: .4byte 0x00000D64
_0804A41C: .4byte 0x0201930C
_0804A420: .4byte gUnk_08622AB4
_0804A424: .4byte 0x000001E7
_0804A428:
	ldr r0, _0804A42C @ =0x000001A5
	b _0804A440
_0804A42C: .4byte 0x000001A5
_0804A430:
	mov r0, #0xB6
	lsl r0, r0, #2
	cmp r1, r0
	bgt _0804A446
	sub r0, #2
	cmp r1, r0
	bge _0804A458
	sub r0, #0x5C
_0804A440:
	cmp r1, r0
	beq _0804A464
	b _0804A474
_0804A446:
	ldr r0, _0804A454 @ =0x000002FE
	cmp r1, r0
	beq _0804A458
	add r0, #0x2E
	cmp r1, r0
	beq _0804A468
	b _0804A474
_0804A454: .4byte 0x000002FE
_0804A458:
	mov r0, #1
	sub r0, r0, r3
	bl sub_08008F74
	cmp r0, #0
	bne _0804A474
_0804A464:
	mov r0, #1
	b _0804A476
_0804A468:
	sub r0, r4, r3
	bl sub_08008FDC
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0804A476
_0804A474:
	mov r0, #0
_0804A476:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0804A3D8

