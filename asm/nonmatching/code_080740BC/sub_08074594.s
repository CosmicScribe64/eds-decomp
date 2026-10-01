	thumb_func_start sub_08074594
sub_08074594: @ 0x08074594
	push {lr}
	sub sp, #4
	mov r0, sp
	bl sub_08004914
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl sub_08004358
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl sub_080044E4
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl sub_080042D8
	ldr r1, _08074618 @ =0x00000807
	mov r2, #0xF0
	lsl r2, r2, #2
	ldr r3, _0807461C @ =0x08087B34
	mov r0, #0x33
	bl sub_08072BB4
	ldr r0, _08074620 @ =0x08070033
	ldr r1, _08074624 @ =0x000403C0
	ldr r2, [sp, #0]
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	mov r3, #1
	bl sub_08072C0C
	ldr r0, _08074628 @ =0x08070038
	ldr r1, _0807462C @ =0x000203C5
	ldr r2, [sp, #0]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x1C
	mov r3, #1
	bl sub_08072C0C
	ldr r0, _08074630 @ =0x0807003B
	ldr r1, _08074634 @ =0x000203C8
	ldr r2, [sp, #0]
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	mov r3, #1
	bl sub_08072C0C
	add sp, #4
	pop {r0}
	bx r0
_08074618: .4byte 0x00000807
_0807461C: .4byte gUnk_08087B34
_08074620: .4byte 0x08070033
_08074624: .4byte 0x000403C0
_08074628: .4byte 0x08070038
_0807462C: .4byte 0x000203C5
_08074630: .4byte 0x0807003B
_08074634: .4byte 0x000203C8
	thumb_func_end sub_08074594

