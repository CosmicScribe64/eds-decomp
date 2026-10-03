	thumb_func_start PackList_FlushVram
PackList_FlushVram: @ 0x08064698
	push {r4, r5, r6, lr}
	mov r4, #0
	ldr r6, _08064714 @ =0x02020310
	ldr r2, _08064718 @ =0x040000D4
	ldr r5, _0806471C @ =0x06004000
	add r3, r6, #0
	add r3, #0x6E
_080646A6:
	str r3, [r2]
	str r5, [r2, #4]
	ldr r0, _08064720 @ =0x80000800
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r0, [r2, #8]
	mov r1, #0x80
	lsl r1, r1, #0x18
	cmp r0, #0
	bge _080646C2
_080646BA:
	ldr r0, [r2, #8]
	and r0, r1
	cmp r0, #0
	bne _080646BA
_080646C2:
	mov r0, #0x80
	lsl r0, r0, #5
	add r5, r5, r0
	add r3, r3, r0
	add r4, #1
	cmp r4, #7
	ble _080646A6
	mov r4, #0
	ldr r3, _08064718 @ =0x040000D4
	ldr r5, _08064724 @ =0x0300045C
_080646D6:
	lsl r1, r4, #0xB
	add r0, r1, r5
	str r0, [r3]
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r1, r1, r0
	str r1, [r3, #4]
	ldr r0, _08064728 @ =0x80000400
	str r0, [r3, #8]
	ldr r0, [r3, #8]
	ldr r0, [r3, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	add r1, r4, #1
	cmp r0, #0
	bge _080646FE
_080646F6:
	ldr r0, [r3, #8]
	and r0, r2
	cmp r0, #0
	bne _080646F6
_080646FE:
	add r4, r1, #0
	cmp r4, #7
	ble _080646D6
	mov r0, #2
	neg r0, r0
	ldrb r1, [r6, #0x18]
	and r0, r1
	strb r0, [r6, #0x18]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08064714: .4byte 0x02020310
_08064718: .4byte 0x040000D4
_0806471C: .4byte 0x06004000
_08064720: .4byte 0x80000800
_08064724: .4byte 0x0300045C
_08064728: .4byte 0x80000400
	thumb_func_end PackList_FlushVram

