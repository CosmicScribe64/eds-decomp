	thumb_func_start TextDrawLatinString
TextDrawLatinString: @ 0x08074F50
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r3, #0
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	mov sl, r3
	lsr r6, r2, #0x18
	add r4, r0, #0
	add r7, r1, #0
	mov r9, r4
	ldr r2, _08075008 @ =0x02000000
	ldr r1, _0807500C @ =0x00010002
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _08075010 @ =0x00010003
	add r0, r2, r3
	strb r1, [r0]
	ldrb r0, [r5]
	cmp r0, #0
	beq _08074FFA
	ldr r0, _08075014 @ =0x00010004
	add r0, r0, r2
	mov r8, r0
_08074F86:
	mov r0, #0x80
	mov r1, r8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08074FBE
	add r0, r5, #0
	bl TextWordLength
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r1, r0, #0
	mul r1, r6
	asr r1, r1, #1
	add r1, r4, r1
	ldr r2, _08075018 @ =0x02010000
	ldrb r0, [r2]
	sub r0, #2
	lsl r0, r0, #3
	cmp r1, r0
	ble _08074FBE
	mov r4, r9
	mov r3, r8
	ldrb r3, [r3]
	lsl r0, r3, #0x19
	lsr r0, r0, #0x19
	add r0, r6, r0
	add r7, r7, r0
_08074FBE:
	ldrb r0, [r5]
	add r1, r4, #0
	add r2, r7, #0
	mov r3, sl
	bl TextDrawLatinGlyph
	ldr r3, _08075008 @ =0x02000000
	ldr r0, _0807500C @ =0x00010002
	add r2, r3, r0
	lsr r0, r6, #1
	add r1, r4, r0
	ldrb r0, [r2]
	cmp r0, r1
	bge _08074FDC
	strb r1, [r2]
_08074FDC:
	ldr r0, _08075010 @ =0x00010003
	add r2, r3, r0
	add r0, r7, r6
	ldrb r3, [r2]
	cmp r3, r0
	bge _08074FEA
	strb r0, [r2]
_08074FEA:
	add r4, r1, #0
	cmp r6, #0x10
	bne _08074FF2
	add r4, r1, #1
_08074FF2:
	add r5, #1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08074F86
_08074FFA:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08075008: .4byte 0x02000000
_0807500C: .4byte 0x00010002
_08075010: .4byte 0x00010003
_08075014: .4byte 0x00010004
_08075018: .4byte 0x02010000
	thumb_func_end TextDrawLatinString

