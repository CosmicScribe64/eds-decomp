	thumb_func_start AiPickOpponentHandCard
AiPickOpponentHandCard: @ 0x080578F4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r0, #0
	mov ip, r0
	ldr r0, _08057938 @ =0x020192E4
	ldrb r1, [r0, #2]
	mov r8, r1
	ldr r1, _0805793C @ =0x00000684
	add r1, r1, r0
	mov r9, r1
	ldr r7, _08057940 @ =0x0819D316
_0805790E:
	mov r2, #0
	cmp r2, r8
	bge _08057954
	ldrh r4, [r7]
	ldr r0, _08057938 @ =0x020192E4
	ldrb r3, [r0, #2]
	mov r1, r9
	ldr r6, _08057944 @ =0x000007FF
	ldr r5, _08057948 @ =0x08622AB4
_08057920:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r4
	bne _0805794C
	add r0, r2, #0
	b _0805796C
	.align 2, 0
_08057938: .4byte 0x020192E4
_0805793C: .4byte 0x00000684
_08057940: .4byte gAiHandPickPriority
_08057944: .4byte 0x000007FF
_08057948: .4byte gCardIdToNumber
_0805794C:
	add r1, #4
	add r2, #1
	cmp r2, r3
	blt _08057920
_08057954:
	add r7, #2
	mov r0, #1
	add ip, r0
	mov r1, ip
	cmp r1, #0x19
	bls _0805790E
	bl Random
	ldr r1, _08057978 @ =0x020192E4
	ldrb r1, [r1, #2]
	bl __modsi3
_0805796C:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08057978: .4byte 0x020192E4
	thumb_func_end AiPickOpponentHandCard

