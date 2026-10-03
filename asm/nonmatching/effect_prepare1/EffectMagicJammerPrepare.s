	thumb_func_start EffectMagicJammerPrepare
EffectMagicJammerPrepare: @ 0x0802E564
	push {r4, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802E5CC
	cmp r1, #0
	beq _0802E5CC
	ldr r3, _0802E5B4 @ =0x000007FF
	ldrh r1, [r1]
	and r3, r1
	lsl r0, r3, #2
	ldr r1, _0802E5B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802E5CC
	ldr r2, _0802E5BC @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E5C0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _0802E5CC
	lsl r0, r3, #1
	ldr r1, _0802E5C4 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0802E5C8 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802E5CC
	mov r0, #1
	b _0802E5CE
	.align 2, 0
_0802E5B4: .4byte 0x000007FF
_0802E5B8: .4byte gCardStats
_0802E5BC: .4byte 0x020192E4
_0802E5C0: .4byte 0x00000D64
_0802E5C4: .4byte gCardIdToNumber
_0802E5C8: .4byte 0x00000603
_0802E5CC:
	mov r0, #0
_0802E5CE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectMagicJammerPrepare

