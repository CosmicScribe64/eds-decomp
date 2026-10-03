	thumb_func_start TextWordLength
TextWordLength: @ 0x08074AB4
	add r1, r0, #0
	mov r2, #0
	b _08074AFC
_08074ABA:
	ldrb r0, [r1]
	cmp r0, #0x40
	beq _08074AE2
	cmp r0, #0x40
	bgt _08074AD6
	cmp r0, #0x20
	beq _08074B02
	cmp r0, #0x20
	bgt _08074AF8
	cmp r0, #0
	beq _08074B02
	cmp r0, #0xA
	beq _08074B02
	b _08074AF8
_08074AD6:
	cmp r0, #0x5C
	bne _08074AF8
	ldrb r0, [r1, #1]
	cmp r0, #0x6E
	bne _08074AF8
	b _08074B02
_08074AE2:
	ldrb r0, [r1, #1]
	cmp r0, #0x30
	beq _08074AF4
	cmp r0, #0x30
	blt _08074AF8
	cmp r0, #0x33
	bgt _08074AF8
	cmp r0, #0x32
	blt _08074AF8
_08074AF4:
	add r1, #1
	sub r2, #1
_08074AF8:
	add r1, #1
	add r2, #1
_08074AFC:
	ldrb r0, [r1]
	cmp r0, #0
	bne _08074ABA
_08074B02:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	bx lr
	thumb_func_end TextWordLength

