	thumb_func_start sub_08027620
sub_08027620: @ 0x08027620
	push {r4, r5, r6, r7, lr}
	ldr r2, _0802765C @ =0x02020310
	ldr r0, _08027660 @ =0x00000B08
	add r1, r2, r0
	mov r0, #0x14
	strb r0, [r1]
	mov r3, #0
	add r7, r2, #0
	add r6, r7, #0
	ldr r5, _08027664 @ =0x00000B0C
	mov r2, #0
	ldr r4, _08027668 @ =0x00000B0D
_08027638:
	lsl r0, r3, #2
	add r0, r0, r6
	add r1, r0, r5
	strb r2, [r1]
	add r0, r0, r4
	strb r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #4
	bls _08027638
	ldr r0, _0802766C @ =0x00000B09
	add r1, r7, r0
	mov r0, #0
	strb r0, [r1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0802765C: .4byte 0x02020310
_08027660: .4byte 0x00000B08
_08027664: .4byte 0x00000B0C
_08027668: .4byte 0x00000B0D
_0802766C: .4byte 0x00000B09
	thumb_func_end sub_08027620

