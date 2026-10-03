	thumb_func_start HasActivatableStandbyCard
HasActivatableStandbyCard: @ 0x0804F6EC
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _0804F734 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
	ldr r7, _0804F738 @ =0x0201930C
_0804F6FE:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0804F738 @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0804F796
	ldr r3, _0804F73C @ =0x000007FF
	add r0, r3, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0804F740 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804F744 @ =0x00000243
	cmp r1, r0
	beq _0804F75C
	cmp r1, r0
	bgt _0804F748
	sub r0, #0xA3
	cmp r1, r0
	beq _0804F75C
	b _0804F796
	.align 2, 0
_0804F734: .4byte 0x00000D64
_0804F738: .4byte 0x0201930C
_0804F73C: .4byte 0x000007FF
_0804F740: .4byte gCardIdToNumber
_0804F744: .4byte 0x00000243
_0804F748:
	ldr r0, _0804F758 @ =0x000002DB
	cmp r1, r0
	beq _0804F75C
	mov r0, #0x85
	lsl r0, r0, #3
	cmp r1, r0
	beq _0804F776
	b _0804F796
_0804F758: .4byte 0x000002DB
_0804F75C:
	cmp r4, #4
	bgt _0804F796
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r6
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804F796
	b _0804F784
_0804F776:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0804F796
	cmp r4, #4
	ble _0804F796
_0804F784:
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #2
	bl CanActivateEffectInZone
	cmp r0, #0
	beq _0804F796
	mov r0, #1
	b _0804F79E
_0804F796:
	add r4, #1
	cmp r4, #9
	ble _0804F6FE
	mov r0, #0
_0804F79E:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end HasActivatableStandbyCard

