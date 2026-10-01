	thumb_func_start sub_080172A8
sub_080172A8: @ 0x080172A8
	push {r4, r5, lr}
	ldr r4, _080172C0 @ =0x020185C0
	ldr r0, _080172C4 @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _080172C8
	cmp r0, #1
	beq _080172F4
	b _0801730A
_080172C0: .4byte 0x020185C0
_080172C4: .4byte 0x0000080A
_080172C8:
	mov r0, #4
	mov r1, #0
	bl sub_0801E998
	ldr r1, _080172F0 @ =0x02017A30
	ldrh r0, [r4, #2]
	strh r0, [r1, #6]
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _0801730A
	.align 2, 0
_080172F0: .4byte 0x02017A30
_080172F4:
	bl sub_0801EAD8
	cmp r0, #0
	beq _0801730A
	ldr r2, _08017310 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801730A:
	pop {r4, r5}
	pop {r0}
	bx r0
_08017310: .4byte 0x0000080D
	thumb_func_end sub_080172A8

