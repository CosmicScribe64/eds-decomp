	thumb_func_start TextDrawString
TextDrawString: @ 0x0807501C
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r1, _0807503C @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08075040
	add r0, r4, #0
	add r1, r5, #0
	bl TextDrawSjisString
	b _08075048
_0807503C: .4byte 0x02011C20
_08075040:
	add r0, r4, #0
	add r1, r5, #0
	bl TextDrawLatinString
_08075048:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end TextDrawString
	.align 2, 0

