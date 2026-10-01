	thumb_func_start sub_0802A09C
sub_0802A09C: @ 0x0802A09C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	mov r9, r1
	mov r0, #0x20
	mov r1, #9
	bl sub_08074B08
	mov r4, #0
	cmp r4, r9
	bge _0802A144
	ldr r0, _0802A118 @ =0x02019FA8
	mov sl, r0
	mov r1, #7
	mov r8, r1
_0802A0C0:
	ldr r0, [r5]
	lsl r1, r0, #0x14
	add r7, r0, #0
	cmp r1, #0
	beq _0802A136
	mov r6, #1
	ldr r1, _0802A11C @ =0x0201D810
	ldrb r3, [r1]
	mov r0, #0xE0
	and r0, r3
	cmp r0, #0x60
	bne _0802A0FC
	ldrh r1, [r1, #6]
	add r2, r1, r4
	lsl r2, r2, #1
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	add r1, r6, #0
	and r1, r0
	ldr r0, _0802A120 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	add r2, sl
	ldrb r0, [r2]
	cmp r0, #2
	bne _0802A0FC
	and r0, r3
	cmp r0, #0
	beq _0802A0FC
	mov r6, #0
_0802A0FC:
	cmp r6, #0
	beq _0802A128
	lsl r2, r7, #0x14
	lsr r2, r2, #0xE
	ldr r0, _0802A124 @ =0x0822C720
	add r2, r2, r0
	add r5, #4
	mov r0, #8
	mov r1, r8
	mov r3, #0xA
	bl sub_08029FC8
	b _0802A136
	.align 2, 0
_0802A118: .4byte 0x02019FA8
_0802A11C: .4byte 0x0201D810
_0802A120: .4byte 0x00000D64
_0802A124: .4byte gUnk_0822C720
_0802A128:
	mov r0, #8
	mov r1, r8
	ldr r2, _0802A174 @ =0x0808275C
	mov r3, #0xA
	bl sub_08029FC8
	add r5, #4
_0802A136:
	mov r1, #0x10
	add r8, r1
	add r4, #1
	cmp r4, #3
	bgt _0802A144
	cmp r4, r9
	blt _0802A0C0
_0802A144:
	mov r4, #0
	ldr r2, _0802A178 @ =0x0000011F
	ldr r3, _0802A17C @ =0x0201D810
	ldr r0, _0802A180 @ =0x03000040
	ldr r5, _0802A184 @ =0x0000049C
	add r1, r0, r5
_0802A150:
	add r0, r4, #0
	add r0, #0x10
	strh r0, [r1]
	add r1, #2
	add r4, #1
	cmp r4, r2
	ble _0802A150
	mov r0, #4
	ldrb r1, [r3]
	orr r0, r1
	strb r0, [r3]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0802A174: .4byte gUnk_0808275C
_0802A178: .4byte 0x0000011F
_0802A17C: .4byte 0x0201D810
_0802A180: .4byte 0x03000040
_0802A184: .4byte 0x0000049C
	thumb_func_end sub_0802A09C

