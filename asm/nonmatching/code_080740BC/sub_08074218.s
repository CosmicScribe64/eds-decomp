	thumb_func_start sub_08074218
sub_08074218: @ 0x08074218
	push {r4, r5, lr}
	mov r5, #0
	ldr r4, _08074254 @ =0x03005B60
	ldr r2, _08074258 @ =0x00000A3E
	add r1, r4, r2
	strh r5, [r1]
	add r2, #2
	add r1, r4, r2
	mov r2, #0xC
	bl CpuSet
	mov r1, #0
	ldr r0, _0807425C @ =0x00000A3C
	add r4, r4, r0
_08074234:
	ldrh r2, [r4]
	add r0, r2, r5
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r4, #2
	add r1, #1
	cmp r1, #9
	bls _08074234
	ldr r0, _08074254 @ =0x03005B60
	mvn r1, r5
	ldr r2, _08074258 @ =0x00000A3E
	add r0, r0, r2
	strh r1, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
_08074254: .4byte 0x03005B60
_08074258: .4byte 0x00000A3E
_0807425C: .4byte 0x00000A3C
	thumb_func_end sub_08074218

