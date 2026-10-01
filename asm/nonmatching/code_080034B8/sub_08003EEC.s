	thumb_func_start sub_08003EEC
sub_08003EEC: @ 0x08003EEC
	push {lr}
	ldr r0, _08003F04 @ =0x0201F814
	ldrb r0, [r0, #1]
	lsr r0, r0, #5
	cmp r0, #1
	beq _08003F18
	cmp r0, #1
	bgt _08003F08
	cmp r0, #0
	beq _08003F12
	b _08003F2E
	.align 2, 0
_08003F04: .4byte 0x0201F814
_08003F08:
	cmp r0, #2
	beq _08003F1E
	cmp r0, #3
	beq _08003F24
	b _08003F2E
_08003F12:
	bl sub_08063BAC
	b _08003F28
_08003F18:
	bl sub_08063C14
	b _08003F28
_08003F1E:
	bl sub_08063C7C
	b _08003F28
_08003F24:
	bl sub_08063CE4
_08003F28:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08003F30
_08003F2E:
	mov r0, #0
_08003F30:
	pop {r1}
	bx r1
	thumb_func_end sub_08003EEC

