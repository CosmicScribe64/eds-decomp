	thumb_func_start sub_080668DC
sub_080668DC: @ 0x080668DC
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08066914 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08066918 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x17
	bgt _080668FC
	cmp r0, #0x15
	bge _0806698A
_080668FC:
	ldr r0, _08066914 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _0806691C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08066920 @ =0x00000776
	cmp r1, r0
	bne _08066924
	mov r0, #3
	b _08066986
	.align 2, 0
_08066914: .4byte 0x000007FF
_08066918: .4byte gUnk_08621DE0
_0806691C: .4byte gUnk_08622AB4
_08066920: .4byte 0x00000776
_08066924:
	cmp r1, r0
	blt _08066934
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08066934
	mov r0, #1
	b _08066986
_08066934:
	ldr r0, _08066958 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0806695C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08066966
	cmp r0, #0x16
	bgt _08066960
	cmp r0, #0x15
	beq _0806696A
	b _08066972
	.align 2, 0
_08066958: .4byte 0x000007FF
_0806695C: .4byte gUnk_08621DE0
_08066960:
	cmp r0, #0x17
	beq _0806696E
	b _08066972
_08066966:
	mov r0, #7
	b _08066986
_0806696A:
	mov r0, #8
	b _08066986
_0806696E:
	mov r0, #9
	b _08066986
_08066972:
	ldr r0, _08066990 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08066994 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08066986:
	cmp r0, #2
	beq _08066998
_0806698A:
	mov r0, #0
	b _0806699A
	.align 2, 0
_08066990: .4byte 0x000007FF
_08066994: .4byte gUnk_08621DE0
_08066998:
	mov r0, #1
_0806699A:
	bx lr
	thumb_func_end sub_080668DC

