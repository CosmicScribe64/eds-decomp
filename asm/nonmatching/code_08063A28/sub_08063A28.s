	thumb_func_start sub_08063A28
sub_08063A28: @ 0x08063A28
	push {r4, r5, r6, r7, lr}
	bl sub_08063040
	mov r4, #0
	ldr r5, _08063A54 @ =0x02015160
	mov r0, #0x81
	lsl r0, r0, #1
	add r6, r5, r0
	ldr r7, _08063A58 @ =0x0000FFFF
_08063A3A:
	mov r1, #0x86
	lsl r1, r1, #1
	add r0, r5, r1
	add r0, r4, r0
	mov r1, #0x18
	strb r1, [r0]
	ldrh r1, [r6]
	add r2, r1, #0
	cmp r1, r7
	bne _08063A5C
	mov r0, #0
	b _08063A92
	.align 2, 0
_08063A54: .4byte 0x02015160
_08063A58: .4byte 0x0000FFFF
_08063A5C:
	ldr r0, _08063A74 @ =0x000007CF
	cmp r1, r0
	bhi _08063A80
	ldr r2, _08063A78 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08063A7C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08063A92
	.align 2, 0
_08063A74: .4byte 0x000007CF
_08063A78: .4byte 0x000007FF
_08063A7C: .4byte gUnk_08623DF4
_08063A80:
	ldr r1, _08063AAC @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08063AB0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08063AB4 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08063A92:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r4, #0
	bl sub_08062604
	add r6, #2
	add r4, #1
	cmp r4, #4
	ble _08063A3A
	mov r0, #1
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08063AAC: .4byte 0xFFFFF830
_08063AB0: .4byte 0x000007FF
_08063AB4: .4byte gUnk_08623DF4
	thumb_func_end sub_08063A28

