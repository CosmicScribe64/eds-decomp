	thumb_func_start GetFaceUpFieldMagicNumber
GetFaceUpFieldMagicNumber: @ 0x080094E4
	push {r4, r5, lr}
	mov r3, #0
	ldr r5, _08009518 @ =0x020198D4
	ldr r4, _0800951C @ =0x000007FF
_080094EC:
	mov r0, #1
	and r0, r3
	ldr r1, _08009520 @ =0x00000D64
	mul r1, r0
	add r1, r1, r5
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08009528
	cmp r2, #0
	beq _08009528
	and r2, r4
	lsl r0, r2, #1
	ldr r1, _08009524 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08009530
	.align 2, 0
_08009518: .4byte 0x020198D4
_0800951C: .4byte 0x000007FF
_08009520: .4byte 0x00000D64
_08009524: .4byte gCardIdToNumber
_08009528:
	add r3, #1
	cmp r3, #1
	ble _080094EC
	mov r0, #0
_08009530:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end GetFaceUpFieldMagicNumber
	.align 2, 0

