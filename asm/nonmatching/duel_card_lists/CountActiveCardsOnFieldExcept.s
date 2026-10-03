	thumb_func_start CountActiveCardsOnFieldExcept
CountActiveCardsOnFieldExcept: @ 0x0800849C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r2
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r4, #0
	mov r2, #0
	ldr r1, _08008514 @ =0x0201930C
	mov r8, r1
	mov r1, #1
	and r1, r0
	ldr r0, _08008518 @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
	ldr r3, _0800851C @ =0x000007FF
	mov ip, r3
_080084C0:
	mov r0, #0x94
	mul r0, r2
	add r0, r0, r5
	mov r7, r8
	add r1, r0, r7
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _08008500
	cmp r2, r9
	beq _08008500
	mov r0, #2
	ldrb r7, [r1, #6]
	and r0, r7
	cmp r0, #0
	beq _08008500
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08008500
	mov r0, ip
	and r3, r0
	lsl r0, r3, #1
	ldr r1, _08008520 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _08008500
	add r4, #1
_08008500:
	add r2, #1
	cmp r2, #0xA
	ble _080084C0
	add r0, r4, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08008514: .4byte 0x0201930C
_08008518: .4byte 0x00000D64
_0800851C: .4byte 0x000007FF
_08008520: .4byte gCardIdToNumber
	thumb_func_end CountActiveCardsOnFieldExcept

