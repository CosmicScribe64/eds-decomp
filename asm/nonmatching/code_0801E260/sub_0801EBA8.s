	thumb_func_start sub_0801EBA8
sub_0801EBA8: @ 0x0801EBA8
	push {r4, r5, r6, r7, lr}
	ldr r5, _0801EBCC @ =0x0000FFFF
	mov r4, #0
	ldr r2, _0801EBD0 @ =0x08198F20
	ldr r1, _0801EBD4 @ =0x03000040
	ldr r3, _0801EBD8 @ =0x00004870
	add r0, r1, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	ldr r6, _0801EBDC @ =0x02015EE8
	ldr r7, _0801EBE0 @ =0x020192E0
	mov ip, r7
	ldrh r3, [r2]
	cmp r3, r0
	bne _0801EBE4
	ldrh r5, [r2, #2]
	b _0801EC00
_0801EBCC: .4byte 0x0000FFFF
_0801EBD0: .4byte gUnk_08198F20
_0801EBD4: .4byte 0x03000040
_0801EBD8: .4byte 0x00004870
_0801EBDC: .4byte 0x02015EE8
_0801EBE0: .4byte 0x020192E0
_0801EBE4:
	add r4, #1
	cmp r4, #0x17
	bhi _0801EC00
	lsl r0, r4, #2
	add r3, r0, r2
	ldr r7, _0801EC3C @ =0x00004870
	add r0, r1, r7
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	ldrh r7, [r3]
	cmp r7, r0
	bne _0801EBE4
	ldrh r5, [r3, #2]
_0801EC00:
	ldr r2, _0801EC40 @ =0x0000487C
	add r0, r1, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x14
	and r0, r1
	cmp r0, #0
	beq _0801EC12
	mov r5, #0x13
_0801EC12:
	mov r2, #1
	add r0, r2, #0
	ldrb r6, [r6, #1]
	and r0, r6
	cmp r0, #0
	beq _0801EC20
	mov r5, #5
_0801EC20:
	ldr r1, _0801EC44 @ =0x00001B12
	add r1, ip
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801EC30
	ldr r5, _0801EC48 @ =0x0000FFFF
_0801EC30:
	ldr r0, _0801EC48 @ =0x0000FFFF
	cmp r5, r0
	bne _0801EC4C
	bl sub_08077BCC
	b _0801EC52
_0801EC3C: .4byte 0x00004870
_0801EC40: .4byte 0x0000487C
_0801EC44: .4byte 0x00001B12
_0801EC48: .4byte 0x0000FFFF
_0801EC4C:
	add r0, r5, #0
	bl sub_08077B24
_0801EC52:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0801EBA8

