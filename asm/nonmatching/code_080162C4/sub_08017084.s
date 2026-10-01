	thumb_func_start sub_08017084
sub_08017084: @ 0x08017084
	push {r4, r5, lr}
	ldr r5, _0801709C @ =0x020185C0
	ldr r0, _080170A0 @ =0x0000080A
	add r4, r5, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _080170A4
	cmp r0, #1
	beq _080170C4
	b _080170DA
_0801709C: .4byte 0x020185C0
_080170A0: .4byte 0x0000080A
_080170A4:
	mov r0, #5
	mov r1, #0
	bl sub_0801E998
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _080170DA
_080170C4:
	bl sub_0801EAD8
	cmp r0, #0
	beq _080170DA
	ldr r2, _080170E0 @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080170DA:
	pop {r4, r5}
	pop {r0}
	bx r0
_080170E0: .4byte 0x0000080D
	thumb_func_end sub_08017084

