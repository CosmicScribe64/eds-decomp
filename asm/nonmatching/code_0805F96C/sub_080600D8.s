	thumb_func_start sub_080600D8
sub_080600D8: @ 0x080600D8
	push {r4, r5, r6, lr}
	ldr r6, _080600E8 @ =0x0201AE60
	ldrh r5, [r6, #4]
	cmp r5, #0
	beq _080600EC
	cmp r5, #1
	beq _08060108
	b _08060158
_080600E8: .4byte 0x0201AE60
_080600EC:
	ldr r1, _08060104 @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08060158
	mov r0, #2
	bl sub_08077AEC
_080600FE:
	mov r0, #1
	b _0806015A
	.align 2, 0
_08060104: .4byte 0x03000040
_08060108:
	ldr r0, _08060128 @ =0x03000040
	ldrh r1, [r0, #6]
	add r4, r5, #0
	and r4, r1
	cmp r4, #0
	beq _08060136
	mov r0, #1
	bl sub_08077AEC
	ldrh r0, [r6, #0x12]
	cmp r0, #0
	beq _0806012C
	cmp r0, #1
	beq _08060130
	b _080600FE
	.align 2, 0
_08060128: .4byte 0x03000040
_0806012C:
	strh r5, [r6, #0x14]
	b _080600FE
_08060130:
	mov r0, #0
	strh r0, [r6, #0x14]
	b _080600FE
_08060136:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _08060148
	mov r0, #2
	bl sub_08077AEC
	strh r4, [r6, #0x14]
	b _080600FE
_08060148:
	mov r0, #0x30
	and r0, r1
	cmp r0, #0
	beq _08060158
	mov r0, #1
	ldrh r1, [r6, #0x12]
	sub r0, r0, r1
	strh r0, [r6, #0x12]
_08060158:
	mov r0, #0
_0806015A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_080600D8

