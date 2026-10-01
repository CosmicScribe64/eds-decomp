	thumb_func_start sub_08009768
sub_08009768: @ 0x08009768
	push {r4, r5, r6, r7, lr}
	add r3, r0, #0
	ldr r2, [r3]
	lsl r0, r2, #0x13
	lsr r0, r0, #0x1F
	ldr r1, _080097D8 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _080097DC @ =0x02019E68
	add r1, r5, r6
	ldr r4, _080097E0 @ =0xFFFFF47C
	add r0, r6, r4
	add r4, r5, r0
	ldrb r7, [r4, #6]
	lsl r0, r7, #2
	add r1, r1, r0
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	cmp r2, #0
	beq _080097D2
	ldr r0, _080097E4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _080097E8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r7, _080097EC @ =0xFFFFF880
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _080097D2
	mov r0, #5
	neg r0, r0
	ldrb r2, [r3, #2]
	and r0, r2
	strb r0, [r3, #2]
	add r0, r1, #0
	add r1, r3, #0
	bl sub_08007558
	ldrb r7, [r4, #6]
	lsl r0, r7, #1
	add r0, r0, r5
	mov r2, #0xA0
	lsl r2, r2, #1
	add r1, r6, r2
	add r0, r0, r1
	mov r1, #0
	strh r1, [r0]
	ldrb r0, [r4, #6]
	add r0, #1
	strb r0, [r4, #6]
_080097D2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080097D8: .4byte 0x00000D64
_080097DC: .4byte 0x02019E68
_080097E0: .4byte 0xFFFFF47C
_080097E4: .4byte 0x000007FF
_080097E8: .4byte gUnk_08622AB4
_080097EC: .4byte 0xFFFFF880
	thumb_func_end sub_08009768

