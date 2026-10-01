	thumb_func_start sub_08014AA8
sub_08014AA8: @ 0x08014AA8
	push {r4, lr}
	ldr r0, _08014AE0 @ =0x020185C0
	ldrh r1, [r0, #2]
	add r4, r0, #0
	cmp r1, #0
	beq _08014AEC
	ldr r3, _08014AE4 @ =0x020192E4
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0
	cmp r0, #0
	beq _08014ACA
	ldr r1, _08014AE8 @ =0x00000D64
_08014ACA:
	add r2, r1, r3
	mov r1, #0
	cmp r0, #0
	beq _08014AD4
	ldr r1, _08014AE8 @ =0x00000D64
_08014AD4:
	add r0, r1, r3
	ldrb r0, [r0, #0xC]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1D
	add r1, #1
	b _08014B2A
_08014AE0: .4byte 0x020185C0
_08014AE4: .4byte 0x020192E4
_08014AE8: .4byte 0x00000D64
_08014AEC:
	ldr r3, _08014B50 @ =0x020192E4
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	mov r0, #0
	cmp r1, #0
	beq _08014B02
	ldr r0, _08014B54 @ =0x00000D64
_08014B02:
	add r0, r0, r3
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	cmp r0, #0
	beq _08014B3C
	mov r0, #0
	cmp r1, #0
	beq _08014B16
	ldr r0, _08014B54 @ =0x00000D64
_08014B16:
	add r2, r0, r3
	mov r0, #0
	cmp r1, #0
	beq _08014B20
	ldr r0, _08014B54 @ =0x00000D64
_08014B20:
	add r0, r0, r3
	ldrb r0, [r0, #0xC]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1D
	sub r1, #1
_08014B2A:
	mov r0, #7
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r3, [r2, #0xC]
	and r0, r3
	orr r0, r1
	strb r0, [r2, #0xC]
_08014B3C:
	ldr r0, _08014B58 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_08014B50: .4byte 0x020192E4
_08014B54: .4byte 0x00000D64
_08014B58: .4byte 0x0000080D
	thumb_func_end sub_08014AA8

