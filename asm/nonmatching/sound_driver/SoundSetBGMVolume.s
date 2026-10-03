	thumb_func_start SoundSetBGMVolume
SoundSetBGMVolume: @ 0x0807E724
	add r3, r0, #0
	ldr r2, _0807E740 @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r2, r1
	ldrh r1, [r0]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	bne _0807E744
	mov r0, #1
	neg r0, r0
	b _0807E75E
	.align 2, 0
_0807E740: .4byte 0x03005210
_0807E744:
	ldr r1, _0807E760 @ =0x00000193
	add r0, r2, r1
	strb r3, [r0]
	mov r0, #0xC9
	lsl r0, r0, #1
	add r1, r2, r0
	mov r0, #0x40
	strb r0, [r1]
	mov r1, #0xC7
	lsl r1, r1, #1
	add r0, r2, r1
	mov r1, #0
	ldsh r0, [r0, r1]
_0807E75E:
	bx lr
_0807E760: .4byte 0x00000193
	thumb_func_end SoundSetBGMVolume

