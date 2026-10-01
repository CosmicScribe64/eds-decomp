	thumb_func_start sub_08006B80
sub_08006B80: @ 0x08006B80
	push {r4, r5, r6, lr}
	sub sp, #4
	ldr r4, _08006BE4 @ =0x02013D90
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r0, #3
	add r6, r1, r0
	ldrh r0, [r4, #2]
	bl sub_08005A70
	ldr r1, _08006BE8 @ =0x000007FF
	add r0, r1, #0
	ldrh r2, [r4, #2]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _08006BEC @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r2, _08006BF0 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _08006BB4
	b _08006CE0
_08006BB4:
	mov r2, #0x88
	lsl r2, r2, #3
	add r0, r2, #0
	add r5, r6, #0
	orr r5, r0
	ldrh r2, [r4, #2]
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08006BF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08006C08
	cmp r0, #0x16
	bgt _08006BF8
	cmp r0, #0x15
	beq _08006BFE
	b _08006C18
	.align 2, 0
_08006BE4: .4byte 0x02013D90
_08006BE8: .4byte 0x000007FF
_08006BEC: .4byte gUnk_08622AB4
_08006BF0: .4byte 0xFFFFF880
_08006BF4: .4byte gUnk_08621DE0
_08006BF8:
	cmp r0, #0x17
	beq _08006C10
	b _08006C18
_08006BFE:
	ldr r3, _08006C04 @ =0x08631558
	b _08006CD6
	.align 2, 0
_08006C04: .4byte gUnk_08631558
_08006C08:
	ldr r3, _08006C0C @ =0x0862EEC0
	b _08006CD6
_08006C0C: .4byte gUnk_0862EEC0
_08006C10:
	ldr r3, _08006C14 @ =0x08633BF0
	b _08006CD6
_08006C14: .4byte gUnk_08633BF0
_08006C18:
	ldr r0, _08006C30 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08006C34 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08006C38 @ =0x00000776
	cmp r1, r0
	bne _08006C3C
	mov r0, #3
	b _08006C9E
	.align 2, 0
_08006C30: .4byte 0x000007FF
_08006C34: .4byte gUnk_08622AB4
_08006C38: .4byte 0x00000776
_08006C3C:
	cmp r1, r0
	blt _08006C4C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08006C4C
	mov r0, #1
	b _08006C9E
_08006C4C:
	ldr r0, _08006C70 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08006C74 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08006C7E
	cmp r0, #0x16
	bgt _08006C78
	cmp r0, #0x15
	beq _08006C82
	b _08006C8A
	.align 2, 0
_08006C70: .4byte 0x000007FF
_08006C74: .4byte gUnk_08621DE0
_08006C78:
	cmp r0, #0x17
	beq _08006C86
	b _08006C8A
_08006C7E:
	mov r0, #7
	b _08006C9E
_08006C82:
	mov r0, #8
	b _08006C9E
_08006C86:
	mov r0, #9
	b _08006C9E
_08006C8A:
	ldr r0, _08006CAC @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r2, _08006CB0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08006C9E:
	cmp r0, #2
	beq _08006CC4
	cmp r0, #2
	bgt _08006CB4
	cmp r0, #1
	beq _08006CBA
	b _08006CD4
_08006CAC: .4byte 0x000007FF
_08006CB0: .4byte gUnk_08621DE0
_08006CB4:
	cmp r0, #3
	beq _08006CCC
	b _08006CD4
_08006CBA:
	ldr r3, _08006CC0 @ =0x08627AF8
	b _08006CD6
	.align 2, 0
_08006CC0: .4byte gUnk_08627AF8
_08006CC4:
	ldr r3, _08006CC8 @ =0x0862A190
	b _08006CD6
_08006CC8: .4byte gUnk_0862A190
_08006CCC:
	ldr r3, _08006CD0 @ =0x0862C828
	b _08006CD6
_08006CD0: .4byte gUnk_0862C828
_08006CD4:
	ldr r3, _08006D00 @ =0x08625460
_08006CD6:
	add r0, r5, #0
	mov r1, #0x20
	mov r2, #0x10
	bl sub_08072EB0
_08006CE0:
	add r1, r6, #0
	add r1, #0xC2
	ldr r0, _08006D04 @ =0x02013D90
	ldrh r2, [r0, #2]
	mov r3, #0x98
	lsl r3, r3, #1
	mov r0, #0x80
	str r0, [sp, #0]
	mov r0, #1
	bl sub_08072D28
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006D00: .4byte gUnk_08625460
_08006D04: .4byte 0x02013D90
	thumb_func_end sub_08006B80

