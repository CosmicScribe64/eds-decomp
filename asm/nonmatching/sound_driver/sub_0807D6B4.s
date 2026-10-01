	thumb_func_start sub_0807D6B4
sub_0807D6B4: @ 0x0807D6B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r3, r0, #0
	add r7, r1, #0
	ldr r0, _0807D724 @ =0x03005210
	mov sl, r0
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #3
	add r0, #0xF8
	mov r1, sl
	add r6, r0, r1
	ldrb r1, [r6, #0x13]
	mov r2, #0x80
	mov r9, r2
	mov r0, r9
	and r0, r1
	cmp r0, #0
	bne _0807D6E2
	b _0807DB4A
_0807D6E2:
	mov r4, #0
	strh r4, [r7, #4]
	ldr r5, [r6]
	ldrb r2, [r6, #0x13]
	mov r0, #8
	and r0, r2
	cmp r0, #0
	beq _0807D728
	mov r0, #0x15
	ldsb r0, [r6, r0]
	cmp r0, #0
	bge _0807D6FC
	b _0807DB4A
_0807D6FC:
	mov r3, #0x15
	ldsb r3, [r6, r3]
	cmp r3, #0
	beq _0807D718
	mov r1, #0
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #3
	add r0, r0, r6
_0807D70E:
	strb r1, [r0, #0x15]
	sub r0, #0x18
	sub r3, #1
	cmp r3, #0
	bne _0807D70E
_0807D718:
	mov r1, #0
	mov r0, #1
	strh r0, [r6, #0xC]
	mov r5, #0
	strb r1, [r6, #0x13]
	b _0807D824
_0807D724: .4byte 0x03005210
_0807D728:
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, sl
	ldrh r1, [r0]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0807D740
	mov r0, #8
	orr r0, r2
	strb r0, [r6, #0x13]
	b _0807D6FC
_0807D740:
	mov r0, #0x40
	mov r8, r0
	and r0, r2
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0
	bne _0807D794
	mov r0, #0x40
	orr r0, r2
	strb r0, [r6, #0x13]
	strb r4, [r6, #0x15]
	strh r1, [r6, #0xC]
	strb r4, [r6, #0xB]
	str r1, [r6, #4]
	b _0807D824
_0807D75E:
	mov r0, #0xF
	and r2, r0
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #3
	add r0, sl
	mov r2, #0x85
	lsl r2, r2, #1
	add r1, r0, r2
	mov r0, #1
	strb r0, [r1]
	str r5, [r6]
	b _0807DB36
_0807D778:
	mov r0, #0xF
	and r2, r0
	lsl r3, r2, #8
	ldrb r0, [r5]
	orr r3, r0
	add r5, #1
	strh r3, [r7]
	strh r3, [r6, #8]
	mov r0, #2
	b _0807DB34
_0807D78C:
	add r5, r0, #0
	mov r0, #0
	str r0, [r6, #4]
	b _0807D7EE
_0807D794:
	ldrb r0, [r6, #0xB]
	strb r0, [r7, #3]
	ldrb r2, [r6, #0x13]
	mov r4, #1
	add r0, r4, #0
	and r0, r2
	cmp r0, #0
	beq _0807D7CC
	cmp r3, #1
	ble _0807D7C0
	ldr r0, _0807D818 @ =0x081A79E8
	add r0, r3, r0
	ldrb r3, [r0]
	ldr r1, _0807D81C @ =0x030053AC
	lsl r0, r3, #4
	add r0, r0, r1
	ldrb r1, [r0, #0xE]
	mov r0, r9
	and r0, r1
	cmp r0, #0
	beq _0807D7C0
	b _0807DB40
_0807D7C0:
	mov r0, #0xFE
	and r0, r2
	strb r0, [r6, #0x13]
	mov r3, r8
	strb r3, [r7, #5]
	strh r4, [r6, #0xC]
_0807D7CC:
	ldrh r0, [r6, #0xC]
	sub r0, #1
	strh r0, [r6, #0xC]
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807D7DA
	b _0807DB4A
_0807D7DA:
	ldrb r1, [r6, #0x13]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0807D7EE
	mov r0, #0xFD
	and r0, r1
	strb r0, [r6, #0x13]
	mov r0, #0x40
	strb r0, [r7, #5]
_0807D7EE:
	ldrb r1, [r6, #0x13]
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _0807D7FC
	mov r0, #1
	strh r0, [r6, #0xC]
_0807D7FC:
	ldrb r2, [r5]
	add r5, #1
	cmp r2, #0xEF
	ble _0807D810
	mov r0, #0xF
	and r0, r2
	lsl r2, r0, #8
	ldrb r0, [r5]
	add r2, r2, r0
	add r5, #1
_0807D810:
	strh r2, [r6, #0xC]
	ldrb r2, [r5]
	add r5, #1
	b _0807DB0C
_0807D818: .4byte gUnk_081A79E8
_0807D81C: .4byte 0x030053AC
_0807D820:
	cmp r2, #0xFD
	bne _0807D82E
_0807D824:
	mov r0, #0
	strh r0, [r7]
	strh r0, [r7, #2]
	mov r0, #0x40
	b _0807D9C4
_0807D82E:
	cmp r2, #0xFC
	bne _0807D874
	ldrb r3, [r5]
	add r5, #1
	cmp r3, #0
	bne _0807D848
	ldrb r1, [r6, #0x13]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0807D880
	mov r0, #0xFB
	b _0807D862
_0807D848:
	ldrb r1, [r6, #0x13]
	mov r2, #0x20
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _0807D86A
	ldrb r0, [r6, #0x12]
	sub r0, #1
	strb r0, [r6, #0x12]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _0807D880
	mov r0, #0xDF
_0807D862:
	and r0, r1
	strb r0, [r6, #0x13]
	add r5, #4
	b _0807D7EE
_0807D86A:
	strb r3, [r6, #0x12]
	add r0, r1, #0
	orr r0, r2
	strb r0, [r6, #0x13]
	b _0807D880
_0807D874:
	cmp r2, #0xFB
	beq _0807D880
	cmp r2, #0xF9
	ble _0807D896
	add r0, r5, #4
	str r0, [r6, #4]
_0807D880:
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	add r1, r1, r0
	ldrb r0, [r5, #2]
	lsl r0, r0, #0x10
	add r1, r1, r0
	ldrb r0, [r5, #3]
	lsl r0, r0, #0x18
	add r5, r1, r0
	b _0807D7EE
_0807D896:
	cmp r2, #0xF9
	beq _0807D89C
	b _0807DB36
_0807D89C:
	ldrb r1, [r6, #0x13]
	mov r0, #1
	orr r0, r1
	strb r0, [r6, #0x13]
	b _0807DB3E
_0807D8A6:
	cmp r2, #0x9F
	ble _0807D9A0
	add r4, r2, #0
	cmp r2, #0xBF
	ble _0807D900
	cmp r2, #0xDF
	ble _0807D8C0
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	add r2, r1, r0
	asr r0, r2, #4
	b _0807D916
_0807D8C0:
	cmp r2, #0xCF
	ble _0807D8DC
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	orr r1, r0
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	ldrh r0, [r6, #8]
	add r1, r1, r0
	strh r1, [r7]
	ldrb r2, [r5, #2]
	add r5, #1
	b _0807D8F2
_0807D8DC:
	ldrb r2, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	orr r2, r0
	ldr r1, _0807D8FC @ =0xFFFFFC00
	add r0, r1, #0
	ldrh r3, [r6, #8]
	add r0, r0, r3
	asr r1, r2, #5
	add r0, r0, r1
	strh r0, [r7]
_0807D8F2:
	mov r0, #0x1F
	and r2, r0
	ldrb r1, [r6, #0xB]
	add r0, r2, r1
	b _0807D94A
_0807D8FC: .4byte 0xFFFFFC00
_0807D900:
	cmp r2, #0xAF
	ble _0807D91E
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	add r2, r1, r0
	asr r0, r2, #4
	mov r3, #0x80
	lsl r3, r3, #7
	add r1, r3, #0
	orr r0, r1
_0807D916:
	strh r0, [r7]
	mov r0, #0xF
	and r2, r0
	b _0807D95A
_0807D91E:
	ldrb r3, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	orr r3, r0
	asr r0, r3, #5
	ldr r1, _0807D998 @ =0xFFFFFC00
	add r2, r0, r1
	ldrh r1, [r6, #8]
	ldr r0, _0807D99C @ =0xFFFFBFFF
	and r0, r1
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add r2, r2, r0
	mov r1, #0x80
	lsl r1, r1, #7
	add r0, r1, #0
	orr r2, r0
	strh r2, [r7]
	mov r0, #0x1F
	and r0, r3
	ldrb r2, [r6, #0xB]
	add r0, r0, r2
_0807D94A:
	add r2, r0, #0
	sub r2, #0x10
	cmp r2, #0
	bge _0807D954
	mov r2, #0
_0807D954:
	cmp r2, #0xF
	ble _0807D95A
	mov r2, #0xF
_0807D95A:
	mov r3, #3
	and r3, r4
	lsl r0, r2, #8
	orr r3, r0
	mov r0, #8
	and r0, r4
	cmp r0, #0
	bne _0807D97C
	ldrh r0, [r6, #0xA]
	cmp r0, r3
	beq _0807D976
	strh r3, [r6, #0xA]
	mov r0, #2
	strb r0, [r7, #4]
_0807D976:
	mov r0, #2
	strb r0, [r7, #5]
	strh r3, [r7, #2]
_0807D97C:
	mov r0, #4
	and r4, r0
	cmp r4, #0
	bne _0807D994
	ldrh r0, [r7]
	strh r0, [r6, #8]
	ldrh r0, [r6, #0xA]
	cmp r0, r3
	beq _0807D992
	mov r0, #8
	orr r3, r0
_0807D992:
	strh r3, [r6, #0xA]
_0807D994:
	add r5, #2
	b _0807DB36
_0807D998: .4byte 0xFFFFFC00
_0807D99C: .4byte 0xFFFFBFFF
_0807D9A0:
	cmp r2, #0x7F
	ble _0807D9D0
	mov r1, #0xF
	add r0, r2, #0
	and r0, r1
	strb r0, [r7, #3]
	strb r0, [r6, #0xB]
	ldrb r3, [r5]
	strb r3, [r6, #0xA]
	add r5, #1
	cmp r2, #0x8F
	bgt _0807D9BA
	b _0807DB36
_0807D9BA:
	ldr r4, _0807D9CC @ =0xFFFF8000
	add r0, r4, #0
	orr r3, r0
	strh r3, [r7]
	mov r0, #2
_0807D9C4:
	strb r0, [r7, #5]
	strb r0, [r7, #4]
	b _0807DB36
	.align 2, 0
_0807D9CC: .4byte 0xFFFF8000
_0807D9D0:
	cmp r2, #0x6F
	ble _0807D9D6
	b _0807D75E
_0807D9D6:
	cmp r2, #0x5F
	ble _0807DA6C
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	add r3, r1, r0
	ldr r1, _0807DA68 @ =0xFFFF8000
	add r0, r1, #0
	mov r1, #0
	orr r3, r0
	strh r3, [r7, #6]
	ldrb r0, [r5, #2]
	strb r0, [r7, #3]
	ldrb r0, [r5, #2]
	strb r0, [r6, #0xB]
	add r3, r0, #0
	add r5, #3
	strh r1, [r6, #8]
	mov r0, #4
	and r0, r2
	cmp r0, #0
	beq _0807DA0E
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r6, #8]
	add r5, #2
_0807DA0E:
	ldrh r0, [r6, #8]
	strh r0, [r7]
	mov r0, #0x80
	strb r0, [r7, #5]
	mov r0, #2
	strb r0, [r7, #4]
	asr r3, r3, #4
	strb r3, [r6, #0x15]
	cmp r3, #0
	beq _0807DA4E
	mov r4, #0x88
	mov r8, r4
	mov r1, #0
	mov ip, r1
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #3
	add r1, r0, r6
_0807DA32:
	ldrb r0, [r1, #0x13]
	mov r4, r8
	orr r0, r4
	strb r0, [r1, #0x13]
	ldrb r0, [r1, #0x15]
	mov r4, #0xFF
	orr r0, r4
	strb r0, [r1, #0x15]
	mov r0, ip
	str r0, [r1]
	sub r1, #0x18
	sub r3, #1
	cmp r3, #0
	bne _0807DA32
_0807DA4E:
	mov r0, #8
	and r2, r0
	cmp r2, #0
	beq _0807DB36
	ldrh r0, [r6, #0xC]
	cmp r0, #0
	bne _0807DA5E
	b _0807D89C
_0807DA5E:
	ldrb r1, [r6, #0x13]
	mov r0, #2
	orr r0, r1
	strb r0, [r6, #0x13]
	b _0807DB36
_0807DA68: .4byte 0xFFFF8000
_0807DA6C:
	cmp r2, #0x4F
	ble _0807DA7E
	mov r0, #0xF
	and r2, r0
_0807DA74:
	strb r2, [r6, #0xB]
_0807DA76:
	strb r2, [r7, #3]
	mov r0, #2
	strb r0, [r7, #4]
	b _0807DB36
_0807DA7E:
	cmp r2, #0x3F
	ble _0807DAA4
	add r3, r2, #0
	mov r0, #0
	ldsb r0, [r5, r0]
	ldrb r1, [r6, #0xB]
	add r2, r0, r1
	cmp r2, #0
	bge _0807DA92
	mov r2, #0
_0807DA92:
	cmp r2, #0x3F
	ble _0807DA98
	mov r2, #0x3F
_0807DA98:
	add r5, #1
	mov r0, #1
	and r0, r3
	cmp r0, #0
	bne _0807DA76
	b _0807DA74
_0807DAA4:
	cmp r2, #0x2F
	ble _0807DAAA
	b _0807D778
_0807DAAA:
	cmp r2, #0x1F
	ble _0807DADC
	mov r0, #7
	and r0, r2
	lsl r0, r0, #8
	ldrb r1, [r5]
	orr r0, r1
	ldr r4, _0807DAD8 @ =0xFFFFFC00
	add r3, r0, r4
	mov r0, #8
	and r2, r0
	cmp r2, #0
	beq _0807DACC
	ldrh r0, [r6, #8]
	add r0, r0, r3
	strh r0, [r6, #8]
	mov r3, #0
_0807DACC:
	ldrh r0, [r6, #8]
	add r0, r0, r3
	strh r0, [r7]
	mov r0, #2
	b _0807DB34
	.align 2, 0
_0807DAD8: .4byte 0xFFFFFC00
_0807DADC:
	cmp r2, #0xF
	ble _0807DB0C
	mov r3, #7
	and r3, r2
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #3
	add r0, sl
	ldr r1, _0807DB28 @ =0x0000010B
	add r3, r0, r1
	ldrb r0, [r3]
	mov r4, #0xEF
	and r4, r0
	mov r1, #0
	strb r4, [r3]
	mov r0, #8
	and r0, r2
	cmp r0, #0
	beq _0807DB0A
	mov r1, #0x10
	add r0, r4, #0
	orr r0, r1
	strb r0, [r3]
_0807DB0A:
	str r5, [r6]
_0807DB0C:
	cmp r2, #0xEF
	bgt _0807DB12
	b _0807D8A6
_0807DB12:
	cmp r2, #0xFF
	bne _0807DB2C
	ldr r0, [r6, #4]
	cmp r0, #0
	beq _0807DB1E
	b _0807D78C
_0807DB1E:
	ldrb r1, [r6, #0x13]
	mov r0, #8
	orr r0, r1
	strb r0, [r6, #0x13]
	b _0807D824
_0807DB28: .4byte 0x0000010B
_0807DB2C:
	cmp r2, #0xFE
	beq _0807DB32
	b _0807D820
_0807DB32:
	mov r0, #0x40
_0807DB34:
	strb r0, [r7, #5]
_0807DB36:
	ldrh r0, [r6, #0xC]
	cmp r0, #0
	bne _0807DB3E
	b _0807D7EE
_0807DB3E:
	str r5, [r6]
_0807DB40:
	ldrb r1, [r7, #3]
	ldrb r0, [r6, #0x14]
	mul r0, r1
	asr r0, r0, #4
	strb r0, [r7, #3]
_0807DB4A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807D6B4

