	thumb_func_start sub_08000944
sub_08000944: @ 0x08000944
	push {r4, r5, lr}
	add r2, r0, #0
	mov r4, #0
	mov r0, #2
	strb r0, [r2, #0x18]
	strb r4, [r2, #0x19]
	ldr r3, _0800097C @ =0x02013DE0
	ldr r5, _08000980 @ =0x0000137C
	add r1, r3, r5
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08000962
	mov r0, #1
	strb r0, [r2, #0x19]
_08000962:
	ldr r1, _08000984 @ =0x000009A6
	add r0, r3, r1
	strb r4, [r0]
	ldr r1, _08000988 @ =0x08087B90
	ldrb r0, [r1]
	strb r0, [r2, #0x1C]
	ldrb r0, [r1, #1]
	strb r0, [r2, #0x1D]
	mov r0, #7
	strh r0, [r2, #0x24]
	pop {r4, r5}
	pop {r0}
	bx r0
_0800097C: .4byte 0x02013DE0
_08000980: .4byte 0x0000137C
_08000984: .4byte 0x000009A6
_08000988: .4byte gUnk_08087B90
	thumb_func_end sub_08000944

