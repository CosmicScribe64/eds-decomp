	thumb_func_start UpdateSaveChecksum
UpdateSaveChecksum: @ 0x08077080
	push {r4, r5, r6, lr}
	mov r1, #0
	ldr r2, _080770B0 @ =0x02011C20
	mov r3, #0
	add r5, r2, #0
	ldr r4, _080770B4 @ =0x000010B5
_0807708C:
	ldrh r6, [r2]
	add r0, r6, r1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, r4
	bls _0807708C
	mvn r0, r1
	add r0, #1
	ldr r2, _080770B8 @ =0x0000216E
	add r1, r5, r2
	strh r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_080770B0: .4byte 0x02011C20
_080770B4: .4byte 0x000010B5
_080770B8: .4byte 0x0000216E
	thumb_func_end UpdateSaveChecksum

