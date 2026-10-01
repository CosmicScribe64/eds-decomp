	thumb_func_start sub_08006FAC
sub_08006FAC: @ 0x08006FAC
	push {r4, lr}
	bl sub_080064AC
	ldr r0, _08006FCC @ =0x03000040
	ldr r1, _08006FD0 @ =0x0000485A
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0xA
	bls _08006FC0
	b _080071F0
_08006FC0:
	lsl r0, r0, #2
	ldr r1, _08006FD4 @ =0x08006FD8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08006FCC: .4byte 0x03000040
_08006FD0: .4byte 0x0000485A
_08006FD4: .4byte 0x08006FD8
_08006FD8:
	.4byte _08007004
	.4byte _080070D8
	.4byte _080070F8
	.4byte _080071D0
	.4byte _080071F0
	.4byte _080071F0
	.4byte _080071F0
	.4byte _080071F0
	.4byte _080071F0
	.4byte _080071F0
	.4byte _080070D8
_08007004:
	mov r4, #0
	ldr r1, _08007048 @ =0x03000040
	mov r0, #0x10
	ldrh r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	beq _08007026
	ldr r0, _0800704C @ =0x02013D90
	add r3, r0, #0
	add r3, #0x40
	ldrh r2, [r3]
	ldr r0, _08007050 @ =0x00000333
	cmp r2, r0
	bhi _08007026
	add r0, r2, #1
	strh r0, [r3]
	mov r4, #1
_08007026:
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	beq _08007060
	ldr r0, _0800704C @ =0x02013D90
	add r3, r0, #0
	add r3, #0x40
	ldrh r2, [r3]
	ldr r0, _08007054 @ =0x00000329
	cmp r2, r0
	bhi _08007058
	add r0, r2, #0
	add r0, #0xA
	b _0800705C
	.align 2, 0
_08007048: .4byte 0x03000040
_0800704C: .4byte 0x02013D90
_08007050: .4byte 0x00000333
_08007054: .4byte 0x00000329
_08007058:
	mov r0, #0xCD
	lsl r0, r0, #2
_0800705C:
	strh r0, [r3]
	mov r4, #1
_08007060:
	mov r0, #0x20
	ldrh r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	beq _0800707C
	ldr r0, _08007098 @ =0x02013D90
	add r2, r0, #0
	add r2, #0x40
	ldrh r0, [r2]
	cmp r0, #1
	bls _0800707C
	sub r0, #1
	strh r0, [r2]
	mov r4, #1
_0800707C:
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080070A2
	ldr r0, _08007098 @ =0x02013D90
	add r1, r0, #0
	add r1, #0x40
	ldrh r0, [r1]
	cmp r0, #0xA
	bls _0800709C
	sub r0, #0xA
	b _0800709E
_08007098: .4byte 0x02013D90
_0800709C:
	mov r0, #1
_0800709E:
	strh r0, [r1]
	mov r4, #1
_080070A2:
	bl sub_08006AE8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080070C0
	ldr r0, _080070B8 @ =0x03000040
	ldr r1, _080070BC @ =0x0000485A
	add r0, r0, r1
	mov r1, #0xA
	b _080071E2
	.align 2, 0
_080070B8: .4byte 0x03000040
_080070BC: .4byte 0x0000485A
_080070C0:
	cmp r4, #0
	bne _080070C6
	b _080071E4
_080070C6:
	ldr r0, _080070D0 @ =0x03000040
	ldr r2, _080070D4 @ =0x0000485A
	add r0, r0, r2
	mov r1, #1
	b _080071E2
_080070D0: .4byte 0x03000040
_080070D4: .4byte 0x0000485A
_080070D8:
	bl sub_08006ABC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080070E4
	b _080071E4
_080070E4:
	ldr r0, _080070F0 @ =0x03000040
	ldr r1, _080070F4 @ =0x0000485A
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	b _080071E2
_080070F0: .4byte 0x03000040
_080070F4: .4byte 0x0000485A
_080070F8:
	ldr r2, _08007128 @ =0x02013D90
	add r1, r2, #0
	add r1, #0x40
	ldrh r0, [r1]
	strh r0, [r2, #2]
	ldrh r3, [r1]
	ldr r0, _0800712C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007130 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800713E
	cmp r0, #0x17
	ble _08007134
	cmp r0, #0x18
	beq _08007138
	b _0800713E
	.align 2, 0
_08007128: .4byte 0x02013D90
_0800712C: .4byte 0x000007FF
_08007130: .4byte gUnk_08621DE0
_08007134:
	mov r0, #0
	b _08007154
_08007138:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08007154
_0800713E:
	ldr r0, _08007180 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _08007184 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08007154:
	str r0, [r2, #0x2C]
	add r0, r2, #0
	add r0, #0x40
	ldrh r3, [r0]
	ldr r0, _08007180 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007184 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08007192
	cmp r0, #0x17
	ble _08007188
	cmp r0, #0x18
	beq _0800718C
	b _08007192
	.align 2, 0
_08007180: .4byte 0x000007FF
_08007184: .4byte gUnk_08621DE0
_08007188:
	mov r0, #0
	b _080071A8
_0800718C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080071A8
_08007192:
	ldr r0, _080071BC @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _080071C0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _080071C4 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080071A8:
	str r0, [r2, #0x30]
	bl sub_08006B80
	ldr r0, _080071C8 @ =0x03000040
	ldr r2, _080071CC @ =0x0000485A
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	b _080071E2
	.align 2, 0
_080071BC: .4byte 0x000007FF
_080071C0: .4byte gUnk_08621DE0
_080071C4: .4byte 0x000001FF
_080071C8: .4byte 0x03000040
_080071CC: .4byte 0x0000485A
_080071D0:
	bl sub_08006A98
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080071E4
	ldr r0, _080071E8 @ =0x03000040
	ldr r1, _080071EC @ =0x0000485A
	add r0, r0, r1
	mov r1, #0
_080071E2:
	strb r1, [r0]
_080071E4:
	mov r0, #0
	b _080071F2
_080071E8: .4byte 0x03000040
_080071EC: .4byte 0x0000485A
_080071F0:
	mov r0, #1
_080071F2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08006FAC

