	thumb_func_start RenderStringToTiles
RenderStringToTiles: @ 0x080791F4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r1
	lsl r2, r2, #0x18
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r6, r3, #0x18
	mov r1, #0
	mov r9, r1
	mov r5, #0
	mov sl, r5
	add r4, r0, #0
	b _08079288
_08079216:
	ldr r1, _08079250 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08079258
	ldrb r3, [r4]
	lsl r2, r3, #8
	ldrb r0, [r4, #1]
	orr r2, r0
	ldr r0, _08079254 @ =0x0000813F
	cmp r2, r0
	bls _0807924C
	add r1, r5, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, sl
	lsl r0, r3, #5
	add r1, r1, r0
	lsl r1, r1, #5
	add r1, r8
	add r0, r2, #0
	add r2, r7, #0
	add r3, r6, #0
	bl RenderFullWidthGlyph
_0807924C:
	add r4, #2
	b _08079288
_08079250: .4byte 0x02011C20
_08079254: .4byte 0x0000813F
_08079258:
	ldrb r0, [r4]
	mov r2, sl
	lsl r1, r2, #5
	add r1, r5, r1
	lsl r1, r1, #5
	add r1, r8
	mov r3, r9
	str r3, [sp, #0]
	add r2, r7, #0
	add r3, r6, #0
	bl RenderHalfWidthGlyph
	mov r0, r9
	cmp r0, #0
	beq _08079282
	mov r1, #0
	mov r9, r1
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	b _08079286
_08079282:
	mov r2, #1
	mov r9, r2
_08079286:
	add r4, #1
_08079288:
	ldrb r0, [r4]
	cmp r0, #0
	bne _08079216
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end RenderStringToTiles
	.align 2, 0

