	thumb_func_start sub_0804A92C
sub_0804A92C: @ 0x0804A92C
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r2, _0804A95C @ =0x020192E0
	ldr r1, _0804A960 @ =0x00001B10
	add r0, r2, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0804A958
	add r2, #4
	mov r0, #1
	and r0, r5
	ldr r1, _0804A964 @ =0x00000D64
	mul r0, r1
	add r2, r0, r2
	ldrb r1, [r2, #9]
	lsl r0, r1, #0x1B
	cmp r0, #0
	bge _0804A968
	ldrb r2, [r2, #8]
	lsl r0, r2, #0x19
	cmp r0, #0
	blt _0804A968
_0804A958:
	mov r0, #0
	b _0804A988
_0804A95C: .4byte 0x020192E0
_0804A960: .4byte 0x00001B10
_0804A964: .4byte 0x00000D64
_0804A968:
	ldr r4, _0804A990 @ =0x020192E4
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #1
	bl sub_0804A848
	mov r0, #1
	and r0, r5
	ldr r1, _0804A994 @ =0x00000D64
	mul r1, r0
	add r1, r1, r4
	ldrh r2, [r1, #0x24]
	neg r0, r2
	orr r0, r2
	lsr r0, r0, #0x1F
_0804A988:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0804A990: .4byte 0x020192E4
_0804A994: .4byte 0x00000D64
	thumb_func_end sub_0804A92C

