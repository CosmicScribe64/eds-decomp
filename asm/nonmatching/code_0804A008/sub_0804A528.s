	thumb_func_start sub_0804A528
sub_0804A528: @ 0x0804A528
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	add r5, r0, #0
	add r6, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0xC]
	mov r4, #1
	and r0, r4
	ldr r1, _0804A678 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	ldr r3, _0804A67C @ =0x0201930C
	add r1, r2, r3
	mov r0, #0x94
	mul r0, r6
	add r1, r1, r0
	mov r9, r1
	ldrb r1, [r1, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	str r0, [sp, #0x10]
	mov r7, r9
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r7, r0, #0x14
	cmp r7, #0
	bne _0804A56A
	b _0804A834
_0804A56A:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _0804A574
	b _0804A834
_0804A574:
	add r0, r3, #0
	sub r0, #0x28
	add r0, r2, r0
	ldrh r0, [r0, #0x26]
	asr r0, r6
	and r0, r4
	cmp r0, #0
	beq _0804A586
	b _0804A834
_0804A586:
	add r0, r5, #0
	add r1, r6, #0
	mov r2, sp
	bl sub_0800ABC8
	mov r0, sp
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1B
	mov r8, r0
	ldr r0, [sp, #4]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov sl, r0
	mov r2, #0xAE
	lsl r2, r2, #1
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804A5B4
	b _0804A834
_0804A5B4:
	ldr r2, _0804A680 @ =0x000004DC
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804A5C4
	b _0804A834
_0804A5C4:
	ldr r2, _0804A684 @ =0x0000060C
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A8CC
	cmp r0, #0
	beq _0804A5D4
	b _0804A834
_0804A5D4:
	ldr r2, _0804A688 @ =0x00000417
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A8CC
	cmp r0, #0
	beq _0804A5EA
	mov r0, r8
	cmp r0, #7
	beq _0804A5EA
	b _0804A834
_0804A5EA:
	mov r0, r8
	bl sub_0804A47C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804A5F8
	b _0804A834
_0804A5F8:
	ldr r2, _0804A68C @ =0x000002D9
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804A608
	b _0804A834
_0804A608:
	ldr r2, _0804A690 @ =0x00000534
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804A618
	b _0804A834
_0804A618:
	ldr r4, _0804A694 @ =0x0000046B
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0804A632
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _0804A63A
_0804A632:
	ldr r0, _0804A698 @ =0x000005DB
	cmp sl, r0
	bls _0804A63A
	b _0804A834
_0804A63A:
	ldr r4, _0804A69C @ =0x0000052B
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0804A654
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _0804A6CA
_0804A654:
	ldr r0, _0804A6A0 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0804A6A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0804A6B0
	cmp r0, #0x17
	ble _0804A6A8
	cmp r0, #0x18
	beq _0804A6AC
	b _0804A6B0
	.align 2, 0
_0804A678: .4byte 0x00000D64
_0804A67C: .4byte 0x0201930C
_0804A680: .4byte 0x000004DC
_0804A684: .4byte 0x0000060C
_0804A688: .4byte 0x00000417
_0804A68C: .4byte 0x000002D9
_0804A690: .4byte 0x00000534
_0804A694: .4byte 0x0000046B
_0804A698: .4byte 0x000005DB
_0804A69C: .4byte 0x0000052B
_0804A6A0: .4byte 0x000007FF
_0804A6A4: .4byte gUnk_08621DE0
_0804A6A8:
	mov r0, #0
	b _0804A6C4
_0804A6AC:
	mov r0, #0xA
	b _0804A6C4
_0804A6B0:
	ldr r0, _0804A768 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _0804A76C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0804A6C4:
	cmp r0, #3
	bls _0804A6CA
	b _0804A834
_0804A6CA:
	mov r0, #1
	sub r0, r0, r5
	mov r1, #0xA9
	lsl r1, r1, #3
	bl sub_08008524
	cmp r0, #0
	ble _0804A6E2
	mov r0, r8
	cmp r0, #0xA
	bne _0804A6E2
	b _0804A834
_0804A6E2:
	ldr r2, _0804A770 @ =0x0000058B
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804A6F2
	b _0804A834
_0804A6F2:
	ldr r0, _0804A768 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0804A774 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0804A778 @ =0x000005F3
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804A712
	mov r0, #1
	sub r0, r0, r5
	bl sub_08008860
	cmp r0, #0
	bne _0804A712
	b _0804A834
_0804A712:
	ldr r4, _0804A77C @ =0x00000536
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0804A72C
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _0804A73E
_0804A72C:
	ldr r0, _0804A768 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _0804A774 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r4
	beq _0804A73E
	b _0804A834
_0804A73E:
	ldr r0, [sp, #0xC]
	cmp r0, #0
	beq _0804A7F0
	ldr r0, _0804A768 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0804A774 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xB6
	lsl r0, r0, #2
	cmp r1, r0
	bgt _0804A780
	sub r0, #2
	cmp r1, r0
	bge _0804A7C8
	sub r0, #0xB0
	cmp r1, r0
	beq _0804A7A4
	b _0804A7F0
	.align 2, 0
_0804A768: .4byte 0x000007FF
_0804A76C: .4byte gUnk_08621DE0
_0804A770: .4byte 0x0000058B
_0804A774: .4byte gUnk_08622AB4
_0804A778: .4byte 0x000005F3
_0804A77C: .4byte 0x00000536
_0804A780:
	ldr r0, _0804A794 @ =0x000002F9
	cmp r1, r0
	beq _0804A7E4
	cmp r1, r0
	bgt _0804A798
	sub r0, #0x11
	cmp r1, r0
	beq _0804A7E4
	b _0804A7F0
	.align 2, 0
_0804A794: .4byte 0x000002F9
_0804A798:
	ldr r0, _0804A7A0 @ =0x000002FE
	cmp r1, r0
	beq _0804A7C8
	b _0804A7F0
_0804A7A0: .4byte 0x000002FE
_0804A7A4:
	ldr r2, _0804A7BC @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _0804A7C0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0804A7C4 @ =0x000003E7
_0804A7B2:
	ldrh r0, [r0]
	cmp r0, r1
	bhi _0804A7F0
	b _0804A834
	.align 2, 0
_0804A7BC: .4byte 0x020192E4
_0804A7C0: .4byte 0x00000D64
_0804A7C4: .4byte 0x000003E7
_0804A7C8:
	ldr r2, _0804A7D8 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _0804A7DC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0804A7E0 @ =0x000001F3
	b _0804A7B2
_0804A7D8: .4byte 0x020192E4
_0804A7DC: .4byte 0x00000D64
_0804A7E0: .4byte 0x000001F3
_0804A7E4:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08008AF8
	cmp r0, #0
	beq _0804A834
_0804A7F0:
	ldr r0, _0804A82C @ =0x000007FF
	and r7, r0
	lsl r0, r7, #1
	ldr r2, _0804A830 @ =0x08622AB4
	add r0, r0, r2
	mov r1, #0xC7
	lsl r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804A808
	mov r7, #0
	str r7, [sp, #0x10]
_0804A808:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0804A834
	mov r0, #0x10
	mov r1, r9
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _0804A834
	mov r1, r9
	add r1, #0x8C
	mov r0, #0x18
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0804A834
	mov r0, #1
	b _0804A836
_0804A82C: .4byte 0x000007FF
_0804A830: .4byte gUnk_08622AB4
_0804A834:
	mov r0, #0
_0804A836:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804A528
	.align 2, 0

