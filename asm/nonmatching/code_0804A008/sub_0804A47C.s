	thumb_func_start sub_0804A47C
sub_0804A47C: @ 0x0804A47C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r5, #0
	ldr r0, _0804A4F8 @ =0x0201930C
	mov r8, r0
	mov r1, #1
	mov sl, r1
	ldr r0, _0804A4FC @ =0x00000D64
	mov r9, r0
	ldr r1, _0804A500 @ =0x0000047A
	mov ip, r1
_0804A49C:
	mov r4, #5
	add r0, r5, #0
	mov r1, sl
	and r0, r1
	mov r6, r9
	mul r6, r0
_0804A4A8:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	mov r1, r8
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0804A50C
	mov r0, #2
	ldrb r1, [r3, #6]
	and r0, r1
	cmp r0, #0
	beq _0804A50C
	add r1, r3, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0804A50C
	ldr r0, _0804A504 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0804A508 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, ip
	bne _0804A50C
	add r0, r3, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1B
	cmp r0, r7
	bne _0804A50C
	mov r0, #1
	b _0804A51A
	.align 2, 0
_0804A4F8: .4byte 0x0201930C
_0804A4FC: .4byte 0x00000D64
_0804A500: .4byte 0x0000047A
_0804A504: .4byte 0x000007FF
_0804A508: .4byte gUnk_08622AB4
_0804A50C:
	add r4, #1
	cmp r4, #9
	ble _0804A4A8
	add r5, #1
	cmp r5, #1
	ble _0804A49C
	mov r0, #0
_0804A51A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804A47C

