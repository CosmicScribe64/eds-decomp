	thumb_func_start EffectDestroyOpponentLevel4Resolve
EffectDestroyOpponentLevel4Resolve: @ 0x0803AA80
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	mov r0, #4
	mov r1, r8
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0803AB5E
	mov r7, #0
	mov r6, #1
	ldr r2, _0803AB08 @ =0x00000D64
	mov sl, r2
	ldr r0, _0803AB0C @ =0x0201930C
	mov r9, r0
_0803AAA4:
	mov r1, r8
	ldrb r4, [r1, #2]
	lsl r2, r4, #0x1F
	lsr r1, r2, #0x1F
	sub r1, r6, r1
	and r1, r6
	mov r0, #0x94
	add r3, r7, #0
	mul r3, r0
	mov r0, sl
	mul r0, r1
	add r0, r3, r0
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	lsr r2, r2, #0x1F
	sub r2, r6, r2
	and r2, r6
	mov r0, sl
	mul r0, r2
	add r3, r3, r0
	add r3, r9
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _0803AB58
	cmp r5, #0
	beq _0803AB58
	ldr r2, _0803AB10 @ =0x000007FF
	add r1, r2, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0803AB14 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0803AB20
	cmp r0, #0x17
	ble _0803AB18
	cmp r0, #0x18
	beq _0803AB1C
	b _0803AB20
	.align 2, 0
_0803AB08: .4byte 0x00000D64
_0803AB0C: .4byte 0x0201930C
_0803AB10: .4byte 0x000007FF
_0803AB14: .4byte gCardStats
_0803AB18:
	mov r0, #0
	b _0803AB36
_0803AB1C:
	mov r0, #0xA
	b _0803AB36
_0803AB20:
	ldr r2, _0803AB70 @ =0x000007FF
	add r0, r2, #0
	and r5, r0
	lsl r0, r5, #2
	ldr r1, _0803AB74 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0803AB36:
	cmp r0, #4
	bne _0803AB58
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	add r1, r7, #0
	bl DestroyFieldCardByEffect
	mov r2, r8
	ldrb r2, [r2, #2]
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r6, r1
	add r2, r7, #0
	bl OnCardDestroyedByEffect
_0803AB58:
	add r7, #1
	cmp r7, #4
	ble _0803AAA4
_0803AB5E:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803AB70: .4byte 0x000007FF
_0803AB74: .4byte gCardStats
	thumb_func_end EffectDestroyOpponentLevel4Resolve

