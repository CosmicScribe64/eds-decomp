	thumb_func_start DeckEdit_SetListCard
DeckEdit_SetListCard: @ 0x08068DA4
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r5, r1, #0
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	lsl r3, r3, #0x10
	lsr r0, r3, #0x10
	cmp r1, #1
	beq _08068DE4
	cmp r1, #1
	bgt _08068DC6
	cmp r1, #0
	beq _08068DCC
	b _08068E10
_08068DC6:
	cmp r5, #2
	beq _08068DFC
	b _08068E10
_08068DCC:
	ldr r2, _08068DDC @ =0x0201DB20
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r4
	add r0, r0, r1
	ldr r1, _08068DE0 @ =0x00000644
	b _08068E0A
_08068DDC: .4byte 0x0201DB20
_08068DE0: .4byte 0x00000644
_08068DE4:
	ldr r2, _08068DF4 @ =0x0201DB20
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r4
	add r0, r0, r1
	ldr r1, _08068DF8 @ =0x00000CAE
	b _08068E0A
_08068DF4: .4byte 0x0201DB20
_08068DF8: .4byte 0x00000CAE
_08068DFC:
	ldr r2, _08068E18 @ =0x0201DB20
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r4
	add r0, r0, r1
	ldr r1, _08068E1C @ =0x00000D4E
_08068E0A:
	add r2, r2, r1
	add r0, r0, r2
	strh r6, [r0]
_08068E10:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08068E18: .4byte 0x0201DB20
_08068E1C: .4byte 0x00000D4E
	thumb_func_end DeckEdit_SetListCard

