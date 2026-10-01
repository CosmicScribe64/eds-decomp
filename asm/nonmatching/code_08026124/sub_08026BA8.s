	thumb_func_start sub_08026BA8
sub_08026BA8: @ 0x08026BA8
	push {r4, lr}
	ldr r4, _08026BC4 @ =0x02020E28
	add r0, r4, #0
	bl sub_0807883C
	add r0, r4, #0
	add r0, #8
	ldrb r4, [r4, #8]
	cmp r4, #2
	beq _08026BC8
	bl sub_0807B0D0
	mov r0, #0
	b _08026BCA
_08026BC4: .4byte 0x02020E28
_08026BC8:
	mov r0, #1
_08026BCA:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08026BA8

