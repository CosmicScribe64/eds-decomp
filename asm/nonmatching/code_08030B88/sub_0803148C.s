	thumb_func_start sub_0803148C
sub_0803148C: @ 0x0803148C
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080314B4 @ =0x000003EB
	bl sub_08019554
	cmp r0, #0
	bne _080314AC
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080314B8 @ =0x0000040A
	bl sub_08019554
_080314AC:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_080314B4: .4byte 0x000003EB
_080314B8: .4byte 0x0000040A
	thumb_func_end sub_0803148C

