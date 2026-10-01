	thumb_func_start sub_080746F0
sub_080746F0: @ 0x080746F0
	push {lr}
	ldr r0, _08074708 @ =0x02011C20
	ldrb r0, [r0, #4]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	cmp r0, #4
	bhi _08074786
	lsl r0, r0, #2
	ldr r1, _0807470C @ =0x08074710
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08074708: .4byte 0x02011C20
_0807470C: .4byte 0x08074710
_08074710:
	.4byte _08074738
	.4byte _08074724
	.4byte _0807474C
	.4byte _08074760
	.4byte _08074778
_08074724:
	ldr r1, _08074730 @ =0x00000805
	mov r2, #0xFC
	lsl r2, r2, #2
	ldr r3, _08074734 @ =0x08087B58
	b _08074768
	.align 2, 0
_08074730: .4byte 0x00000805
_08074734: .4byte gUnk_08087B58
_08074738:
	ldr r1, _08074744 @ =0x00000805
	mov r2, #0xFC
	lsl r2, r2, #2
	ldr r3, _08074748 @ =0x08087B60
	b _08074768
	.align 2, 0
_08074744: .4byte 0x00000805
_08074748: .4byte gUnk_08087B60
_0807474C:
	ldr r1, _08074758 @ =0x00000805
	mov r2, #0xFC
	lsl r2, r2, #2
	ldr r3, _0807475C @ =0x08087B68
	b _08074768
	.align 2, 0
_08074758: .4byte 0x00000805
_0807475C: .4byte gUnk_08087B68
_08074760:
	ldr r1, _08074770 @ =0x00000805
	mov r2, #0xFC
	lsl r2, r2, #2
	ldr r3, _08074774 @ =0x08087B70
_08074768:
	mov r0, #0x21
	bl sub_08072BB4
	b _08074786
_08074770: .4byte 0x00000805
_08074774: .4byte gUnk_08087B70
_08074778:
	ldr r1, _0807478C @ =0x00000805
	mov r2, #0xFC
	lsl r2, r2, #2
	ldr r3, _08074790 @ =0x08087B78
	mov r0, #0x21
	bl sub_08072BB4
_08074786:
	pop {r0}
	bx r0
	.align 2, 0
_0807478C: .4byte 0x00000805
_08074790: .4byte gUnk_08087B78
	thumb_func_end sub_080746F0

