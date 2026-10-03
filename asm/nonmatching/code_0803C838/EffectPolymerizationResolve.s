	thumb_func_start EffectPolymerizationResolve
EffectPolymerizationResolve: @ 0x0803D57C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	add r2, r1, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _0803D598
	bl _0803DD6C @ far jump
_0803D598:
	ldr r1, _0803D5B8 @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r1, r3
	ldrb r0, [r0]
	sub r0, #0x62
	mov sl, r1
	cmp r0, #0x1E
	bls _0803D5AE
	bl _0803DD6C @ far jump
_0803D5AE:
	lsl r0, r0, #2
	ldr r1, _0803D5BC @ =0x0803D5C0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0803D5B8: .4byte 0x02017A40
_0803D5BC: .4byte 0x0803D5C0
_0803D5C0:
	.4byte _0803DD44
	.4byte _0803DCEC
	.4byte _0803DC6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DD6C
	.4byte _0803DA90
	.4byte _0803D6C4
	.4byte _0803D698
	.4byte _0803D63C
_0803D63C:
	add r0, r6, #0
	add r1, r2, #0
	mov r2, #0
	bl EffectPolymerizationPrepare
	cmp r0, #0
	bne _0803D64C
	b _0803DD6C
_0803D64C:
	mov r0, #1
	ldrb r1, [r6, #2]
	and r0, r1
	cmp r0, #0
	beq _0803D67C
	ldrh r0, [r6]
	bl AiPickCardListEntry
	add r2, r0, #0
	cmp r2, #0
	bge _0803D664
	b _0803DD6C
_0803D664:
	ldr r1, _0803D678 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r3, [r1, #5]
	and r0, r3
	strb r0, [r1, #5]
	strh r2, [r1, #6]
	mov r0, #0x7E
	b _0803DD6E
	.align 2, 0
_0803D678: .4byte 0x0201D810
_0803D67C:
	ldr r0, _0803D68C @ =0x00000206
	ldr r1, _0803D690 @ =0x00000712
	ldr r3, _0803D694 @ =0x08083B78
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7F
	b _0803DD6E
_0803D68C: .4byte 0x00000206
_0803D690: .4byte 0x00000712
_0803D694: .4byte gStrSelectFusionMonsterToSummon
_0803D698:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803D6BC @ =0x000007FF
	ldrh r6, [r6]
	and r2, r6
	lsl r2, r2, #1
	ldr r3, _0803D6C0 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7E
	b _0803DD6E
	.align 2, 0
_0803D6BC: .4byte 0x000007FF
_0803D6C0: .4byte gCardIdToNumber
_0803D6C4:
	ldr r1, _0803D744 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r1, #6]
	add r0, r3, r0
	lsl r0, r0, #2
	add r0, r0, r1
	ldrh r0, [r0, #0xC]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r5, #0xA0
	lsl r5, r5, #3
	add r5, sl
	strh r0, [r5]
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r5]
	ldr r2, _0803D748 @ =0x0000050A
	add r2, sl
	bl FindFusionMaterials
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803D6FA
	b _0803DD6C
_0803D6FA:
	ldr r0, _0803D74C @ =0x000007FF
	ldrh r5, [r5]
	and r0, r5
	lsl r0, r0, #1
	ldr r2, _0803D750 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #0xEB
	lsl r0, r0, #1
	cmp r1, r0
	bne _0803D712
	b _0803D912
_0803D712:
	cmp r1, r0
	ble _0803D718
	b _0803D81E
_0803D718:
	cmp r1, #0xDE
	bne _0803D71E
	b _0803D912
_0803D71E:
	cmp r1, #0xDE
	bgt _0803D7A0
	cmp r1, #0x5B
	bne _0803D728
	b _0803D912
_0803D728:
	cmp r1, #0x5B
	bgt _0803D76E
	cmp r1, #0x24
	bne _0803D732
	b _0803D912
_0803D732:
	cmp r1, #0x24
	bgt _0803D754
	cmp r1, #0xE
	bne _0803D73C
	b _0803D912
_0803D73C:
	cmp r1, #0x1D
	bne _0803D742
	b _0803D912
_0803D742:
	b _0803D942
_0803D744: .4byte 0x0201D810
_0803D748: .4byte 0x0000050A
_0803D74C: .4byte 0x000007FF
_0803D750: .4byte gCardIdToNumber
_0803D754:
	cmp r1, #0x44
	bne _0803D75A
	b _0803D912
_0803D75A:
	cmp r1, #0x44
	bgt _0803D766
	cmp r1, #0x2A
	bne _0803D764
	b _0803D912
_0803D764:
	b _0803D942
_0803D766:
	cmp r1, #0x55
	bne _0803D76C
	b _0803D912
_0803D76C:
	b _0803D942
_0803D76E:
	cmp r1, #0xA7
	bne _0803D774
	b _0803D912
_0803D774:
	cmp r1, #0xA7
	bgt _0803D786
	cmp r1, #0x72
	bne _0803D77E
	b _0803D912
_0803D77E:
	cmp r1, #0x84
	bne _0803D784
	b _0803D912
_0803D784:
	b _0803D942
_0803D786:
	cmp r1, #0xD6
	bne _0803D78C
	b _0803D912
_0803D78C:
	cmp r1, #0xD6
	bgt _0803D798
	cmp r1, #0xBC
	bne _0803D796
	b _0803D912
_0803D796:
	b _0803D942
_0803D798:
	cmp r1, #0xD8
	bne _0803D79E
	b _0803D912
_0803D79E:
	b _0803D942
_0803D7A0:
	mov r0, #0xC2
	lsl r0, r0, #1
	cmp r1, r0
	bne _0803D7AA
	b _0803D912
_0803D7AA:
	cmp r1, r0
	bgt _0803D7DE
	sub r0, #0x5F
	cmp r1, r0
	bne _0803D7B6
	b _0803D912
_0803D7B6:
	cmp r1, r0
	bgt _0803D7C4
	cmp r1, #0xE5
	bne _0803D7C0
	b _0803D912
_0803D7C0:
	sub r0, #0x16
	b _0803D8FC
_0803D7C4:
	ldr r0, _0803D7D4 @ =0x0000017B
	cmp r1, r0
	bne _0803D7CC
	b _0803D934
_0803D7CC:
	cmp r1, r0
	bgt _0803D7D8
	sub r0, #0xA
	b _0803D8FC
_0803D7D4: .4byte 0x0000017B
_0803D7D8:
	mov r0, #0xC0
	lsl r0, r0, #1
	b _0803D8FC
_0803D7DE:
	ldr r0, _0803D7FC @ =0x000001B9
	cmp r1, r0
	bne _0803D7E6
	b _0803D934
_0803D7E6:
	cmp r1, r0
	bgt _0803D806
	sub r0, #0xF
	cmp r1, r0
	bne _0803D7F2
	b _0803D912
_0803D7F2:
	cmp r1, r0
	bgt _0803D800
	sub r0, #0x12
	b _0803D8FC
	.align 2, 0
_0803D7FC: .4byte 0x000001B9
_0803D800:
	mov r0, #0xDA
	lsl r0, r0, #1
	b _0803D8FC
_0803D806:
	mov r0, #0xE8
	lsl r0, r0, #1
	cmp r1, r0
	bne _0803D810
	b _0803D912
_0803D810:
	cmp r1, r0
	bgt _0803D818
	sub r0, #7
	b _0803D8FC
_0803D818:
	mov r0, #0xE9
	lsl r0, r0, #1
	b _0803D8FC
_0803D81E:
	ldr r0, _0803D848 @ =0x00000251
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D894
	sub r0, #0x49
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D866
	sub r0, #0x22
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D84C
	sub r0, #0xE
	cmp r1, r0
	beq _0803D912
	add r0, #9
	b _0803D8FC
	.align 2, 0
_0803D848: .4byte 0x00000251
_0803D84C:
	mov r0, #0xF7
	lsl r0, r0, #1
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	blt _0803D942
	add r0, #0xE
	cmp r1, r0
	bgt _0803D942
	sub r0, #1
	cmp r1, r0
	blt _0803D942
	b _0803D912
_0803D866:
	mov r0, #0x88
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D87C
	sub r0, #0xE
	cmp r1, r0
	beq _0803D912
	add r0, #2
	b _0803D8FC
_0803D87C:
	mov r0, #0x8D
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803D934
	cmp r1, r0
	bgt _0803D88C
	sub r0, #1
	b _0803D8FC
_0803D88C:
	ldr r0, _0803D890 @ =0x0000023B
	b _0803D8FC
_0803D890: .4byte 0x0000023B
_0803D894:
	mov r0, #0xCB
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D8CA
	sub r0, #0xC3
	cmp r1, r0
	bgt _0803D8B0
	sub r0, #1
	cmp r1, r0
	bge _0803D912
	sub r0, #4
	b _0803D8FC
_0803D8B0:
	ldr r0, _0803D8C0 @ =0x000002C2
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D8C4
	sub r0, #0x44
	b _0803D8FC
	.align 2, 0
_0803D8C0: .4byte 0x000002C2
_0803D8C4:
	mov r0, #0xB2
	lsl r0, r0, #2
	b _0803D8FC
_0803D8CA:
	ldr r0, _0803D8E4 @ =0x000005A5
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D8F0
	sub r0, #0x6F
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D8E8
	sub r0, #0x5D
	b _0803D8FC
	.align 2, 0
_0803D8E4: .4byte 0x000005A5
_0803D8E8:
	ldr r0, _0803D8EC @ =0x0000057C
	b _0803D8FC
_0803D8EC: .4byte 0x0000057C
_0803D8F0:
	ldr r0, _0803D904 @ =0x000007DE
	cmp r1, r0
	beq _0803D912
	cmp r1, r0
	bgt _0803D90C
	ldr r0, _0803D908 @ =0x000005F6
_0803D8FC:
	cmp r1, r0
	beq _0803D912
	b _0803D942
	.align 2, 0
_0803D904: .4byte 0x000007DE
_0803D908: .4byte 0x000005F6
_0803D90C:
	ldr r0, _0803D928 @ =0x00000814
	cmp r1, r0
	bne _0803D942
_0803D912:
	ldr r0, _0803D92C @ =0x02017A40
	ldr r3, _0803D930 @ =0x00000502
	add r0, r0, r3
	mov r1, #4
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	b _0803D942
_0803D928: .4byte 0x00000814
_0803D92C: .4byte 0x02017A40
_0803D930: .4byte 0x00000502
_0803D934:
	ldr r1, _0803D950 @ =0x02017A40
	ldr r3, _0803D954 @ =0x00000502
	add r1, r1, r3
	mov r0, #3
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0803D942:
	mov r0, #1
	ldrb r3, [r6, #2]
	and r0, r3
	cmp r0, #0
	beq _0803D958
_0803D94C:
	mov r0, #0x64
	b _0803DD6E
_0803D950: .4byte 0x02017A40
_0803D954: .4byte 0x00000502
_0803D958:
	ldr r0, _0803D970 @ =0x02017A40
	ldr r1, _0803D974 @ =0x00000502
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #2
	beq _0803D978
	cmp r0, #3
	beq _0803D994
	b _0803D9A0
	.align 2, 0
_0803D970: .4byte 0x02017A40
_0803D974: .4byte 0x00000502
_0803D978:
	ldr r0, _0803D988 @ =0x00000206
	ldr r1, _0803D98C @ =0x00000712
	ldr r3, _0803D990 @ =0x08083BC8
	mov r2, #0xB
	bl TextBoxOpen
	b _0803D9A0
	.align 2, 0
_0803D988: .4byte 0x00000206
_0803D98C: .4byte 0x00000712
_0803D990: .4byte gStrSelectTwoFusionMaterials
_0803D994:
	ldr r0, _0803DA68 @ =0x00000206
	ldr r1, _0803DA6C @ =0x00000712
	ldr r3, _0803DA70 @ =0x08083C0C
	mov r2, #0xB
	bl TextBoxOpen
_0803D9A0:
	ldr r4, _0803DA74 @ =0x02017A40
	ldr r2, _0803DA78 @ =0x00000502
	add r3, r4, r2
	mov r1, #0xF
	ldrb r0, [r3]
	and r1, r0
	lsl r2, r1, #0x1E
	lsr r2, r2, #0x1C
	mov r0, #0xD
	neg r0, r0
	and r0, r1
	orr r0, r2
	strb r0, [r3]
	lsl r0, r0, #0x1E
	mov r5, #0
	mov sl, r4
	cmp r0, #0
	beq _0803DA46
	mov r8, sl
	ldr r1, _0803DA7C @ =0x02019968
	mov r9, r1
	ldr r7, _0803DA80 @ =0x0000050A
	add r7, sl
	mov r2, #1
	mov ip, r2
_0803D9D2:
	mov r1, #0
	lsl r4, r5, #1
	ldrh r3, [r7]
	mov r0, #0x80
	lsl r0, r0, #8
	and r0, r3
	cmp r0, #0
	beq _0803DA00
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r2, ip
	and r2, r0
	mov r0, #0xFF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0803DA84 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
_0803DA00:
	mov r0, #0x80
	lsl r0, r0, #7
	and r0, r3
	cmp r0, #0
	beq _0803DA2C
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r2, ip
	and r2, r0
	mov r0, #0xFF
	and r0, r3
	mov r1, #0x94
	mul r1, r0
	ldr r0, _0803DA84 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r3, _0803DA88 @ =0x0201930C
	add r1, r1, r3
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
_0803DA2C:
	ldr r0, _0803DA8C @ =0x00000504
	add r0, r8
	add r0, r4, r0
	strh r1, [r0]
	add r7, #2
	add r5, #1
	ldr r0, _0803DA78 @ =0x00000502
	add r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	cmp r5, r0
	blt _0803D9D2
_0803DA46:
	ldr r3, _0803DA78 @ =0x00000502
	add r3, sl
	ldrb r2, [r3]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1E
	sub r1, #1
	mov r0, #3
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0xD
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_0803DA62:
	mov r0, #0x7D
	b _0803DD6E
	.align 2, 0
_0803DA68: .4byte 0x00000206
_0803DA6C: .4byte 0x00000712
_0803DA70: .4byte gStrSelectThreeFusionMaterials
_0803DA74: .4byte 0x02017A40
_0803DA78: .4byte 0x00000502
_0803DA7C: .4byte 0x02019968
_0803DA80: .4byte 0x0000050A
_0803DA84: .4byte 0x00000D64
_0803DA88: .4byte 0x0201930C
_0803DA8C: .4byte 0x00000504
_0803DA90:
	mov r0, #0xF1
	bl DuelCursor_PickTarget
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r1, _0803DAC4 @ =0x0201CFB0
	ldr r2, _0803DAC8 @ =0x00000828
	add r0, r1, r2
	ldr r0, [r0]
	cmp r0, #0
	beq _0803DAD8
	cmp r0, #0xB
	bne _0803DB08
	ldrb r3, [r6, #2]
	lsl r2, r3, #0x1F
	lsr r2, r2, #0x1F
	ldr r3, _0803DACC @ =0x0000082C
	add r0, r1, r3
	ldr r0, [r0]
	lsl r0, r0, #2
	ldr r1, _0803DAD0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803DAD4 @ =0x02019968
	b _0803DAF0
	.align 2, 0
_0803DAC4: .4byte 0x0201CFB0
_0803DAC8: .4byte 0x00000828
_0803DACC: .4byte 0x0000082C
_0803DAD0: .4byte 0x00000D64
_0803DAD4: .4byte 0x02019968
_0803DAD8:
	ldrb r0, [r6, #2]
	lsl r2, r0, #0x1F
	lsr r2, r2, #0x1F
	ldr r3, _0803DAFC @ =0x0000082C
	add r0, r1, r3
	ldr r1, [r0]
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0803DB00 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803DB04 @ =0x0201930C
_0803DAF0:
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	b _0803DB0A
	.align 2, 0
_0803DAFC: .4byte 0x0000082C
_0803DB00: .4byte 0x00000D64
_0803DB04: .4byte 0x0201930C
_0803DB08:
	mov r4, #0
_0803DB0A:
	mov r5, #0
	cmp r4, #0
	beq _0803DB48
	add r0, r4, #0
	bl IsPendingFusionMaterial
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	neg r1, r0
	orr r1, r0
	lsr r5, r1, #0x1F
	ldr r0, _0803DBB4 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0803DBB8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803DB48
	ldr r0, _0803DBBC @ =0x02017A40
	ldr r2, _0803DBC0 @ =0x00000502
	add r0, r0, r2
	ldrb r0, [r0]
	lsr r0, r0, #4
	mov r5, #0
	cmp r0, #0
	bne _0803DB48
	mov r5, #1
_0803DB48:
	cmp r5, #0
	beq _0803DA62
	cmp r7, #0
	beq _0803DA62
	add r0, r4, #0
	bl RemovePendingFusionMaterial
	ldr r5, _0803DBB4 @ =0x000007FF
	and r4, r5
	lsl r0, r4, #1
	ldr r4, _0803DBB8 @ =0x08622AB4
	add r0, r0, r4
	ldrh r0, [r0]
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803DB82
	ldr r2, _0803DBBC @ =0x02017A40
	ldr r3, _0803DBC0 @ =0x00000502
	add r2, r2, r3
	ldrb r3, [r2]
	lsr r1, r3, #4
	add r1, #1
	lsl r1, r1, #4
	mov r0, #0xF
	and r0, r3
	orr r0, r1
	strb r0, [r2]
_0803DB82:
	ldr r3, _0803DBC4 @ =0x0201CFB0
	ldr r1, _0803DBC8 @ =0x00000828
	add r0, r3, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0803DBFC
	cmp r0, #0xB
	bne _0803DC3A
	add r0, r5, #0
	ldrh r2, [r6]
	and r0, r2
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _0803DBCC @ =0x0000060B
	ldrh r0, [r0]
	cmp r0, r1
	beq _0803DBD4
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r2, #0xCC
	cmp r0, #0
	beq _0803DBE2
	ldr r2, _0803DBD0 @ =0x000080CC
	b _0803DBE2
_0803DBB4: .4byte 0x000007FF
_0803DBB8: .4byte gCardIdToNumber
_0803DBBC: .4byte 0x02017A40
_0803DBC0: .4byte 0x00000502
_0803DBC4: .4byte 0x0201CFB0
_0803DBC8: .4byte 0x00000828
_0803DBCC: .4byte 0x0000060B
_0803DBD0: .4byte 0x000080CC
_0803DBD4:
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r2, #0xCD
	cmp r0, #0
	beq _0803DBE2
	ldr r2, _0803DBF4 @ =0x000080CD
_0803DBE2:
	ldr r1, _0803DBF8 @ =0x0000082C
	add r0, r3, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _0803DC3A
_0803DBF4: .4byte 0x000080CD
_0803DBF8: .4byte 0x0000082C
_0803DBFC:
	add r0, r5, #0
	ldrh r2, [r6]
	and r0, r2
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _0803DC20 @ =0x0000060B
	ldrh r0, [r0]
	cmp r0, r1
	beq _0803DC28
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0803DC24 @ =0x0000082C
	add r1, r3, r2
	ldr r1, [r1]
	bl SendFusionMaterialToGrave
	b _0803DC3A
_0803DC20: .4byte 0x0000060B
_0803DC24: .4byte 0x0000082C
_0803DC28:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0803DC60 @ =0x0000082C
	add r1, r3, r2
	ldr r1, [r1]
	mov r2, #1
	bl BanishFieldCard
_0803DC3A:
	ldr r0, _0803DC64 @ =0x02017A40
	ldr r1, _0803DC68 @ =0x00000502
	add r3, r0, r1
	ldrb r2, [r3]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _0803DCE8
	sub r0, #1
	mov r1, #3
	and r0, r1
	lsl r0, r0, #2
	mov r1, #0xD
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3]
	b _0803DA62
	.align 2, 0
_0803DC60: .4byte 0x0000082C
_0803DC64: .4byte 0x02017A40
_0803DC68: .4byte 0x00000502
_0803DC6C:
	ldr r5, _0803DCC0 @ =0x00000502
	add r5, sl
	ldrb r2, [r5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _0803DCE8
	sub r1, r0, #1
	mov r0, #3
	and r1, r0
	mov r0, #4
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1D
	ldr r1, _0803DCC4 @ =0x0000050A
	add r1, sl
	add r0, r0, r1
	ldrh r2, [r0]
	mov r0, #0x80
	lsl r0, r0, #8
	and r0, r2
	cmp r0, #0
	beq _0803DCCC
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r3, #0xCC
	cmp r0, #0
	beq _0803DCAE
	ldr r3, _0803DCC8 @ =0x000080CC
_0803DCAE:
	mov r1, #0xFF
	and r1, r2
	add r0, r3, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _0803D94C
	.align 2, 0
_0803DCC0: .4byte 0x00000502
_0803DCC4: .4byte 0x0000050A
_0803DCC8: .4byte 0x000080CC
_0803DCCC:
	mov r0, #0x80
	lsl r0, r0, #7
	and r0, r2
	cmp r0, #0
	bne _0803DCD8
	b _0803D94C
_0803DCD8:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xFF
	and r1, r2
	bl SendFusionMaterialToGrave
	b _0803D94C
_0803DCE8:
	mov r0, #0x63
	b _0803DD6E
_0803DCEC:
	mov r4, #1
	add r0, r4, #0
	ldrb r2, [r6, #2]
	and r0, r2
	mov r1, #0xCA
	cmp r0, #0
	beq _0803DCFC
	ldr r1, _0803DD38 @ =0x000080CA
_0803DCFC:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _0803DD3C @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r0, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	add r0, r4, #0
	ldrb r6, [r6, #2]
	and r0, r6
	mov r3, #0xDC
	cmp r0, #0
	beq _0803DD28
	ldr r3, _0803DD40 @ =0x000080DC
_0803DD28:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x62
	b _0803DD6E
_0803DD38: .4byte 0x000080CA
_0803DD3C: .4byte 0x0201D810
_0803DD40: .4byte 0x000080DC
_0803DD44:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0803DD68 @ =0x0201D810
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #1
	mov r3, #1
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x61
	b _0803DD6E
_0803DD68: .4byte 0x0201D810
_0803DD6C:
	mov r0, #0
_0803DD6E:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectPolymerizationResolve

