	thumb_func_start SoundCountActivePcm
SoundCountActivePcm: @ 0x0807E990
	push {r4, r5, lr}
	ldr r3, _0807E9B8 @ =0x030053AC
	mov r4, #0
	mov r5, #0x80
	mov r2, #5
_0807E99A:
	ldrb r1, [r3, #0xE]
	add r0, r5, #0
	and r0, r1
	cmp r0, #0
	beq _0807E9A6
	add r4, #1
_0807E9A6:
	add r3, #0x10
	sub r2, #1
	cmp r2, #0
	bge _0807E99A
	add r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0807E9B8: .4byte 0x030053AC
	thumb_func_end SoundCountActivePcm

