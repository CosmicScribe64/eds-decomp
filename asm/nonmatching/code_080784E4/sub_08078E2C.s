	thumb_func_start sub_08078E2C
sub_08078E2C: @ 0x08078E2C
	push {r4, r5, lr}
	add r2, r0, #0
	mov r5, #0
	mov r4, #0
	mov r3, #1
	ldr r1, [r2]
	b _08078E42
_08078E3A:
	add r1, #1
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_08078E42:
	ldrb r0, [r1]
	cmp r0, #0x29
	beq _08078E4C
	cmp r0, #0x2C
	bne _08078E3A
_08078E4C:
	add r0, r1, #1
	str r0, [r2]
	sub r1, #1
	mov r2, #0
	cmp r2, r4
	bcs _08078E78
_08078E58:
	ldrb r0, [r1]
	sub r0, #0x30
	mul r0, r3
	add r0, r5, r0
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #0x19
	lsr r3, r0, #0x18
	sub r1, #1
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, r4
	bcc _08078E58
_08078E78:
	add r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08078E2C

