	thumb_func_start sub_0803D3D0
sub_0803D3D0: @ 0x0803D3D0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	mov r3, #0
	ldr r4, _0803D430 @ =0x02017A40
	ldr r1, _0803D434 @ =0x00000502
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r1, r0, #0x1E
	cmp r3, r1
	bge _0803D454
	ldr r0, _0803D438 @ =0x000007FF
	mov ip, r0
	ldr r7, _0803D43C @ =0x08622AB4
	add r0, r2, #0
	mov r2, ip
	and r0, r2
	lsl r0, r0, #1
	add r0, r0, r7
	mov r8, r0
	ldr r0, _0803D440 @ =0x00000504
	add r4, r4, r0
	ldr r5, _0803D444 @ =0xFFFFF830
	add r6, r1, #0
_0803D406:
	ldrh r1, [r4]
	cmp r1, #0
	beq _0803D44C
	mov r0, ip
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r1, [r0]
	mov r2, r8
	ldrh r0, [r2]
	ldr r2, _0803D448 @ =0x000007CF
	cmp r0, r2
	ble _0803D422
	add r0, r0, r5
_0803D422:
	cmp r1, r2
	ble _0803D428
	add r1, r1, r5
_0803D428:
	cmp r1, r0
	bne _0803D44C
	mov r0, #1
	b _0803D456
_0803D430: .4byte 0x02017A40
_0803D434: .4byte 0x00000502
_0803D438: .4byte 0x000007FF
_0803D43C: .4byte gUnk_08622AB4
_0803D440: .4byte 0x00000504
_0803D444: .4byte 0xFFFFF830
_0803D448: .4byte 0x000007CF
_0803D44C:
	add r4, #2
	add r3, #1
	cmp r3, r6
	blt _0803D406
_0803D454:
	mov r0, #0
_0803D456:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803D3D0

