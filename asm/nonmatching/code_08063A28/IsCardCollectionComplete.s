	thumb_func_start IsCardCollectionComplete
IsCardCollectionComplete: @ 0x08063D4C
	push {r4, r5, r6, r7, lr}
	mov r5, #0
	mov r4, #0
	mov r3, #1
	ldr r0, _08063D98 @ =0x0862311E
	ldrh r6, [r0]
	ldr r7, _08063D9C @ =0x0000077F
	ldr r0, _08063DA0 @ =0x02011C20
	add r2, r0, #0
	add r2, #0xC
_08063D60:
	cmp r6, r7
	bhi _08063D82
	add r4, #1
	ldrh r1, [r2]
	lsl r0, r1, #0x16
	cmp r0, #0
	bne _08063D80
	ldrb r1, [r2, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08063D80
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08063D82
_08063D80:
	add r5, #1
_08063D82:
	add r2, #4
	add r3, #1
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r3, r0
	ble _08063D60
	cmp r5, r4
	beq _08063DA4
	mov r0, #0
	b _08063DA6
	.align 2, 0
_08063D98: .4byte gUnk_0862311E
_08063D9C: .4byte 0x0000077F
_08063DA0: .4byte 0x02011C20
_08063DA4:
	mov r0, #1
_08063DA6:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end IsCardCollectionComplete

