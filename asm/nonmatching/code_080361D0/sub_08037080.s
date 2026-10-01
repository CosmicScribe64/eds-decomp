	thumb_func_start sub_08037080
sub_08037080: @ 0x08037080
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _080370AC
	mov r0, #1
	ldrb r2, [r1, #2]
	and r0, r2
	mov r2, #0x92
	cmp r0, #0
	beq _0803709C
	ldr r2, _080370B4 @ =0x00008092
_0803709C:
	ldrh r1, [r1, #2]
	lsl r1, r1, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_080370AC:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_080370B4: .4byte 0x00008092
	thumb_func_end sub_08037080

