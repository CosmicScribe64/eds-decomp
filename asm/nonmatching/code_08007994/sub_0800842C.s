	thumb_func_start sub_0800842C
sub_0800842C: @ 0x0800842C
	push {r4, r5, r6, r7, lr}
	add r6, r2, #0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r2, #0
	ldr r1, _0800847C @ =0x0201930C
	mov ip, r1
	mov r1, #1
	and r1, r0
	ldr r0, _08008480 @ =0x00000D64
	add r4, r1, #0
	mul r4, r0
	ldr r7, _08008484 @ =0x000007FF
_08008446:
	mov r0, #0x94
	mul r0, r2
	add r0, r0, r4
	mov r3, ip
	add r1, r0, r3
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _0800848C
	cmp r2, r6
	beq _0800848C
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800848C
	and r3, r7
	lsl r0, r3, #1
	ldr r1, _08008488 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r5
	bne _0800848C
	add r0, r2, #0
	b _08008496
	.align 2, 0
_0800847C: .4byte 0x0201930C
_08008480: .4byte 0x00000D64
_08008484: .4byte 0x000007FF
_08008488: .4byte gUnk_08622AB4
_0800848C:
	add r2, #1
	cmp r2, #0xA
	ble _08008446
	mov r0, #1
	neg r0, r0
_08008496:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800842C

