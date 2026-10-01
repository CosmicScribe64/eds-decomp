	thumb_func_start sub_08031C14
sub_08031C14: @ 0x08031C14
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08031D1C
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08031D1C
	ldrb r5, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r6, r1, #8
	and r2, r5
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08031CA4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08031CA8 @ =0x0201930C
	add r7, r1, r0
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08031D1C
	ldr r0, _08031CAC @ =0x000007FF
	mov ip, r0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08031CB0 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08031CB4 @ =0x000003FF
	cmp r1, r0
	beq _08031C6A
	add r0, #0xBC
	cmp r1, r0
	bne _08031CC0
_08031C6A:
	mov r0, ip
	and r2, r0
	lsl r0, r2, #1
	add r0, r0, r3
	ldr r1, _08031CB8 @ =0x000004B1
	ldrh r0, [r0]
	cmp r0, r1
	bne _08031CC0
	mov r0, #3
	ldrb r1, [r7, #6]
	and r0, r1
	cmp r0, #1
	bne _08031CC0
	mov r0, #0x7F
	cmp r5, #0
	beq _08031C8C
	ldr r0, _08031CBC @ =0x0000807F
_08031C8C:
	add r1, r6, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r1, [r7]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r5, #0
	bl sub_080197C0
	b _08031D1C
_08031CA4: .4byte 0x00000D64
_08031CA8: .4byte 0x0201930C
_08031CAC: .4byte 0x000007FF
_08031CB0: .4byte gUnk_08622AB4
_08031CB4: .4byte 0x000003FF
_08031CB8: .4byte 0x000004B1
_08031CBC: .4byte 0x0000807F
_08031CC0:
	ldr r0, _08031D24 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08031D28 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08031CEA
	cmp r6, #4
	bgt _08031CEA
	add r0, r5, #0
	add r1, r6, #0
	bl sub_0802B28C
	cmp r0, #0
	beq _08031D1C
_08031CEA:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r5
	beq _08031D06
	mov r0, #0x8B
	cmp r5, #0
	beq _08031CFC
	ldr r0, _08031D2C @ =0x0000808B
_08031CFC:
	add r1, r6, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_08031D06:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08030028
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r6, #0
	bl sub_08046CB0
_08031D1C:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08031D24: .4byte 0x000007FF
_08031D28: .4byte gUnk_08621DE0
_08031D2C: .4byte 0x0000808B
	thumb_func_end sub_08031C14

