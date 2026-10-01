	thumb_func_start sub_0800646C
sub_0800646C: @ 0x0800646C
	push {r4, r5, lr}
	ldr r5, _080064A4 @ =0x02013D90
	mov r0, #1
	ldrb r1, [r5]
	and r0, r1
	mov r4, #0
	cmp r0, #0
	beq _0800647E
	mov r4, #0x48
_0800647E:
	add r0, r4, #0
	add r0, #0x3E
	mov r1, #0x8E
	lsl r1, r1, #0x10
	orr r0, r1
	mov r1, #0x80
	lsl r1, r1, #7
	ldr r2, _080064A8 @ =0x0000303C
	bl sub_080761F0
	add r0, r4, #0
	add r0, #0x44
	ldr r2, [r5, #0x30]
	mov r1, #0x8E
	bl sub_080063D0
	pop {r4, r5}
	pop {r0}
	bx r0
_080064A4: .4byte 0x02013D90
_080064A8: .4byte 0x0000303C
	thumb_func_end sub_0800646C

