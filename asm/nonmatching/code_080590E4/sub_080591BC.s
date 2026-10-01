	thumb_func_start sub_080591BC
sub_080591BC: @ 0x080591BC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r7, #0
	ldr r1, _0805921C @ =0x020192E4
	mov r2, #1
	and r2, r0
	ldr r3, _08059220 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r7, r0
	bge _08059242
	mov r8, r2
_080591DE:
	lsl r1, r7, #2
	mov r0, r8
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08059224 @ =0x02019AA8
	add r1, r1, r0
	ldr r4, [r1]
	lsl r4, r4, #0x14
	lsr r4, r4, #0x14
	ldr r0, _08059228 @ =0x000007FF
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805922C @ =0x08622AB4
	add r6, r0, r1
	ldrh r1, [r6]
	mov r0, r9
	bl sub_08059164
	add r5, r0, #0
	add r0, r4, #0
	bl sub_0807717C
	cmp r5, r0
	ble _08059230
	ldrh r1, [r6]
	mov r0, r9
	bl sub_080590E4
	b _08059232
_0805921C: .4byte 0x020192E4
_08059220: .4byte 0x00000D64
_08059224: .4byte 0x02019AA8
_08059228: .4byte 0x000007FF
_0805922C: .4byte gUnk_08622AB4
_08059230:
	add r7, #1
_08059232:
	ldr r1, _08059250 @ =0x020192E4
	ldr r3, _08059254 @ =0x00000D64
	mov r0, r8
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r7, r0
	blt _080591DE
_08059242:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08059250: .4byte 0x020192E4
_08059254: .4byte 0x00000D64
	thumb_func_end sub_080591BC

