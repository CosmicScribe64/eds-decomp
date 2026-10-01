	thumb_func_start sub_080091F0
sub_080091F0: @ 0x080091F0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	mov r6, #0
	mov r4, #0
	mov r2, #1
	and r2, r1
	ldr r1, _08009270 @ =0x00000D64
	add r5, r2, #0
	mul r5, r1
	add r7, r0, r5
	ldr r0, _08009274 @ =0x000007FF
	mov r8, r0
_08009210:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r0, r1, #0
	add r0, #0x28
	add r2, r7, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _0800925E
	add r0, r1, r5
	ldr r1, _08009278 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800925E
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	bne _0800925E
	add r1, r2, #0
	add r1, #0x91
	mov r0, #0xC
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #4
	bne _0800925E
	mov r0, r8
	and r3, r0
	lsl r0, r3, #1
	ldr r1, _0800927C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, ip
	bne _0800925E
	add r6, #1
_0800925E:
	add r4, #1
	cmp r4, #0xA
	ble _08009210
	add r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08009270: .4byte 0x00000D64
_08009274: .4byte 0x000007FF
_08009278: .4byte 0x0201930C
_0800927C: .4byte gUnk_08622AB4
	thumb_func_end sub_080091F0

