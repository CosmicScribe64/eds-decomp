	thumb_func_start TakeDeckCardByNumber
TakeDeckCardByNumber: @ 0x08007FEC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	mov ip, r2
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	mov r0, #0
	mov r8, r0
	ldr r4, _08008080 @ =0x020192E4
	mov r0, #1
	mov r1, sl
	and r0, r1
	ldr r1, _08008084 @ =0x00000D64
	mul r0, r1
	add r3, r0, r4
	ldrb r7, [r3, #3]
	cmp r8, r7
	bge _080080A2
	ldr r1, _08008088 @ =0x000007C4
	add r6, r4, r1
	add r1, r0, #0
	mov r9, r4
	add r4, r3, #0
	mov r5, #0
	add r3, r1, r6
_08008026:
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r7, _0800808C @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, r2
	bne _08008094
	add r1, r1, r6
	add r1, r1, r5
	mov r0, ip
	bl CopyDuelCard
	ldrb r0, [r4, #3]
	sub r0, #1
	strb r0, [r4, #3]
	mov r6, r8
	cmp r8, r0
	bge _0800807C
	mov r0, #1
	mov r1, sl
	and r0, r1
	ldr r1, _08008084 @ =0x00000D64
	mul r0, r1
	ldr r1, _08008090 @ =0x02019AA8
	mov r3, r9
	add r2, r0, r3
	add r5, #4
	add r7, r0, r1
	lsl r0, r6, #2
	add r4, r0, r7
_08008064:
	add r1, r7, r5
	add r0, r4, #0
	str r2, [sp, #0]
	bl CopyDuelCard
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r2, [sp, #0]
	ldrb r0, [r2, #3]
	cmp r6, r0
	blt _08008064
_0800807C:
	mov r0, #1
	b _080080A4
_08008080: .4byte 0x020192E4
_08008084: .4byte 0x00000D64
_08008088: .4byte 0x000007C4
_0800808C: .4byte gCardIdToNumber
_08008090: .4byte 0x02019AA8
_08008094:
	add r5, #4
	add r3, #4
	mov r7, #1
	add r8, r7
	ldrb r0, [r4, #3]
	cmp r8, r0
	blt _08008026
_080080A2:
	mov r0, #0
_080080A4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end TakeDeckCardByNumber

