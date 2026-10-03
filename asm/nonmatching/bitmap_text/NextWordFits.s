	thumb_func_start NextWordFits
NextWordFits: @ 0x08079F40
	push {r4, r5, lr}
	add r3, r0, #0
	lsl r1, r1, #0x18
	lsr r5, r1, #0x18
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	mov r2, #0
	ldr r1, _08079F78 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08079FBC
	mov r1, #1
	ldrb r0, [r3]
	cmp r0, #0
	beq _08079FCA
_08079F62:
	cmp r0, #0x7E
	bhi _08079F8A
	ldrb r0, [r3]
	cmp r0, #0x24
	bne _08079F80
	ldrb r0, [r3, #1]
	cmp r0, #0x72
	bne _08079F7C
	add r3, #3
	b _08079F94
	.align 2, 0
_08079F78: .4byte 0x02011C20
_08079F7C:
	add r3, #2
	b _08079F94
_08079F80:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	add r3, #1
	b _08079F94
_08079F8A:
	add r0, r2, #2
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	add r3, #2
	mov r1, #0
_08079F94:
	ldrb r0, [r3]
	cmp r0, #0
	beq _08079FCA
	cmp r1, #0
	bne _08079F62
	b _08079FCA
_08079FA0:
	ldrb r0, [r3]
	cmp r0, #0x24
	bne _08079FB4
	ldrb r0, [r3, #1]
	cmp r0, #0x72
	bne _08079FB0
	add r3, #3
	b _08079FBC
_08079FB0:
	add r3, #2
	b _08079FBC
_08079FB4:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	add r3, #1
_08079FBC:
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08079FCA
	cmp r0, #0
	beq _08079FCA
	cmp r0, #0x2E
	bne _08079FA0
_08079FCA:
	add r0, r5, r2
	cmp r0, r4
	bgt _08079FD4
	mov r0, #1
	b _08079FD6
_08079FD4:
	mov r0, #0
_08079FD6:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end NextWordFits

