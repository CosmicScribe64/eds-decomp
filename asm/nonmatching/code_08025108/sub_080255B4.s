	thumb_func_start sub_080255B4
sub_080255B4: @ 0x080255B4
	push {r4, lr}
	ldr r4, _080255CC @ =0x0201F820
	ldr r1, _080255D0 @ =0x00000AEA
	add r0, r4, r1
	ldrb r0, [r0]
	cmp r0, #1
	beq _080255F0
	cmp r0, #1
	bgt _080255D4
	cmp r0, #0
	beq _080255DA
	b _0802563E
_080255CC: .4byte 0x0201F820
_080255D0: .4byte 0x00000AEA
_080255D4:
	cmp r0, #2
	beq _08025604
	b _0802563E
_080255DA:
	ldr r0, _080255E8 @ =0x081999F8
	ldr r2, _080255EC @ =0x00000918
	add r1, r4, r2
	bl sub_08078670
	b _0802563E
	.align 2, 0
_080255E8: .4byte gUnk_081999F8
_080255EC: .4byte 0x00000918
_080255F0:
	ldr r0, _080255FC @ =0x08199A04
	ldr r3, _08025600 @ =0x00000918
	add r1, r4, r3
	bl sub_08078670
	b _0802563E
_080255FC: .4byte gUnk_08199A04
_08025600: .4byte 0x00000918
_08025604:
	ldr r0, _08025698 @ =0x08199A04
	ldr r2, _0802569C @ =0x00000918
	add r1, r4, r2
	bl sub_08078670
	ldr r3, _080256A0 @ =0x00000ADA
	add r0, r4, r3
	mov r2, #0
	mov r1, #0x80
	lsl r1, r1, #1
	strh r1, [r0]
	add r3, #2
	add r0, r4, r3
	strh r1, [r0]
	ldr r1, _080256A4 @ =0x00000AD8
	add r0, r4, r1
	strb r2, [r0]
	mov r2, #0xAE
	lsl r2, r2, #4
	add r3, r4, r2
	mov r0, #0
	mov r1, #0xF
	mov r2, #1
	bl sub_0807B100
	ldr r3, _080256A8 @ =0x00000AC4
	add r1, r4, r3
	mov r0, #1
	strb r0, [r1]
_0802563E:
	ldr r1, _080256AC @ =0xFFFFFE80
	ldr r3, _080256B0 @ =0x020202DC
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	ldr r0, _080256B4 @ =0x086A12EC
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r2, #0x96
	lsl r2, r2, #7
	bl CpuSet
	ldr r0, _080256B8 @ =0x086AA8EC
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #1
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080256BC @ =0x086B2168
	ldr r1, _080256C0 @ =0x06014000
	mov r2, #0x10
	bl sub_08077CEC
	ldr r0, _080256C4 @ =0x086B4168
	ldr r1, _080256C8 @ =0x06014200
	mov r2, #0x10
	bl sub_08077CEC
	ldr r0, _080256CC @ =0x086B6168
	ldr r1, _080256D0 @ =0x05000200
	add r2, r4, #0
	bl CpuSet
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _080256D4 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
_08025698: .4byte gUnk_08199A04
_0802569C: .4byte 0x00000918
_080256A0: .4byte 0x00000ADA
_080256A4: .4byte 0x00000AD8
_080256A8: .4byte 0x00000AC4
_080256AC: .4byte 0xFFFFFE80
_080256B0: .4byte 0x020202DC
_080256B4: .4byte gUnk_086A12EC
_080256B8: .4byte gUnk_086AA8EC
_080256BC: .4byte gUnk_086B2168
_080256C0: .4byte 0x06014000
_080256C4: .4byte gUnk_086B4168
_080256C8: .4byte 0x06014200
_080256CC: .4byte gUnk_086B6168
_080256D0: .4byte 0x05000200
_080256D4: .4byte 0x00001F04
	thumb_func_end sub_080255B4

