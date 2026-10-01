	thumb_func_start sub_080647A4
sub_080647A4: @ 0x080647A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov sl, r0
	mov r9, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	ldr r1, _08064884 @ =0x040000D4
	ldr r0, _08064888 @ =0x0863CC7C
	str r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _0806488C @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	ldr r2, _08064890 @ =0x0202037E
	cmp r0, #0
	bge _080647E0
_080647D8:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _080647D8
_080647E0:
	ldr r1, _08064884 @ =0x040000D4
	ldr r0, _08064894 @ =0x0863CE7C
	str r0, [r1]
	mov r3, r8
	lsl r0, r3, #6
	add r0, r0, r2
	str r0, [r1, #4]
	ldr r0, _08064898 @ =0x80000020
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	mov r5, r8
	add r5, #1
	mov r4, r8
	add r4, #2
	cmp r0, #0
	bge _0806480E
_08064806:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _08064806
_0806480E:
	ldr r1, _08064884 @ =0x040000D4
	ldr r0, _0806489C @ =0x0863CEBC
	str r0, [r1]
	lsl r0, r5, #6
	add r0, r0, r2
	str r0, [r1, #4]
	ldr r0, _08064898 @ =0x80000020
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	cmp r0, #0
	bge _08064832
_0806482A:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _0806482A
_08064832:
	ldr r1, _08064884 @ =0x040000D4
	ldr r0, _080648A0 @ =0x0863CEFC
	str r0, [r1]
	lsl r0, r4, #6
	add r0, r0, r2
	str r0, [r1, #4]
	ldr r0, _08064898 @ =0x80000020
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	cmp r0, #0
	bge _08064856
_0806484E:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _0806484E
_08064856:
	mov r4, #0
	ldr r7, _080648A4 @ =0x03001C5C
	str r5, [sp, #0]
	mov r5, #0x80
	lsl r5, r5, #9
	mov ip, r5
_08064862:
	lsl r0, r4, #0x10
	add r6, r4, #1
	str r6, [sp, #4]
	lsr r1, r0, #0xB
	mov r3, #0
	mov r2, #0x1F
_0806486E:
	cmp sl, r4
	bgt _080648A8
	cmp r4, r9
	bge _080648A8
	lsr r0, r3, #0x10
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	mov r5, r8
	strh r5, [r0]
	b _080648B6
_08064884: .4byte 0x040000D4
_08064888: .4byte gUnk_0863CC7C
_0806488C: .4byte 0x80000100
_08064890: .4byte 0x0202037E
_08064894: .4byte gUnk_0863CE7C
_08064898: .4byte 0x80000020
_0806489C: .4byte gUnk_0863CEBC
_080648A0: .4byte gUnk_0863CEFC
_080648A4: .4byte 0x03001C5C
_080648A8:
	lsr r0, r3, #0x10
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	mov r6, sp
	ldrh r6, [r6]
	strh r6, [r0]
_080648B6:
	add r3, ip
	sub r2, #1
	cmp r2, #0
	bge _0806486E
	ldr r4, [sp, #4]
	cmp r4, #0x13
	ble _08064862
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080647A4

