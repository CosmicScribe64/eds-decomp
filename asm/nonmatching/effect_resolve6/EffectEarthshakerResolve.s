	thumb_func_start EffectEarthshakerResolve
EffectEarthshakerResolve: @ 0x080369C4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08036A54
	mov r0, #7
	ldrb r1, [r6, #0xA]
	and r0, r1
	cmp r0, #1
	bne _08036A54
	mov r2, #0
_080369E2:
	cmp r2, #0
	beq _080369EE
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r5, r0, #0x1F
	b _080369F8
_080369EE:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r5, r1, r0
_080369F8:
	mov r4, #0
	add r2, #1
	mov r8, r2
	mov r0, #1
	and r0, r5
	ldr r1, _08036A60 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_08036A08:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08036A64 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08036A48
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08036A48
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardAttribute
	ldrh r1, [r6, #0xC]
	cmp r0, r1
	bne _08036A48
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08036A48:
	add r4, #1
	cmp r4, #4
	ble _08036A08
	mov r2, r8
	cmp r2, #1
	ble _080369E2
_08036A54:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08036A60: .4byte 0x00000D64
_08036A64: .4byte 0x0201930C
	thumb_func_end EffectEarthshakerResolve

