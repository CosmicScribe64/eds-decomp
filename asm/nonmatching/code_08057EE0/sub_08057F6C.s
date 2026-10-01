	thumb_func_start sub_08057F6C
sub_08057F6C: @ 0x08057F6C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r0, #0
	mov r8, r0
	ldr r0, _08057FB4 @ =0x02015F00
	mov r1, #0xD9
	lsl r1, r1, #5
	add r5, r0, r1
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r5]
	mov r0, #1
	bl sub_0804A92C
	cmp r0, #0
	beq _0805805A
	ldr r4, _08057FB8 @ =0x020192E4
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0804A848
	ldr r7, _08057FBC @ =0x00000D8A
	add r7, r7, r4
	mov r9, r7
	ldr r0, _08057FC0 @ =0xFFFFF000
	add r6, r0, #0
	b _08058050
_08057FB4: .4byte 0x02015F00
_08057FB8: .4byte 0x020192E4
_08057FBC: .4byte 0x00000D8A
_08057FC0: .4byte 0xFFFFF000
_08057FC4:
	ldr r4, _08058068 @ =0x020192E4
	ldr r3, _0805806C @ =0x02015F00
	ldrb r1, [r3, #0xC]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x1D
	mov r1, #1
	lsl r1, r0
	mov r2, r9
	ldrh r2, [r2]
	orr r1, r2
	mov r7, r9
	strh r1, [r7]
	ldrb r1, [r3, #0xD]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _08058012
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1F
	mov r1, #0x94
	mul r1, r0
	ldr r2, _08058070 @ =0x00000D8C
	add r0, r4, r2
	add r1, r1, r0
	ldrh r0, [r1]
	and r0, r6
	strh r0, [r1]
	ldrb r2, [r5]
	lsl r0, r2, #0x1D
	lsr r0, r0, #0x1D
	add r0, #1
	mov r1, #7
	and r0, r1
	mov r7, #8
	neg r7, r7
	add r1, r7, #0
	and r2, r1
	orr r2, r0
	strb r2, [r5]
_08058012:
	ldrb r1, [r3, #0xD]
	mov r0, #8
	and r0, r1
	cmp r0, #0
	beq _0805804A
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1F
	mov r1, #0x94
	mul r1, r0
	add r0, r4, #0
	add r0, #0x28
	add r1, r1, r0
	ldrh r0, [r1]
	and r0, r6
	strh r0, [r1]
	ldrb r2, [r5]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1D
	add r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #3
	mov r4, #0x39
	neg r4, r4
	add r1, r4, #0
	and r2, r1
	orr r2, r0
	strb r2, [r5]
_0805804A:
	mov r7, #0x10
	ldsh r0, [r3, r7]
	add r8, r0
_08058050:
	bl sub_08057C94
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08057FC4
_0805805A:
	mov r0, r8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08058068: .4byte 0x020192E4
_0805806C: .4byte 0x02015F00
_08058070: .4byte 0x00000D8C
	thumb_func_end sub_08057F6C

