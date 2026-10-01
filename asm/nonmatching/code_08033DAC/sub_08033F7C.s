	thumb_func_start sub_08033F7C
sub_08033F7C: @ 0x08033F7C
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08033FA4
	mov r0, #1
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #0x48
	cmp r0, #0
	bne _08033F98
	ldr r1, _08033FAC @ =0x00008048
_08033F98:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08033FA4:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08033FAC: .4byte 0x00008048
	thumb_func_end sub_08033F7C

