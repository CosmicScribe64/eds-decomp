	thumb_func_start sub_08031A24
sub_08031A24: @ 0x08031A24
	push {r4, r5, lr}
	add r4, r0, #0
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	cmp r0, #0
	beq _08031A78
	ldr r1, _08031A68 @ =0x020192E4
	ldr r0, _08031A6C @ =0x000003E7
	ldrh r2, [r1]
	cmp r2, r0
	bhi _08031AE4
	ldr r2, _08031A70 @ =0x00000D64
	add r0, r1, r2
	mov r1, #0xFA
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bls _08031AE4
	ldr r0, _08031A74 @ =0x00008043
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019860
	b _08031AE4
_08031A68: .4byte 0x020192E4
_08031A6C: .4byte 0x000003E7
_08031A70: .4byte 0x00000D64
_08031A74: .4byte 0x00008043
_08031A78:
	ldr r0, _08031AA4 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08031AB4
	cmp r0, #0x80
	bne _08031AE4
	ldr r0, _08031AA8 @ =0x00000205
	ldr r1, _08031AAC @ =0x00000914
	ldr r3, _08031AB0 @ =0x08082AF0
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7F
	b _08031AE6
_08031AA4: .4byte 0x02017A40
_08031AA8: .4byte 0x00000205
_08031AAC: .4byte 0x00000914
_08031AB0: .4byte gUnk_08082AF0
_08031AB4:
	ldr r0, _08031AE0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08031ADA
	mov r1, #0xFA
	lsl r1, r1, #3
	mov r0, #0x43
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019860
_08031ADA:
	mov r0, #0x7E
	b _08031AE6
	.align 2, 0
_08031AE0: .4byte 0x0201AE60
_08031AE4:
	mov r0, #0
_08031AE6:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08031A24

