	thumb_func_start sub_08030A64
sub_08030A64: @ 0x08030A64
	push {r4, r5, lr}
	sub sp, #0x100
	add r4, r0, #0
	ldrb r3, [r4, #2]
	mov r0, #0xE
	and r0, r3
	cmp r0, #6
	beq _08030A80
	add r0, r4, #0
	bl sub_08030880
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08030B42
_08030A80:
	ldr r0, _08030AD0 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08030AF0
	cmp r0, #0x80
	bne _08030B40
	ldr r2, _08030AD4 @ =0x020192E4
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08030AD8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _08030ADC @ =0x000001F3
	ldrh r0, [r0]
	cmp r0, r1
	bls _08030B40
	ldr r1, _08030AE0 @ =0x0808292C
	ldrh r4, [r4]
	lsl r2, r4, #6
	ldr r0, _08030AE4 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _08030AE8 @ =0x00000205
	ldr r1, _08030AEC @ =0x00000914
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7F
	b _08030B42
_08030AD0: .4byte 0x02017A40
_08030AD4: .4byte 0x020192E4
_08030AD8: .4byte 0x00000D64
_08030ADC: .4byte 0x000001F3
_08030AE0: .4byte gUnk_0808292C
_08030AE4: .4byte gUnk_0822C720
_08030AE8: .4byte 0x00000205
_08030AEC: .4byte 0x00000914
_08030AF0:
	ldr r0, _08030B34 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08030B2E
	mov r5, #1
	add r0, r5, #0
	and r0, r3
	mov r2, #0x43
	cmp r0, #0
	beq _08030B06
	ldr r2, _08030B38 @ =0x00008043
_08030B06:
	mov r1, #0xFA
	lsl r1, r1, #1
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xD0
	cmp r0, #0
	beq _08030B22
	ldr r2, _08030B3C @ =0x000080D0
_08030B22:
	ldrh r1, [r4]
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08030B2E:
	mov r0, #0x64
	b _08030B42
	.align 2, 0
_08030B34: .4byte 0x0201AE60
_08030B38: .4byte 0x00008043
_08030B3C: .4byte 0x000080D0
_08030B40:
	mov r0, #0
_08030B42:
	add sp, #0x100
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08030A64
	.align 2, 0

