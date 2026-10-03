	thumb_func_start EffectMagicCardInHandPrepare
EffectMagicCardInHandPrepare: @ 0x0802F958
	push {r4, r5, r6, r7, lr}
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _0802F966
	b _0802F9C8
_0802F962:
	mov r0, #1
	b _0802F9CA
_0802F966:
	mov r2, #0
	ldr r4, _0802F9D0 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	ldr r5, _0802F9D4 @ =0x00000D64
	mul r0, r5
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r2, r0
	bge _0802F9C8
	add r3, r1, #0
	mov r6, #1
	ldr r0, _0802F9D8 @ =0x00000684
	add r0, r0, r4
	mov ip, r0
	add r7, r4, #0
	ldr r4, _0802F9DC @ =0x000007FF
_0802F98A:
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	lsl r0, r2, #2
	mul r1, r5
	add r0, r0, r1
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802F9E0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0802F962
	add r2, #1
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r5
	add r0, r0, r7
	ldrb r0, [r0, #2]
	cmp r2, r0
	blt _0802F98A
_0802F9C8:
	mov r0, #0
_0802F9CA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802F9D0: .4byte 0x020192E4
_0802F9D4: .4byte 0x00000D64
_0802F9D8: .4byte 0x00000684
_0802F9DC: .4byte 0x000007FF
_0802F9E0: .4byte gCardStats
	thumb_func_end EffectMagicCardInHandPrepare

