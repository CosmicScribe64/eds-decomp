	thumb_func_start sub_0803B47C
sub_0803B47C: @ 0x0803B47C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B552
	ldr r4, _0803B51C @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0803B552
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0803B552
	mov r0, #7
	ldrb r2, [r6, #0xA]
	and r0, r2
	cmp r0, #2
	bne _0803B552
	mov r5, #0
	add r3, r6, #0
	add r3, #0xC
	ldrb r0, [r6, #2]
	lsl r4, r0, #0x1F
_0803B4B8:
	lsl r0, r5, #1
	add r0, r3, r0
	ldrb r2, [r0]
	ldrh r0, [r0]
	lsr r1, r0, #8
	lsr r0, r4, #0x1F
	cmp r2, r0
	bne _0803B552
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0803B520 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803B524 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803B552
	add r5, #1
	cmp r5, #1
	ble _0803B4B8
	add r4, r3, #0
	mov r5, #1
_0803B4EC:
	ldrb r0, [r4]
	ldrh r2, [r4]
	lsr r1, r2, #8
	bl sub_08017FF4
	add r4, #2
	sub r5, #1
	cmp r5, #0
	bge _0803B4EC
	ldr r0, _0803B528 @ =0x000007FF
	ldrh r1, [r6]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0803B52C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #0xB3
	lsl r0, r0, #3
	cmp r1, r0
	beq _0803B530
	add r0, #0xA
	cmp r1, r0
	beq _0803B544
	b _0803B552
_0803B51C: .4byte 0x0000058A
_0803B520: .4byte 0x00000D64
_0803B524: .4byte 0x0201930C
_0803B528: .4byte 0x000007FF
_0803B52C: .4byte gUnk_08622AB4
_0803B530:
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0x96
	lsl r1, r1, #3
	bl sub_08019860
	b _0803B552
_0803B544:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019980
_0803B552:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B47C
	.align 2, 0

