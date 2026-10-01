	thumb_func_start sub_0801ABA0
sub_0801ABA0: @ 0x0801ABA0
	push {lr}
	mov r0, #8
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801ABB2
	mov r0, #0
	b _0801ABC4
_0801ABB2:
	ldr r1, _0801ABC8 @ =0x02015EE8
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1, #1]
	and r0, r2
	strb r0, [r1, #1]
	bl sub_0807255C
	mov r0, #1
_0801ABC4:
	pop {r1}
	bx r1
_0801ABC8: .4byte 0x02015EE8
	thumb_func_end sub_0801ABA0

