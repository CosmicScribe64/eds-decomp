	thumb_func_start EffectHeavyStormResolve
EffectHeavyStormResolve: @ 0x08035C0C
	push {r4, r5, r6, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	bne _08035CA8
	ldr r0, _08035C8C @ =0x02017A40
	mov r4, #0xF8
	lsl r4, r4, #2
	add r2, r0, r4
	ldrb r1, [r2]
	add r5, r0, #0
	cmp r1, #0x7F
	beq _08035C44
	cmp r1, #0x80
	bne _08035CA8
	ldrb r6, [r3, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	add r4, #1
	add r0, r5, r4
	strb r1, [r0]
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
_08035C44:
	mov r2, #5
	ldr r6, _08035C90 @ =0x000003E1
	add r0, r5, r6
	ldrb r4, [r0]
	mov r1, #1
	add r0, r4, #0
	and r0, r1
	ldr r1, _08035C94 @ =0x00000D64
	mul r0, r1
	ldr r1, _08035C98 @ =0x0201930C
	add r0, r0, r1
	sub r6, #0xFD
	add r1, r0, r6
_08035C5E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08035C9C
	add r1, #0x94
	add r2, #1
	cmp r2, #0xA
	ble _08035C5E
	ldr r1, _08035C90 @ =0x000003E1
	add r0, r5, r1
	mov r1, #1
	ldrb r2, [r0]
	sub r1, r1, r2
	strb r1, [r0]
	ldrb r3, [r3, #2]
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	ldrb r0, [r0]
	cmp r0, r1
	bne _08035CA8
	mov r0, #0x7F
	b _08035CAA
	.align 2, 0
_08035C8C: .4byte 0x02017A40
_08035C90: .4byte 0x000003E1
_08035C94: .4byte 0x00000D64
_08035C98: .4byte 0x0201930C
_08035C9C:
	add r0, r4, #0
	add r1, r2, #0
	bl DestroyFieldCard
	mov r0, #0x7F
	b _08035CAA
_08035CA8:
	mov r0, #0
_08035CAA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectHeavyStormResolve

