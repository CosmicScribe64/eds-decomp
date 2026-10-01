	thumb_func_start sub_08069DD8
sub_08069DD8: @ 0x08069DD8
	push {lr}
	sub sp, #0x20
	lsl r0, r0, #0x18
	ldr r1, _08069E18 @ =0x081A715C
	lsr r0, r0, #0x15
	add r0, r0, r1
	ldr r1, [r0]
	ldrb r2, [r0, #4]
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	mov r0, #0x80
	lsl r0, r0, #3
	str r0, [sp, #0x18]
	ldr r0, _08069E1C @ =0x0201DB20
	str r0, [sp, #0x1C]
	add r0, r1, #0
	mov r1, #1
	bl sub_08077EF4
	add sp, #0x20
	pop {r0}
	bx r0
	.align 2, 0
_08069E18: .4byte gUnk_081A715C
_08069E1C: .4byte 0x0201DB20
	thumb_func_end sub_08069DD8

