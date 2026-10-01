	thumb_func_start sub_080087EC
sub_080087EC: @ 0x080087EC
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	mov r4, #0
	mov r2, #0
	ldr r1, _08008850 @ =0x0201930C
	mov ip, r1
	mov r1, #1
	and r1, r0
	ldr r0, _08008854 @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
_08008808:
	mov r0, #0x94
	mul r0, r2
	add r0, r0, r5
	mov r1, ip
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08008840
	ldrb r3, [r3, #6]
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _08008840
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r6
	bne _08008840
	ldr r0, _08008858 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0800885C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _08008840
	add r4, #1
_08008840:
	add r2, #1
	cmp r2, #4
	ble _08008808
	add r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08008850: .4byte 0x0201930C
_08008854: .4byte 0x00000D64
_08008858: .4byte 0x000007FF
_0800885C: .4byte gUnk_08622AB4
	thumb_func_end sub_080087EC

