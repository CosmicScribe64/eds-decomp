	thumb_func_start DuelLink_RunRemoteResolve
DuelLink_RunRemoteResolve: @ 0x08051140
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, _08051160 @ =0x02017FB0
	ldr r0, _08051164 @ =0x0000048F
	add r0, r0, r7
	mov r8, r0
	ldrb r1, [r0]
	cmp r1, #0
	beq _08051168
	cmp r1, #1
	bne _0805115A
	b _08051268
_0805115A:
	mov r0, #1
	b _080512D4
	.align 2, 0
_08051160: .4byte 0x02017FB0
_08051164: .4byte 0x0000048F
_08051168:
	ldr r1, _08051240 @ =0x0000045E
	add r6, r7, r1
	ldrb r5, [r6]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	mov r4, #1
	sub r1, r4, r1
	mov r3, #1
	and r1, r3
	mov r2, #2
	neg r2, r2
	add r0, r2, #0
	and r0, r5
	orr r0, r1
	strb r0, [r6]
	ldr r0, _08051244 @ =0x00000472
	add r5, r7, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r4, r4, r0
	and r4, r3
	and r2, r1
	orr r2, r4
	strb r2, [r5]
	ldr r1, _08051248 @ =0x00000462
	add r2, r7, r1
	ldrh r1, [r2]
	sub r0, r3, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r2]
	ldr r0, _0805124C @ =0x00000464
	add r2, r7, r0
	ldrh r1, [r2]
	sub r0, r3, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r2]
	ldr r1, _08051250 @ =0x00000476
	add r2, r7, r1
	ldrh r1, [r2]
	sub r0, r3, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r2]
	mov r2, #0x8F
	lsl r2, r2, #3
	add r1, r7, r2
	ldrh r0, [r1]
	sub r3, r3, r0
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsr r0, r0, #8
	lsl r0, r0, #8
	orr r3, r0
	strh r3, [r1]
	ldr r3, _08051254 @ =0x0000045C
	add r0, r7, r3
	ldrh r0, [r0]
	bl FindCardEffect
	ldr r4, _08051258 @ =0x02017A40
	ldr r2, _0805125C @ =0x000003D6
	add r1, r4, r2
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	blt _0805115A
	mov r0, #0xF6
	lsl r0, r0, #2
	add r3, r4, r0
	ldr r2, _08051260 @ =0x0819A9D4
	mov r0, #0
	ldsh r1, [r1, r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r2, #4
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [r3]
	cmp r0, #0
	beq _0805115A
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r4, r2
	mov r0, #0x80
	strb r0, [r1]
	ldr r3, _08051264 @ =0x000003E1
	add r1, r4, r3
	mov r0, #0
	strb r0, [r1]
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _080512D2
	.align 2, 0
_08051240: .4byte 0x0000045E
_08051244: .4byte 0x00000472
_08051248: .4byte 0x00000462
_0805124C: .4byte 0x00000464
_08051250: .4byte 0x00000476
_08051254: .4byte 0x0000045C
_08051258: .4byte 0x02017A40
_0805125C: .4byte 0x000003D6
_08051260: .4byte gCardEffects
_08051264: .4byte 0x000003E1
_08051268:
	mov r2, #0x92
	lsl r2, r2, #3
	add r0, r7, r2
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _0805129C
	ldr r4, _08051294 @ =0x02017A40
	mov r3, #0xF6
	lsl r3, r3, #2
	add r2, r4, r3
	ldr r1, _08051298 @ =0x0000045C
	add r0, r7, r1
	add r3, #0x98
	add r1, r7, r3
	ldr r2, [r2]
	bl _call_via_r2
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r4, r2
	b _080512B6
_08051294: .4byte 0x02017A40
_08051298: .4byte 0x0000045C
_0805129C:
	ldr r4, _080512E0 @ =0x02017A40
	mov r3, #0xF6
	lsl r3, r3, #2
	add r1, r4, r3
	ldr r2, _080512E4 @ =0x0000045C
	add r0, r7, r2
	ldr r2, [r1]
	mov r1, #0
	bl _call_via_r2
	mov r3, #0xF8
	lsl r3, r3, #2
	add r1, r4, r3
_080512B6:
	strb r0, [r1]
	ldr r0, _080512E0 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _080512D2
	ldr r0, _080512E8 @ =0x02017FB0
	ldr r2, _080512EC @ =0x0000048F
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_080512D2:
	mov r0, #0
_080512D4:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080512E0: .4byte 0x02017A40
_080512E4: .4byte 0x0000045C
_080512E8: .4byte 0x02017FB0
_080512EC: .4byte 0x0000048F
	thumb_func_end DuelLink_RunRemoteResolve

