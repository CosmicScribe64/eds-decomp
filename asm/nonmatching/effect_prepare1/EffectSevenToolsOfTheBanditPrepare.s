	thumb_func_start EffectSevenToolsOfTheBanditPrepare
EffectSevenToolsOfTheBanditPrepare: @ 0x0802E5D4
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802E628
	cmp r1, #0
	beq _0802E628
	ldr r0, _0802E614 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802E618 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0802E628
	ldr r2, _0802E61C @ =0x020192E4
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E620 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0802E624 @ =0x000003E7
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802E628
	mov r0, #1
	b _0802E62A
_0802E614: .4byte 0x000007FF
_0802E618: .4byte gCardStats
_0802E61C: .4byte 0x020192E4
_0802E620: .4byte 0x00000D64
_0802E624: .4byte 0x000003E7
_0802E628:
	mov r0, #0
_0802E62A:
	bx lr
	thumb_func_end EffectSevenToolsOfTheBanditPrepare

