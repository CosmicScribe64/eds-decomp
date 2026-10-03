	thumb_func_start EffectMorphingJarResolve
EffectMorphingJarResolve: @ 0x08031FAC
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08032050
	ldr r0, _08031FF0 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08031FFC
	cmp r0, #0x80
	bne _08032034
	ldr r2, _08031FF4 @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r3, r4, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _08031FF8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08032022
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
	mov r0, #0x80
	b _08032052
_08031FF0: .4byte 0x02017A40
_08031FF4: .4byte 0x020192E4
_08031FF8: .4byte 0x00000D64
_08031FFC:
	ldr r2, _08032028 @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r4, r4, #0x1F
	lsr r0, r4, #0x1F
	mov r3, #1
	sub r0, r3, r0
	and r0, r3
	ldr r1, _0803202C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08032030
	lsr r0, r4, #0x1F
	sub r0, r3, r0
	mov r1, #0
	mov r2, #1
	bl DiscardHandCard
_08032022:
	mov r0, #0x7F
	b _08032052
	.align 2, 0
_08032028: .4byte 0x020192E4
_0803202C: .4byte 0x00000D64
_08032030:
	mov r0, #0x7E
	b _08032052
_08032034:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #5
	bl DrawCards
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #5
	bl DrawCards
_08032050:
	mov r0, #0
_08032052:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectMorphingJarResolve

