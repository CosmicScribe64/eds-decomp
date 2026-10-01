	thumb_func_start sub_08001374
sub_08001374: @ 0x08001374
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080013A0
	ldr r1, _08001440 @ =0x08080AB6
	ldr r0, _08001444 @ =0x02013DE0
	ldr r2, _08001448 @ =0x000012E9
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #0x43
	mov r2, #4
	mov r3, #1
	bl sub_080002C0
_080013A0:
	ldr r0, _0800144C @ =0x02011C20
	mov sl, r0
	ldr r1, _08001450 @ =0x08080AE0
	mov r9, r1
	ldr r4, _08001444 @ =0x02013DE0
	ldr r2, _08001448 @ =0x000012E9
	add r2, r2, r4
	mov r8, r2
	ldr r0, _08001454 @ =0x000012EA
	add r4, r4, r0
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r1
	ldrb r2, [r2]
	add r0, r2, r0
	add r0, r9
	ldrb r0, [r0]
	lsl r0, r0, #2
	add r0, sl
	ldr r7, _08001458 @ =0x000020D0
	add r0, r0, r7
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	ldr r5, _0800145C @ =0x08080AD4
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	mov r6, #2
	str r6, [sp, #0]
	mov r3, #3
	bl sub_08000324
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r1
	mov r2, r8
	ldrb r2, [r2]
	add r0, r2, r0
	add r0, r9
	ldrb r0, [r0]
	lsl r0, r0, #2
	add r0, sl
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0xA
	lsr r0, r0, #0x15
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	str r6, [sp, #0]
	mov r3, #3
	bl sub_08000324
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r1
	mov r2, r8
	ldrb r2, [r2]
	add r0, r2, r0
	add r0, r9
	ldrb r0, [r0]
	lsl r0, r0, #2
	add r0, sl
	ldr r1, _08001460 @ =0x000020D2
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #6
	ldrh r1, [r5, #8]
	ldrh r2, [r5, #0xA]
	str r6, [sp, #0]
	mov r3, #3
	bl sub_08000324
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08001440: .4byte gUnk_08080AB6
_08001444: .4byte 0x02013DE0
_08001448: .4byte 0x000012E9
_0800144C: .4byte 0x02011C20
_08001450: .4byte gUnk_08080AE0
_08001454: .4byte 0x000012EA
_08001458: .4byte 0x000020D0
_0800145C: .4byte gUnk_08080AD4
_08001460: .4byte 0x000020D2
	thumb_func_end sub_08001374

