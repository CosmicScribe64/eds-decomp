	thumb_func_start EffectSealOfTheAncientsPrepare
EffectSealOfTheAncientsPrepare: @ 0x0802EFF0
	push {r4, r5, r6, r7, lr}
	ldr r3, _0802F00C @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldr r6, _0802F010 @ =0x00000D64
	mul r0, r6
	add r0, r0, r3
	ldr r1, _0802F014 @ =0x000003E7
	ldrh r0, [r0]
	cmp r0, r1
	bhi _0802F01C
	b _0802F060
	.align 2, 0
_0802F00C: .4byte 0x020192E4
_0802F010: .4byte 0x00000D64
_0802F014: .4byte 0x000003E7
_0802F018:
	mov r0, #1
	b _0802F062
_0802F01C:
	mov r4, #0
	add r7, r3, #0
	add r7, #0x28
	add r5, r2, #0
	mov r3, #1
_0802F026:
	lsr r1, r5, #0x1F
	sub r1, r3, r1
	and r1, r3
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802F05A
	lsr r0, r5, #0x1F
	sub r0, r3, r0
	and r0, r3
	add r1, r0, #0
	mul r1, r6
	add r1, r2, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802F018
_0802F05A:
	add r4, #1
	cmp r4, #0xA
	ble _0802F026
_0802F060:
	mov r0, #0
_0802F062:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectSealOfTheAncientsPrepare

