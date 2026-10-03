	thumb_func_start SoundVBlank
SoundVBlank: @ 0x0807E3B0
	push {lr}
	ldr r2, _0807E3D4 @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r2, r1
	ldrh r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #6
	and r0, r1
	cmp r0, #0
	bne _0807E3D0
	add r0, r2, #0
	bl SoundSequencerTick
	bl __sub_0807EAD0_from_thumb
_0807E3D0:
	pop {r0}
	bx r0
_0807E3D4: .4byte 0x03005210
	thumb_func_end SoundVBlank

