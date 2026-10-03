	thumb_func_start CardTrading_LoadObjTiles
CardTrading_LoadObjTiles: @ 0x0807CC78
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	lsl r1, r1, #5
	ldr r0, _0807CCA8 @ =0x06010000
	add r6, r1, r0
	cmp r3, #0
	ble _0807CCA2
	lsl r5, r2, #5
	add r4, r3, #0
_0807CC8A:
	add r0, r6, #0
	add r1, r7, #0
	add r2, r5, #0
	bl MemCopy16
	mov r0, #0x80
	lsl r0, r0, #3
	add r6, r6, r0
	add r7, r7, r5
	sub r4, #1
	cmp r4, #0
	bne _0807CC8A
_0807CCA2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807CCA8: .4byte 0x06010000
	thumb_func_end CardTrading_LoadObjTiles

