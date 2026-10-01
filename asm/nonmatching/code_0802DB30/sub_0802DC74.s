	thumb_func_start sub_0802DC74
sub_0802DC74: @ 0x0802DC74
	mov r3, #0
	ldr r2, _0802DC94 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802DC98 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0802DC9C @ =0x000003E7
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802DC8E
	mov r3, #1
_0802DC8E:
	add r0, r3, #0
	bx lr
	.align 2, 0
_0802DC94: .4byte 0x020192E4
_0802DC98: .4byte 0x00000D64
_0802DC9C: .4byte 0x000003E7
	thumb_func_end sub_0802DC74

