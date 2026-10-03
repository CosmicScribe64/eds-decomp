	thumb_func_start CountEnabledSpellTrapCards
CountEnabledSpellTrapCards: @ 0x08008538
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov ip, r1
	mov r5, #0
	mov r3, #5
	mov r1, #1
	and r1, r0
	ldr r0, _080085A0 @ =0x00000D64
	add r4, r1, #0
	mul r4, r0
	ldr r6, _080085A4 @ =0x0201930C
	add r7, r4, r6
	ldr r0, _080085A8 @ =0x000007FF
	mov r8, r0
_0800855A:
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	add r0, r7, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0800858E
	add r1, r1, r4
	add r1, r1, r6
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800858E
	mov r1, r8
	and r2, r1
	lsl r0, r2, #1
	ldr r1, _080085AC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, ip
	bne _0800858E
	add r5, #1
_0800858E:
	add r3, #1
	cmp r3, #9
	ble _0800855A
	add r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080085A0: .4byte 0x00000D64
_080085A4: .4byte 0x0201930C
_080085A8: .4byte 0x000007FF
_080085AC: .4byte gCardIdToNumber
	thumb_func_end CountEnabledSpellTrapCards

