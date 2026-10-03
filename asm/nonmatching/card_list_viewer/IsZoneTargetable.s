	thumb_func_start IsZoneTargetable
IsZoneTargetable: @ 0x0802B28C
	push {r4, lr}
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0802B2DC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802B2E0 @ =0x0201930C
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0802B2D6
	ldr r0, _0802B2E4 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0802B2E8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802B2EC @ =0x0000052E
	cmp r1, r0
	beq _0802B2C2
	add r0, #3
	cmp r1, r0
	bne _0802B2F4
_0802B2C2:
	bl GetFaceUpFieldMagicNumber
	ldr r1, _0802B2F0 @ =0x0000014D
	cmp r0, r1
	bne _0802B2F4
	mov r0, #2
	ldrb r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	beq _0802B2F4
_0802B2D6:
	mov r0, #0
	b _0802B2F6
	.align 2, 0
_0802B2DC: .4byte 0x00000D64
_0802B2E0: .4byte 0x0201930C
_0802B2E4: .4byte 0x000007FF
_0802B2E8: .4byte gCardIdToNumber
_0802B2EC: .4byte 0x0000052E
_0802B2F0: .4byte 0x0000014D
_0802B2F4:
	mov r0, #1
_0802B2F6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end IsZoneTargetable

