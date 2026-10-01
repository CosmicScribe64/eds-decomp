	thumb_func_start sub_08034414
sub_08034414: @ 0x08034414
	push {lr}
	add r3, r0, #0
	mov r1, #0
	mov r0, #4
	ldrb r2, [r3, #4]
	and r0, r2
	cmp r0, #0
	bne _08034474
	ldr r0, _08034444 @ =0x000007FF
	ldrh r2, [r3]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _08034448 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _0803444C @ =0x000003F2
	cmp r2, r0
	beq _08034464
	cmp r2, r0
	bgt _08034454
	ldr r0, _08034450 @ =0x0000021B
	cmp r2, r0
	beq _0803445A
	b _08034466
_08034444: .4byte 0x000007FF
_08034448: .4byte gUnk_08622AB4
_0803444C: .4byte 0x000003F2
_08034450: .4byte 0x0000021B
_08034454:
	ldr r0, _08034460 @ =0x000005A7
	cmp r2, r0
	bne _08034466
_0803445A:
	mov r1, #1
	b _0803446A
	.align 2, 0
_08034460: .4byte 0x000005A7
_08034464:
	mov r1, #2
_08034466:
	cmp r1, #0
	ble _08034474
_0803446A:
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl sub_080199E0
_08034474:
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end sub_08034414
	.align 2, 0

