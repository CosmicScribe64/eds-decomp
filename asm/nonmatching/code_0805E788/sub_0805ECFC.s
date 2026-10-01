	thumb_func_start sub_0805ECFC
sub_0805ECFC: @ 0x0805ECFC
	push {r4, r5, r6, lr}
	ldr r0, _0805ED50 @ =0x0201CFB0
	ldr r2, _0805ED54 @ =0x00000824
	add r1, r0, r2
	ldr r2, [r1]
	ldr r3, _0805ED58 @ =0x00000828
	add r1, r0, r3
	ldr r3, [r1]
	ldr r1, _0805ED5C @ =0x0000082C
	add r0, r0, r1
	ldr r5, [r0]
	mov r0, #1
	and r2, r0
	ldr r0, _0805ED60 @ =0x00000D64
	add r4, r2, #0
	mul r4, r0
	ldr r6, _0805ED64 @ =0x0201930C
	add r2, r4, r6
	add r1, r3, r5
	mov r0, #0x94
	mul r0, r1
	add r0, r2, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r3, #5
	beq _0805ED72
	cmp r3, #5
	ble _0805ED6C
	cmp r3, #0xA
	beq _0805ED72
	cmp r3, #0xB
	bne _0805ED70
	ldr r2, _0805ED68 @ =0x0000065C
	add r0, r6, r2
	add r0, r4, r0
	lsl r1, r5, #2
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	b _0805ED72
_0805ED50: .4byte 0x0201CFB0
_0805ED54: .4byte 0x00000824
_0805ED58: .4byte 0x00000828
_0805ED5C: .4byte 0x0000082C
_0805ED60: .4byte 0x00000D64
_0805ED64: .4byte 0x0201930C
_0805ED68: .4byte 0x0000065C
_0805ED6C:
	cmp r3, #0
	beq _0805ED72
_0805ED70:
	mov r0, #0
_0805ED72:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0805ECFC

