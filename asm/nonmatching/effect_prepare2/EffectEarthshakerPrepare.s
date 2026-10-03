	thumb_func_start EffectEarthshakerPrepare
EffectEarthshakerPrepare: @ 0x0802EB58
	push {r4, r5, r6, r7, lr}
	mov r4, #0
	ldr r7, _0802EBA0 @ =0x0201930C
	ldrb r0, [r0, #2]
	lsl r5, r0, #0x1F
	mov r3, #1
	ldr r6, _0802EBA4 @ =0x00000D64
_0802EB66:
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
	beq _0802EBA8
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
	beq _0802EBA8
	mov r0, #1
	b _0802EBB0
	.align 2, 0
_0802EBA0: .4byte 0x0201930C
_0802EBA4: .4byte 0x00000D64
_0802EBA8:
	add r4, #1
	cmp r4, #4
	ble _0802EB66
	mov r0, #0
_0802EBB0:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectEarthshakerPrepare
	.align 2, 0

