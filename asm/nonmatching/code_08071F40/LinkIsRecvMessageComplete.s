	thumb_func_start LinkIsRecvMessageComplete
LinkIsRecvMessageComplete: @ 0x08072238
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldr r2, _08072264 @ =0x030049D0
	lsl r0, r7, #1
	add r0, r0, r7
	lsl r0, r0, #2
	ldr r3, _08072268 @ =0x0000052C
	add r1, r2, r3
	add r0, r0, r1
	ldrb r6, [r0]
	add r6, #5
	lsl r0, r6, #0x18
	lsr r5, r0, #0x18
	mov r1, #0xB0
	lsl r1, r1, #8
	add r0, r1, #0
	orr r5, r0
	mov r4, #0
	mov r8, r2
	b _08072296
_08072264: .4byte 0x030049D0
_08072268: .4byte 0x0000052C
_0807226C:
	add r2, r7, r4
	mov r0, #0x3F
	and r2, r0
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #2
	ldr r0, _0807228C @ =0x0000052C
	add r0, r8
	add r1, r1, r0
	ldr r0, _08072290 @ =0x0000F0FF
	ldrh r1, [r1]
	and r0, r1
	cmp r0, r5
	bne _08072294
	mov r0, #1
	b _080722A6
_0807228C: .4byte 0x0000052C
_08072290: .4byte 0x0000F0FF
_08072294:
	add r4, #1
_08072296:
	add r0, r6, #0
	mov r1, #5
	bl __udivsi3
	add r0, #3
	cmp r4, r0
	bcc _0807226C
	mov r0, #0
_080722A6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end LinkIsRecvMessageComplete

