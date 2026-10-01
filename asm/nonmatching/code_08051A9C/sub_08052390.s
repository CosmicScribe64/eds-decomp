	thumb_func_start sub_08052390
sub_08052390: @ 0x08052390
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, _08052418 @ =0x0201AE60
	ldrh r0, [r5, #8]
	add r0, #1
	lsl r6, r0, #3
	add r0, r5, #0
	add r0, #0x21
	ldrb r0, [r0]
	ldrh r1, [r5, #0xE]
	sub r0, r0, r1
	lsl r0, r0, #3
	mov r8, r0
	add r7, r5, #0
	add r7, #0x23
	ldrb r0, [r7]
	cmp r0, #0
	bne _080523FC
	ldr r0, _0805241C @ =0x050003E0
	ldr r1, _08052420 @ =0x0822C300
	mov r2, #0x20
	bl sub_08075294
	mov r0, #8
	mov r1, #4
	bl sub_08074B08
	ldr r2, _08052424 @ =0x00000C0F
	ldr r4, _08052428 @ =0x0819D264
	ldrh r1, [r5, #0x14]
	lsl r0, r1, #2
	add r0, r0, r4
	ldr r3, [r0]
	mov r0, #4
	mov r1, #0x12
	bl sub_0807501C
	ldr r2, _0805242C @ =0x00000C01
	ldrh r5, [r5, #0x14]
	lsl r0, r5, #2
	add r0, r0, r4
	ldr r3, [r0]
	mov r0, #3
	mov r1, #0x11
	bl sub_0807501C
	ldr r0, _08052430 @ =0x06016C80
	mov r1, #0
	bl sub_08075114
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
_080523FC:
	mov r0, r8
	sub r0, #0x10
	lsl r0, r0, #0x10
	orr r6, r0
	ldr r1, _08052434 @ =0x000040C0
	ldr r2, _08052438 @ =0x0000F364
	add r0, r6, #0
	bl sub_080761F0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08052418: .4byte 0x0201AE60
_0805241C: .4byte 0x050003E0
_08052420: .4byte gUnk_0822C300
_08052424: .4byte 0x00000C0F
_08052428: .4byte gUnk_0819D264
_0805242C: .4byte 0x00000C01
_08052430: .4byte 0x06016C80
_08052434: .4byte 0x000040C0
_08052438: .4byte 0x0000F364
	thumb_func_end sub_08052390

