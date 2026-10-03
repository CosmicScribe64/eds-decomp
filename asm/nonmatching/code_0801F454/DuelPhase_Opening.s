	thumb_func_start DuelPhase_Opening
DuelPhase_Opening: @ 0x0801F97C
	push {r4, r5, r6, lr}
	ldr r1, _0801F9A4 @ =0x020192E0
	mov r0, #0xD9
	lsl r0, r0, #5
	add r4, r1, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _0801F9AC
	cmp r0, #1
	beq _0801F9D0
	ldr r0, _0801F9A8 @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801FA28
	mov r0, #1
	b _0801FA38
	.align 2, 0
_0801F9A4: .4byte 0x020192E0
_0801F9A8: .4byte 0x00001B12
_0801F9AC:
	mov r0, #0x10
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	bl SetupStartFieldCard
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0801FA36
_0801F9D0:
	ldr r0, _0801FA20 @ =0x00001B12
	add r5, r1, r0
	mov r6, #2
	add r0, r6, #0
	ldrb r1, [r5]
	and r0, r1
	mov r1, #0x61
	cmp r0, #0
	beq _0801F9E4
	ldr r1, _0801FA24 @ =0x00008061
_0801F9E4:
	add r0, r1, #0
	mov r1, #0
	mov r2, #5
	mov r3, #0
	bl DuelCmd_Push
	add r0, r6, #0
	ldrb r5, [r5]
	and r0, r5
	mov r1, #0x61
	cmp r0, #0
	bne _0801F9FE
	ldr r1, _0801FA24 @ =0x00008061
_0801F9FE:
	add r0, r1, #0
	mov r1, #0
	mov r2, #5
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x14
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0801FA36
	.align 2, 0
_0801FA20: .4byte 0x00001B12
_0801FA24: .4byte 0x00008061
_0801FA28:
	ldr r1, _0801FA40 @ =0x02015EF0
	mov r0, #0
	strb r0, [r1]
	strb r0, [r1, #1]
	ldr r1, _0801FA44 @ =0x02015EE8
	mov r0, #8
	strb r0, [r1]
_0801FA36:
	mov r0, #0
_0801FA38:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801FA40: .4byte 0x02015EF0
_0801FA44: .4byte 0x02015EE8
	thumb_func_end DuelPhase_Opening

