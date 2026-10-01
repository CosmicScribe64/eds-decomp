	thumb_func_start sub_080327F4
sub_080327F4: @ 0x080327F4
	push {lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _08032828
	mov r1, #1
	add r0, r1, #0
	ldrb r2, [r2, #2]
	and r0, r2
	mov r2, #0x1C
	cmp r0, #0
	beq _08032812
	ldr r2, _08032830 @ =0x0000801C
_08032812:
	ldr r0, _08032834 @ =0x020192E0
	ldr r3, _08032838 @ =0x00001ACD
	add r0, r0, r3
	ldrb r0, [r0]
	lsr r0, r0, #5
	bic r1, r0
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08032828:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08032830: .4byte 0x0000801C
_08032834: .4byte 0x020192E0
_08032838: .4byte 0x00001ACD
	thumb_func_end sub_080327F4

