	thumb_func_start sub_080623AC
sub_080623AC: @ 0x080623AC
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r2, #0
	ldr r3, _080623E0 @ =0x081A42A4
	add r0, r1, r5
	lsl r0, r0, #3
	lsl r2, r4, #7
	add r0, r0, r2
	add r0, r0, r3
	ldr r0, [r0]
	cmp r1, #0xB
	bne _080623DA
	ldr r2, _080623E4 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _080623E8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r2, [r0, #2]
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0806236C
_080623DA:
	pop {r4, r5}
	pop {r1}
	bx r1
_080623E0: .4byte gUnk_081A42A4
_080623E4: .4byte 0x020192E4
_080623E8: .4byte 0x00000D64
	thumb_func_end sub_080623AC

