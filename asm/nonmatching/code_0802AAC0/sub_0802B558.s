	thumb_func_start sub_0802B558
sub_0802B558: @ 0x0802B558
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	ldrh r0, [r0]
	mov r9, r0
	lsl r0, r1, #8
	lsr r6, r0, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	mov r0, #1
	and r0, r6
	ldr r1, _0802B634 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov sl, r2
	ldr r0, _0802B638 @ =0x0201930C
	add r0, sl
	mov r1, #0x94
	mov r7, r8
	mul r7, r1
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	add r0, r6, #0
	mov r1, r8
	bl sub_0800C8BC
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r6, #0
	mov r1, r8
	bl sub_0800CAF0
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, r8
	cmp r0, #4
	ble _0802B5B2
	b _0802B97A
_0802B5B2:
	mov r1, sl
	add r0, r7, r1
	ldr r2, _0802B638 @ =0x0201930C
	add r1, r0, r2
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0802B5C4
	b _0802B97A
_0802B5C4:
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0802B5D0
	b _0802B97A
_0802B5D0:
	ldr r1, [sp, #0]
	ldrh r0, [r1]
	add r1, r6, #0
	mov r2, r8
	str r3, [sp, #4]
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	ldr r3, [sp, #4]
	cmp r0, #0
	bne _0802B5E8
	b _0802B97A
_0802B5E8:
	ldr r2, _0802B63C @ =0x000007FF
	mov r0, r9
	and r0, r2
	lsl r0, r0, #1
	ldr r7, _0802B640 @ =0x08622AB4
	add r0, r0, r7
	ldrh r1, [r0]
	ldr r0, _0802B644 @ =0x00000147
	cmp r1, r0
	bne _0802B5FE
	b _0802B8D0
_0802B5FE:
	cmp r1, r0
	ble _0802B604
	b _0802B6F8
_0802B604:
	sub r0, #0x10
	cmp r1, r0
	bne _0802B60C
	b _0802B8BC
_0802B60C:
	cmp r1, r0
	bgt _0802B68C
	sub r0, #6
	cmp r1, r0
	bgt _0802B654
	sub r0, #1
	cmp r1, r0
	blt _0802B61E
	b _0802B8A8
_0802B61E:
	sub r0, #3
	cmp r1, r0
	bne _0802B626
	b _0802B970
_0802B626:
	cmp r1, r0
	bgt _0802B648
	cmp r1, #0x47
	bne _0802B630
	b _0802B82C
_0802B630:
	sub r0, #1
	b _0802B80E
_0802B634: .4byte 0x00000D64
_0802B638: .4byte 0x0201930C
_0802B63C: .4byte 0x000007FF
_0802B640: .4byte gUnk_08622AB4
_0802B644: .4byte 0x00000147
_0802B648:
	mov r0, #0x97
	lsl r0, r0, #1
	cmp r1, r0
	bne _0802B652
	b _0802B934
_0802B652:
	b _0802B67C
_0802B654:
	mov r0, #0x9A
	lsl r0, r0, #1
	cmp r1, r0
	bne _0802B65E
	b _0802B948
_0802B65E:
	cmp r1, r0
	bgt _0802B674
	sub r0, #2
	cmp r1, r0
	bne _0802B66A
	b _0802B95C
_0802B66A:
	add r0, #1
	cmp r1, r0
	bne _0802B672
	b _0802B92A
_0802B672:
	b _0802B97A
_0802B674:
	ldr r0, _0802B688 @ =0x00000135
	cmp r1, r0
	bne _0802B67C
	b _0802B90C
_0802B67C:
	add r0, #1
_0802B67E:
	cmp r1, r0
	bne _0802B684
	b _0802B89E
_0802B684:
	b _0802B97A
	.align 2, 0
_0802B688: .4byte 0x00000135
_0802B68C:
	mov r0, #0xA0
	lsl r0, r0, #1
	cmp r1, r0
	bne _0802B696
	b _0802B89E
_0802B696:
	cmp r1, r0
	bgt _0802B6C0
	sub r0, #6
	cmp r1, r0
	bne _0802B6A2
	b _0802B8F8
_0802B6A2:
	cmp r1, r0
	bge _0802B6A8
	b _0802B89E
_0802B6A8:
	add r0, #2
	cmp r1, r0
	bne _0802B6B0
	b _0802B860
_0802B6B0:
	cmp r1, r0
	bge _0802B6B6
	b _0802B8DA
_0802B6B6:
	add r0, #2
	cmp r1, r0
	bne _0802B6BE
	b _0802B916
_0802B6BE:
	b _0802B97A
_0802B6C0:
	ldr r0, _0802B6E0 @ =0x00000143
	cmp r1, r0
	bne _0802B6C8
	b _0802B93E
_0802B6C8:
	cmp r1, r0
	bgt _0802B6E4
	sub r0, #2
	cmp r1, r0
	bne _0802B6D4
	b _0802B920
_0802B6D4:
	add r0, #1
	cmp r1, r0
	bne _0802B6DC
	b _0802B8EE
_0802B6DC:
	b _0802B97A
	.align 2, 0
_0802B6E0: .4byte 0x00000143
_0802B6E4:
	ldr r0, _0802B6F4 @ =0x00000145
	cmp r1, r0
	bne _0802B6EC
	b _0802B8C6
_0802B6EC:
	cmp r1, r0
	ble _0802B6F2
	b _0802B902
_0802B6F2:
	b _0802B8B2
_0802B6F4: .4byte 0x00000145
_0802B6F8:
	ldr r0, _0802B730 @ =0x00000422
	cmp r1, r0
	bne _0802B700
	b _0802B818
_0802B700:
	cmp r1, r0
	bgt _0802B7A4
	ldr r0, _0802B734 @ =0x0000029B
	cmp r1, r0
	bne _0802B70C
	b _0802B95C
_0802B70C:
	cmp r1, r0
	bgt _0802B750
	sub r0, #0xE
	cmp r1, r0
	bne _0802B718
	b _0802B952
_0802B718:
	cmp r1, r0
	bgt _0802B738
	sub r0, #3
	cmp r1, r0
	bne _0802B724
	b _0802B818
_0802B724:
	add r0, #1
	cmp r1, r0
	bne _0802B72C
	b _0802B87C
_0802B72C:
	b _0802B97A
	.align 2, 0
_0802B730: .4byte 0x00000422
_0802B734: .4byte 0x0000029B
_0802B738:
	ldr r0, _0802B74C @ =0x00000291
	cmp r1, r0
	ble _0802B740
	b _0802B97A
_0802B740:
	sub r0, #1
	cmp r1, r0
	bge _0802B748
	b _0802B97A
_0802B748:
	b _0802B89E
	.align 2, 0
_0802B74C: .4byte 0x00000291
_0802B750:
	ldr r0, _0802B770 @ =0x000003F5
	cmp r1, r0
	bne _0802B758
	b _0802B966
_0802B758:
	cmp r1, r0
	bgt _0802B774
	sub r0, #0x33
	cmp r1, r0
	bne _0802B764
	b _0802B8B2
_0802B764:
	add r0, #0x32
	cmp r1, r0
	bne _0802B76C
	b _0802B952
_0802B76C:
	b _0802B97A
	.align 2, 0
_0802B770: .4byte 0x000003F5
_0802B774:
	ldr r0, _0802B7A0 @ =0x00000412
	cmp r1, r0
	bne _0802B77C
	b _0802B89E
_0802B77C:
	cmp r1, r0
	bge _0802B782
	b _0802B97A
_0802B782:
	add r0, #5
	cmp r1, r0
	ble _0802B78A
	b _0802B97A
_0802B78A:
	sub r0, #1
	cmp r1, r0
	bge _0802B792
	b _0802B97A
_0802B792:
	mov r1, #7
	eor r1, r4
	neg r0, r1
	orr r0, r1
	lsr r0, r0, #0x1F
	b _0802B97C
	.align 2, 0
_0802B7A0: .4byte 0x00000412
_0802B7A4:
	ldr r0, _0802B7C8 @ =0x0000058C
	cmp r1, r0
	bgt _0802B7E0
	sub r0, #1
	cmp r1, r0
	blt _0802B7B2
	b _0802B89E
_0802B7B2:
	sub r0, #0xA1
	cmp r1, r0
	beq _0802B89E
	cmp r1, r0
	bgt _0802B7CC
	sub r0, #0xC6
	cmp r1, r0
	beq _0802B89E
	add r0, #0x7A
	b _0802B80E
	.align 2, 0
_0802B7C8: .4byte 0x0000058C
_0802B7CC:
	ldr r0, _0802B7DC @ =0x00000521
	cmp r1, r0
	beq _0802B89E
	add r0, #1
	cmp r1, r0
	beq _0802B8B2
	b _0802B97A
	.align 2, 0
_0802B7DC: .4byte 0x00000521
_0802B7E0:
	ldr r0, _0802B7F0 @ =0x000005AA
	cmp r1, r0
	bgt _0802B7F4
	sub r0, #2
	cmp r1, r0
	bge _0802B89E
	sub r0, #0x1A
	b _0802B80E
_0802B7F0: .4byte 0x000005AA
_0802B7F4:
	ldr r0, _0802B808 @ =0x0000060C
	cmp r1, r0
	beq _0802B89E
	cmp r1, r0
	bgt _0802B80C
	sub r0, #8
	cmp r1, r0
	beq _0802B890
	b _0802B97A
	.align 2, 0
_0802B808: .4byte 0x0000060C
_0802B80C:
	ldr r0, _0802B814 @ =0x0000060E
_0802B80E:
	cmp r1, r0
	beq _0802B8E4
	b _0802B97A
_0802B814: .4byte 0x0000060E
_0802B818:
	mov r1, #0
	ldr r2, [sp, #0]
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r6
	bne _0802B828
	mov r1, #1
_0802B828:
	add r0, r1, #0
	b _0802B97C
_0802B82C:
	and r5, r2
	lsl r0, r5, #1
	add r0, r0, r7
	ldr r1, _0802B85C @ =0x00000115
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802B83C
	b _0802B97A
_0802B83C:
	ldr r3, [sp, #0]
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r6
	beq _0802B84A
	b _0802B97A
_0802B84A:
	add r0, r6, #0
	mov r1, r8
	mov r2, #0x47
	bl sub_0800A78C
	cmp r0, #0
	beq _0802B85A
	b _0802B97A
_0802B85A:
	b _0802B89E
_0802B85C: .4byte 0x00000115
_0802B860:
	and r5, r2
	lsl r0, r5, #1
	add r0, r0, r7
	ldrh r1, [r0]
	cmp r1, #0x3D
	bge _0802B86E
	b _0802B97A
_0802B86E:
	cmp r1, #0x3E
	ble _0802B89E
	ldr r0, _0802B878 @ =0x000004E1
	b _0802B67E
	.align 2, 0
_0802B878: .4byte 0x000004E1
_0802B87C:
	and r5, r2
	lsl r0, r5, #1
	add r0, r0, r7
	ldr r1, _0802B88C @ =0x0000016D
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802B89E
	b _0802B97A
_0802B88C: .4byte 0x0000016D
_0802B890:
	and r5, r2
	lsl r0, r5, #1
	add r0, r0, r7
	ldr r1, _0802B8A4 @ =0x0000053B
	ldrh r0, [r0]
	cmp r0, r1
	bne _0802B97A
_0802B89E:
	mov r0, #1
	b _0802B97C
	.align 2, 0
_0802B8A4: .4byte 0x0000053B
_0802B8A8:
	mov r0, #0
	cmp r4, #0xA
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8B2:
	mov r0, #0
	cmp r4, #7
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8BC:
	mov r0, #0
	cmp r4, #0x11
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8C6:
	mov r0, #0
	cmp r4, #9
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8D0:
	mov r0, #0
	cmp r4, #0xE
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8DA:
	mov r0, #0
	cmp r4, #0x13
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8E4:
	mov r0, #0
	cmp r4, #0xF
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8EE:
	mov r0, #0
	cmp r4, #0x12
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B8F8:
	mov r0, #0
	cmp r4, #1
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B902:
	mov r0, #0
	cmp r4, #0x10
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B90C:
	mov r0, #0
	cmp r4, #0xD
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B916:
	mov r0, #0
	cmp r4, #0xC
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B920:
	mov r0, #0
	cmp r4, #2
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B92A:
	mov r0, #0
	cmp r4, #0xB
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B934:
	mov r0, #0
	cmp r4, #3
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B93E:
	mov r0, #0
	cmp r3, #5
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B948:
	mov r0, #0
	cmp r3, #3
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B952:
	mov r0, #0
	cmp r3, #4
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B95C:
	mov r0, #0
	cmp r3, #1
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B966:
	mov r0, #0
	cmp r3, #6
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B970:
	mov r0, #0
	cmp r3, #2
	bne _0802B97C
	mov r0, #1
	b _0802B97C
_0802B97A:
	mov r0, #0
_0802B97C:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802B558

