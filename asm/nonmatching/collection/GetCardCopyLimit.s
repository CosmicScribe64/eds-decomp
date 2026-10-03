	thumb_func_start GetCardCopyLimit
GetCardCopyLimit: @ 0x0807717C
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08077194 @ =0x08622AB4
	add r0, r0, r1
	ldrh r2, [r0]
	mov r1, #0
	ldr r0, _08077198 @ =0x081A78B4
_0807718A:
	ldrh r3, [r0]
	cmp r2, r3
	bne _0807719C
	ldrh r0, [r0, #2]
	b _080771A6
_08077194: .4byte gCardIdToNumber
_08077198: .4byte gCardCopyLimits
_0807719C:
	add r0, #4
	add r1, #1
	cmp r1, #0x2E
	bls _0807718A
	mov r0, #3
_080771A6:
	bx lr
	thumb_func_end GetCardCopyLimit

