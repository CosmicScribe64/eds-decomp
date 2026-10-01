	thumb_func_start sub_08073500
sub_08073500: @ 0x08073500
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	lsr r0, r0, #5
	ldr r2, _08073550 @ =0x0300045C
	add r4, r0, r2
	lsr r1, r1, #0xF
	add r4, r4, r1
	cmp r3, #0
	beq _0807354A
	ldr r0, _08073554 @ =0x081A7760
	mov ip, r0
_08073520:
	mov r2, #0
	add r5, r4, #0
	add r5, #0x40
	sub r3, #1
	cmp r2, r7
	bcs _08073540
	mov r6, ip
_0807352E:
	lsl r0, r2, #1
	add r0, r0, r4
	ldrh r1, [r6]
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, r7
	bcc _0807352E
_08073540:
	add r4, r5, #0
	lsl r0, r3, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0
	bne _08073520
_0807354A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08073550: .4byte 0x0300045C
_08073554: .4byte gUnk_081A7760
	thumb_func_end sub_08073500

