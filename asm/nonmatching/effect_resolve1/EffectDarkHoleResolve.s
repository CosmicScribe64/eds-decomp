	thumb_func_start EffectDarkHoleResolve
EffectDarkHoleResolve: @ 0x08030FFC
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _0803108A
	ldr r3, _08031068 @ =0x02017A40
	mov r4, #0xF8
	lsl r4, r4, #2
	add r2, r3, r4
	ldrb r0, [r2]
	cmp r0, #0x7F
	beq _08031032
	cmp r0, #0x80
	bne _0803108A
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	add r4, #1
	add r0, r3, r4
	strb r1, [r0]
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
_08031032:
	mov r4, #0
	ldr r0, _0803106C @ =0x000003E1
	add r5, r3, r0
_08031038:
	ldrb r0, [r5]
	add r1, r4, #0
	bl IsZoneTargetable
	cmp r0, #0
	bne _08031070
	add r4, #1
	cmp r4, #4
	ble _08031038
	ldr r0, _08031068 @ =0x02017A40
	ldr r1, _0803106C @ =0x000003E1
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	sub r1, r1, r2
	strb r1, [r0]
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r1, r1, #0x1F
	ldrb r0, [r0]
	cmp r0, r1
	bne _0803108A
	mov r0, #0x7F
	b _0803108C
_08031068: .4byte 0x02017A40
_0803106C: .4byte 0x000003E1
_08031070:
	ldrb r0, [r5]
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldrb r1, [r5]
	add r2, r4, #0
	bl OnCardDestroyedByEffect
	mov r0, #0x7F
	b _0803108C
_0803108A:
	mov r0, #0
_0803108C:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectDarkHoleResolve
	.align 2, 0

