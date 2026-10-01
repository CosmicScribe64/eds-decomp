	thumb_func_start sub_0801BE58
sub_0801BE58: @ 0x0801BE58
	push {lr}
	bl sub_0801F744
	ldr r1, _0801BE8C @ =0x02015EE8
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1, #1]
	and r0, r2
	strb r0, [r1, #1]
	bl sub_0800817C
	mov r0, #0
	mov r1, #8
	bl sub_08007E68
	mov r0, #0
	bl sub_08059258
	mov r0, #1
	mov r1, #8
	bl sub_08007E68
	mov r0, #1
	pop {r1}
	bx r1
	.align 2, 0
_0801BE8C: .4byte 0x02015EE8
	thumb_func_end sub_0801BE58

