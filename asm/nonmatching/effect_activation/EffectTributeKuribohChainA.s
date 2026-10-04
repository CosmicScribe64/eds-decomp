	thumb_func_start EffectTributeKuribohChainA
EffectTributeKuribohChainA: @ 0x0802CB0C
	push {r4, r5, lr}
	sub sp, #0x100
	ldr r0, _0802CB24 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r5, r0, r1
	ldrb r4, [r5]
	cmp r4, #0
	beq _0802CB28
	cmp r4, #1
	beq _0802CB64
	b _0802CBE2
_0802CB24: .4byte 0x02017A40
_0802CB28:
	ldr r4, _0802CB50 @ =0x080827EC
	ldr r0, _0802CB54 @ =0x08623E66
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _0802CB58 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	add r1, r4, #0
	bl FormatStr
	ldr r0, _0802CB5C @ =0x00000206
	ldr r1, _0802CB60 @ =0x00000712
	mov r2, #0xB
	add r3, r4, #0
	bl TextBoxOpen
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0802CBE2
_0802CB50: .4byte gStrSelectTributeFmt
_0802CB54: .4byte gCardNumberToId_Kuriboh
_0802CB58: .4byte gCardNames
_0802CB5C: .4byte 0x00000206
_0802CB60: .4byte 0x00000712
_0802CB64:
	mov r0, #0xE0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0802CBE2
	ldr r0, _0802CBC0 @ =0x0201CFB0
	ldr r2, _0802CBC4 @ =0x00000824
	add r1, r0, r2
	ldr r5, [r1]
	ldr r1, _0802CBC8 @ =0x0000082C
	add r0, r0, r1
	ldr r3, [r0]
	and r4, r5
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0802CBCC @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	ldr r0, _0802CBD0 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0802CBDC
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802CBDC
	ldr r0, _0802CBD4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0802CBD8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0x39
	bne _0802CBDC
	add r0, r5, #0
	add r1, r3, #0
	bl TributeMonster
	mov r0, #1
	b _0802CBE4
	.align 2, 0
_0802CBC0: .4byte 0x0201CFB0
_0802CBC4: .4byte 0x00000824
_0802CBC8: .4byte 0x0000082C
_0802CBCC: .4byte 0x00000D64
_0802CBD0: .4byte 0x0201930C
_0802CBD4: .4byte 0x000007FF
_0802CBD8: .4byte gCardIdToNumber
_0802CBDC:
	mov r0, #3
	bl PlaySE
_0802CBE2:
	mov r0, #0
_0802CBE4:
	add sp, #0x100
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeKuribohChainA

