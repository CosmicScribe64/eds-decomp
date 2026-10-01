	thumb_func_start sub_080305EC
sub_080305EC: @ 0x080305EC
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08030614
	mov r0, #1
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #0x60
	cmp r0, #0
	beq _08030608
	ldr r1, _0803061C @ =0x00008060
_08030608:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08030614:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_0803061C: .4byte 0x00008060
	thumb_func_end sub_080305EC

