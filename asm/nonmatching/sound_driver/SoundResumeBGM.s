	thumb_func_start SoundResumeBGM
SoundResumeBGM: @ 0x0807E7B8
	ldr r1, _0807E7DC @ =0x03005210
	mov ip, r1
	mov r3, #0xC4
	lsl r3, r3, #1
	add r3, ip
	ldrh r2, [r3]
	ldr r1, _0807E7E0 @ =0x0000FEFF
	and r1, r2
	strh r1, [r3]
	ldr r2, _0807E7E4 @ =0x00000193
	add r2, ip
	mov r1, #0x10
	strb r1, [r2]
	mov r1, #0xC9
	lsl r1, r1, #1
	add r1, ip
	strb r0, [r1]
	bx lr
_0807E7DC: .4byte 0x03005210
_0807E7E0: .4byte 0x0000FEFF
_0807E7E4: .4byte 0x00000193
	thumb_func_end SoundResumeBGM

