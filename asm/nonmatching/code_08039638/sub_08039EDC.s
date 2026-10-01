	thumb_func_start sub_08039EDC
sub_08039EDC: @ 0x08039EDC
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08039F60
	ldr r0, _08039F20 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08039F2C
	cmp r0, #0x80
	bne _08039F60
	ldr r2, _08039F24 @ =0x020192E4
	ldrb r1, [r1, #2]
	lsl r3, r1, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _08039F28 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08039F1C
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0802272C
_08039F1C:
	mov r0, #0x7F
	b _08039F62
_08039F20: .4byte 0x02017A40
_08039F24: .4byte 0x020192E4
_08039F28: .4byte 0x00000D64
_08039F2C:
	ldr r2, _08039F58 @ =0x020192E4
	ldrb r1, [r1, #2]
	lsl r4, r1, #0x1F
	lsr r0, r4, #0x1F
	mov r3, #1
	sub r0, r3, r0
	and r0, r3
	ldr r1, _08039F5C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08039F52
	lsr r0, r4, #0x1F
	sub r0, r3, r0
	mov r1, #1
	mov r2, #0
	bl sub_0802272C
_08039F52:
	mov r0, #0x7E
	b _08039F62
	.align 2, 0
_08039F58: .4byte 0x020192E4
_08039F5C: .4byte 0x00000D64
_08039F60:
	mov r0, #0
_08039F62:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08039EDC

