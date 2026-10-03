	thumb_func_start EffectPolymerizationPrepare
EffectPolymerizationPrepare: @ 0x0802DEC4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	add r5, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	bne _0802DEE4
	b _0802DF40
_0802DEE0:
	mov r0, #1
	b _0802DF42
_0802DEE4:
	mov r4, #0
	ldr r1, _0802DF50 @ =0x020192E4
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _0802DF54 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #5]
	cmp r4, r0
	bge _0802DF40
	mov r7, #1
	add r6, r3, #0
	ldr r0, _0802DF58 @ =0x00000A44
	add r0, r0, r1
	mov r9, r0
	mov r8, r1
_0802DF06:
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	add r2, r7, #0
	and r2, r1
	lsl r1, r4, #2
	mul r2, r6
	add r1, r1, r2
	add r1, r9
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, sp
	bl FindFusionMaterials
	cmp r0, #0
	bne _0802DEE0
	add r4, #1
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r7, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r8
	ldrb r0, [r0, #5]
	cmp r4, r0
	blt _0802DF06
_0802DF40:
	mov r0, #0
_0802DF42:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802DF50: .4byte 0x020192E4
_0802DF54: .4byte 0x00000D64
_0802DF58: .4byte 0x00000A44
	thumb_func_end EffectPolymerizationPrepare

