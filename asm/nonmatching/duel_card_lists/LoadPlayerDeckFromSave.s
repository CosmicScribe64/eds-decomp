	thumb_func_start LoadPlayerDeckFromSave
LoadPlayerDeckFromSave: @ 0x0800817C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r4, _0800824C @ =0x02019AA8
	mov r1, #0xA0
	lsl r1, r1, #1
	add r0, r4, #0
	bl MemClear16
	ldr r0, _08008250 @ =0xFFFFF83C
	add r1, r4, r0
	mov r0, #0
	strb r0, [r1, #3]
	mov r3, #0
	ldr r6, _08008254 @ =0x02011C20
	ldr r2, _08008258 @ =0x000020C8
	add r0, r6, r2
	ldrh r7, [r0]
	cmp r3, r7
	bge _080081E8
	ldr r2, _0800825C @ =0xFFFFF000
	mov r8, r2
	mov r7, #0x11
	neg r7, r7
	mov ip, r7
	add r5, r1, #0
	mov r9, r0
	add r2, r4, #0
	ldr r0, _08008260 @ =0x00002008
	add r4, r6, r0
	ldr r1, _08008264 @ =0x00000FFF
	add r6, r1, #0
_080081BE:
	ldrh r1, [r4]
	and r1, r6
	mov r0, r8
	ldrh r7, [r2]
	and r0, r7
	orr r0, r1
	strh r0, [r2]
	mov r0, ip
	ldrb r1, [r2, #1]
	and r0, r1
	strb r0, [r2, #1]
	ldrb r0, [r5, #3]
	add r0, #1
	strb r0, [r5, #3]
	add r2, #4
	add r4, #2
	add r3, #1
	mov r7, r9
	ldrh r7, [r7]
	cmp r3, r7
	blt _080081BE
_080081E8:
	ldr r1, _08008268 @ =0x020192E4
	mov r0, #0
	strb r0, [r1, #5]
	mov r3, #0
	ldr r4, _08008254 @ =0x02011C20
	ldr r2, _0800826C @ =0x000020CC
	add r0, r4, r2
	ldrh r7, [r0]
	cmp r3, r7
	bge _08008240
	ldr r2, _0800825C @ =0xFFFFF000
	mov r8, r2
	mov r7, #0x11
	neg r7, r7
	mov ip, r7
	add r5, r1, #0
	mov r9, r0
	ldr r0, _08008270 @ =0x00000A44
	add r2, r5, r0
	ldr r1, _08008274 @ =0x0000209E
	add r4, r4, r1
	ldr r7, _08008264 @ =0x00000FFF
	add r6, r7, #0
_08008216:
	ldrh r1, [r4]
	and r1, r6
	mov r0, r8
	ldrh r7, [r2]
	and r0, r7
	orr r0, r1
	strh r0, [r2]
	mov r0, ip
	ldrb r1, [r2, #1]
	and r0, r1
	strb r0, [r2, #1]
	ldrb r0, [r5, #5]
	add r0, #1
	strb r0, [r5, #5]
	add r2, #4
	add r4, #2
	add r3, #1
	mov r7, r9
	ldrh r7, [r7]
	cmp r3, r7
	blt _08008216
_08008240:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800824C: .4byte 0x02019AA8
_08008250: .4byte 0xFFFFF83C
_08008254: .4byte 0x02011C20
_08008258: .4byte 0x000020C8
_0800825C: .4byte 0xFFFFF000
_08008260: .4byte 0x00002008
_08008264: .4byte 0x00000FFF
_08008268: .4byte 0x020192E4
_0800826C: .4byte 0x000020CC
_08008270: .4byte 0x00000A44
_08008274: .4byte 0x0000209E
	thumb_func_end LoadPlayerDeckFromSave

