	thumb_func_start AddCardToGraveyard
AddCardToGraveyard: @ 0x080096F4
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r2, [r4]
	lsl r0, r2, #0x13
	lsr r0, r0, #0x1F
	ldr r1, _08009750 @ =0x00000D64
	mul r1, r0
	ldr r0, _08009754 @ =0x02019BE8
	add r3, r1, r0
	ldr r5, _08009758 @ =0xFFFFF6FC
	add r0, r0, r5
	add r5, r1, r0
	ldrb r1, [r5, #4]
	lsl r0, r1, #2
	add r3, r3, r0
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	cmp r2, #0
	beq _0800974A
	ldr r0, _0800975C @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _08009760 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _08009764 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0800974A
	mov r0, #5
	neg r0, r0
	ldrb r1, [r4, #2]
	and r0, r1
	strb r0, [r4, #2]
	add r0, r3, #0
	add r1, r4, #0
	bl CopyDuelCard
	ldrb r0, [r5, #4]
	add r0, #1
	strb r0, [r5, #4]
_0800974A:
	pop {r4, r5}
	pop {r0}
	bx r0
_08009750: .4byte 0x00000D64
_08009754: .4byte 0x02019BE8
_08009758: .4byte 0xFFFFF6FC
_0800975C: .4byte 0x000007FF
_08009760: .4byte gCardIdToNumber
_08009764: .4byte 0xFFFFF880
	thumb_func_end AddCardToGraveyard

