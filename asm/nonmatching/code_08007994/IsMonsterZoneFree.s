	thumb_func_start IsMonsterZoneFree
IsMonsterZoneFree: @ 0x08008940
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r1, #0
	mov r3, #1
	and r0, r3
	mov r1, #0x94
	add r6, r5, #0
	mul r6, r1
	ldr r1, _08008990 @ =0x00000D64
	mov r8, r1
	mov r4, r8
	mul r4, r0
	add r0, r6, r4
	ldr r1, _08008994 @ =0x0201930C
	add r0, r0, r1
	mov ip, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	add r7, r1, #0
	cmp r0, #0
	bne _0800898C
	add r0, r7, #0
	sub r0, #0x28
	add r0, r4, r0
	ldrb r1, [r0, #0xB]
	lsr r2, r1, #4
	add r1, r3, #0
	ldrb r0, [r0, #0xC]
	and r1, r0
	lsl r1, r1, #4
	orr r1, r2
	asr r1, r5
	and r1, r3
	cmp r1, #0
	beq _08008998
_0800898C:
	mov r0, #0
	b _08008A06
_08008990: .4byte 0x00000D64
_08008994: .4byte 0x0201930C
_08008998:
	mov r3, #0
	mov r0, ip
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r3, r0
	bge _08008A04
	mov r2, #1
	mov sl, r2
	mov r1, #0x94
	mov r9, r1
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0x4A
	add r0, r6, r0
	add r2, r0, r7
	mov r4, #0xA5
	lsl r4, r4, #3
_080089BA:
	lsl r1, r3, #1
	mov r0, ip
	add r0, #0xA
	add r0, r0, r1
	ldrb r1, [r0]
	ldrh r0, [r0]
	lsr r0, r0, #8
	ldrb r6, [r2]
	cmp r6, #2
	bne _080089FC
	mov r6, sl
	and r1, r6
	mov r6, r9
	mul r6, r0
	add r0, r6, #0
	mov r6, r8
	mul r6, r1
	add r1, r6, #0
	add r0, r0, r1
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080089FC
	ldr r1, _08008A14 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r6, _08008A18 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, r4
	beq _0800898C
_080089FC:
	add r2, #2
	add r3, #1
	cmp r3, r5
	blt _080089BA
_08008A04:
	mov r0, #1
_08008A06:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08008A14: .4byte 0x000007FF
_08008A18: .4byte gCardIdToNumber
	thumb_func_end IsMonsterZoneFree

