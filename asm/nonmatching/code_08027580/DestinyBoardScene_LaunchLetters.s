	thumb_func_start DestinyBoardScene_LaunchLetters
DestinyBoardScene_LaunchLetters: @ 0x08027580
	push {r4, r5, r6, lr}
	ldr r1, _08027600 @ =0x02020310
	ldr r0, _08027604 @ =0x00000B08
	add r5, r1, r0
	ldrb r0, [r5]
	sub r0, #1
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r6, r1, #0
	cmp r0, #0xFF
	bne _080275D4
	ldr r1, _08027608 @ =0x00000B09
	add r4, r6, r1
	ldrb r0, [r4]
	cmp r0, #5
	bhi _080275D4
	ldr r0, _0802760C @ =0x08082404
	ldrb r1, [r4]
	add r2, r1, r0
	mov r1, #0
	ldsb r1, [r2, r1]
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _080275CA
	add r0, r1, #0
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r1, _08027610 @ =0x00000B0D
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r0, #0x2F
	bl PlaySE
_080275CA:
	mov r0, #0x14
	strb r0, [r5]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080275D4:
	ldr r1, _08027614 @ =0x00000B14
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x60
	bne _080275F8
	ldr r0, _08027618 @ =0x00000AAC
	add r3, r6, r0
	mov r0, #0
	mov r1, #0x60
	mov r2, #0
	bl FadeStart
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0802761C @ =0x0000FEFF
	and r0, r1
	strh r0, [r2]
_080275F8:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08027600: .4byte 0x02020310
_08027604: .4byte 0x00000B08
_08027608: .4byte 0x00000B09
_0802760C: .4byte gFinalLetterLaunchOrder
_08027610: .4byte 0x00000B0D
_08027614: .4byte 0x00000B14
_08027618: .4byte 0x00000AAC
_0802761C: .4byte 0x0000FEFF
	thumb_func_end DestinyBoardScene_LaunchLetters

