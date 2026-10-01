	thumb_func_start sub_0800D398
sub_0800D398: @ 0x0800D398
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r0, _0800D428 @ =0x020185C0
	mov r4, #0
	mov r9, r0
	ldrh r0, [r0]
	lsr r1, r0, #0xF
	ldr r0, _0800D42C @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
	ldr r6, _0800D430 @ =0x0201930C
	add r7, r5, r6
	mov r1, #9
	neg r1, r1
	mov r8, r1
	mov r2, #3
	neg r2, r2
	mov ip, r2
_0800D3C0:
	mov r0, #0x94
	add r3, r4, #0
	mul r3, r0
	add r0, r7, r3
	add r2, r0, #0
	add r2, #0x8C
	ldrb r1, [r2]
	mov r0, #8
	and r0, r1
	cmp r0, #0
	beq _0800D3E0
	mov r0, r8
	and r0, r1
	mov r1, #0x10
	orr r0, r1
	strb r0, [r2]
_0800D3E0:
	mov r0, ip
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	add r0, r3, r5
	add r2, r0, r6
	ldr r0, [r2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0800D434 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0xA7
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _0800D408
	mov r0, #0x20
	ldrb r1, [r2, #7]
	orr r0, r1
	strb r0, [r2, #7]
_0800D408:
	add r4, #1
	cmp r4, #4
	ble _0800D3C0
	ldr r1, _0800D438 @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800D428: .4byte 0x020185C0
_0800D42C: .4byte 0x00000D64
_0800D430: .4byte 0x0201930C
_0800D434: .4byte gUnk_08622AB4
_0800D438: .4byte 0x0000080D
	thumb_func_end sub_0800D398

