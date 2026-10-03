	thumb_func_start EffectDestroyRepelledAttackerResolve
EffectDestroyRepelledAttackerResolve: @ 0x0803C4F8
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldrb r0, [r5, #8]
	mov r6, #0xF
	and r6, r0
	lsr r4, r0, #4
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C53E
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0803C548 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803C54C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803C53E
	add r0, r6, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_0803C53E:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803C548: .4byte 0x00000D64
_0803C54C: .4byte 0x0201930C
	thumb_func_end EffectDestroyRepelledAttackerResolve

