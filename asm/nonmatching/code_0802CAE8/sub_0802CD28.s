	thumb_func_start sub_0802CD28
sub_0802CD28: @ 0x0802CD28
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r0, _0802CD5C @ =0x000007FF
	and r0, r3
	lsl r2, r0, #1
	ldr r4, _0802CD60 @ =0x08622AB4
	add r1, r2, r4
	ldrh r5, [r1]
	lsl r0, r0, #2
	ldr r1, _0802CD64 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r4, r0, #0x14
	ldr r0, _0802CD60 @ =0x08622AB4
	add r2, r2, r0
	ldrh r1, [r2]
	ldr r0, _0802CD68 @ =0x00000776
	cmp r1, r0
	bne _0802CD6C
	mov r0, #3
	b _0802CDCE
	.align 2, 0
_0802CD5C: .4byte 0x000007FF
_0802CD60: .4byte gUnk_08622AB4
_0802CD64: .4byte gUnk_08621DE0
_0802CD68: .4byte 0x00000776
_0802CD6C:
	cmp r1, r0
	blt _0802CD7C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0802CD7C
	mov r0, #1
	b _0802CDCE
_0802CD7C:
	ldr r0, _0802CDA0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0802CDA4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0802CDAE
	cmp r0, #0x16
	bgt _0802CDA8
	cmp r0, #0x15
	beq _0802CDB2
	b _0802CDBA
	.align 2, 0
_0802CDA0: .4byte 0x000007FF
_0802CDA4: .4byte gUnk_08621DE0
_0802CDA8:
	cmp r0, #0x17
	beq _0802CDB6
	b _0802CDBA
_0802CDAE:
	mov r0, #7
	b _0802CDCE
_0802CDB2:
	mov r0, #8
	b _0802CDCE
_0802CDB6:
	mov r0, #9
	b _0802CDCE
_0802CDBA:
	ldr r0, _0802CDF8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0802CDFC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0802CDCE:
	add r2, r0, #0
	ldr r0, _0802CDF8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0802CDFC @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802CE00
	cmp r0, #0x15
	blt _0802CE00
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0802CE02
	.align 2, 0
_0802CDF8: .4byte 0x000007FF
_0802CDFC: .4byte gUnk_08621DE0
_0802CE00:
	mov r0, #0
_0802CE02:
	cmp r4, #0x15
	beq _0802CE1C
	cmp r4, #0x16
	beq _0802CE24
	cmp r2, #1
	bne _0802CE30
	add r0, r5, #0
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	beq _0802CE2C
	b _0802CE28
_0802CE1C:
	cmp r0, #1
	bne _0802CE28
	mov r0, #3
	b _0802CE32
_0802CE24:
	cmp r0, #5
	bne _0802CE2C
_0802CE28:
	mov r0, #2
	b _0802CE32
_0802CE2C:
	mov r0, #1
	b _0802CE32
_0802CE30:
	mov r0, #0
_0802CE32:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802CD28

