	thumb_func_start sub_0802F0F4
sub_0802F0F4: @ 0x0802F0F4
	push {r4, lr}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	beq _0802F13E
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x22
	bl sub_08008524
	cmp r0, #0
	bgt _0802F14C
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802F144 @ =0x000004BA
	bl sub_08008524
	cmp r0, #0
	bgt _0802F14C
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802F148 @ =0x000007F2
	bl sub_08008524
	cmp r0, #0
	bgt _0802F14C
_0802F13E:
	mov r0, #0
	b _0802F14E
	.align 2, 0
_0802F144: .4byte 0x000004BA
_0802F148: .4byte 0x000007F2
_0802F14C:
	mov r0, #1
_0802F14E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802F0F4

