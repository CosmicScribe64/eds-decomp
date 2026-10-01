	thumb_func_start sub_08075434
sub_08075434: @ 0x08075434
	push {r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r1, r2, #0
	b _08075468
_0807543E:
	cmp r0, #0x25
	bne _08075460
	ldrb r0, [r4, #1]
	cmp r0, #0x64
	bne _08075460
	mov r0, #0
	strb r0, [r5]
	add r4, #1
	add r0, r5, #0
	bl sub_08075370
	add r4, #1
	add r0, r5, #0
	add r1, r4, #0
	bl sub_080752E8
	b _0807546E
_08075460:
	ldrb r0, [r4]
	strb r0, [r5]
	add r4, #1
	add r5, #1
_08075468:
	ldrb r0, [r4]
	cmp r0, #0
	bne _0807543E
_0807546E:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08075434

