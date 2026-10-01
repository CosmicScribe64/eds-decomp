	thumb_func_start sub_0802226C
sub_0802226C: @ 0x0802226C
	push {r4, r5, r6, lr}
	add r6, r1, #0
	ldr r4, _08022284 @ =0x020192E0
	ldr r0, _08022288 @ =0x00001B62
	add r5, r4, r0
	ldrb r1, [r5]
	cmp r1, #0
	beq _0802228C
	cmp r1, #1
	beq _080222B8
	mov r0, #1
	b _080222E4
_08022284: .4byte 0x020192E0
_08022288: .4byte 0x00001B62
_0802228C:
	ldr r2, _080222A8 @ =0x00001B64
	add r0, r4, r2
	strh r1, [r0]
	ldr r0, _080222AC @ =0x00000206
	ldr r1, _080222B0 @ =0x00000713
	ldr r3, _080222B4 @ =0x08081F28
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _080222E2
	.align 2, 0
_080222A8: .4byte 0x00001B64
_080222AC: .4byte 0x00000206
_080222B0: .4byte 0x00000713
_080222B4: .4byte gUnk_08081F28
_080222B8:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _080222E2
	ldr r0, _080222EC @ =0x0201CFB0
	ldr r1, _080222F0 @ =0x0000082C
	add r0, r0, r1
	ldr r1, [r0]
	cmp r1, r6
	beq _080222DC
	ldr r2, _080222F4 @ =0x00001B64
	add r0, r4, r2
	strh r1, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_080222DC:
	mov r0, #3
	bl sub_08077AEC
_080222E2:
	mov r0, #0
_080222E4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080222EC: .4byte 0x0201CFB0
_080222F0: .4byte 0x0000082C
_080222F4: .4byte 0x00001B64
	thumb_func_end sub_0802226C

