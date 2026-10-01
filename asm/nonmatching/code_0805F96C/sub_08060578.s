	thumb_func_start sub_08060578
sub_08060578: @ 0x08060578
	push {r4, r5, lr}
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	ldr r0, _08060698 @ =0x05000200
	ldr r1, _0806069C @ =0x0867793C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606A0 @ =0x05000220
	ldr r1, _080606A4 @ =0x0867795C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606A8 @ =0x050002A0
	ldr r1, _080606AC @ =0x0868467C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606B0 @ =0x05000240
	ldr r1, _080606B4 @ =0x0867FC3C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606B8 @ =0x05000260
	ldr r1, _080606BC @ =0x0868045C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606C0 @ =0x05000280
	ldr r1, _080606C4 @ =0x0868167C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606C8 @ =0x050002C0
	ldr r1, _080606CC @ =0x0868557C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080606D0 @ =0x06010000
	ldr r1, _080606D4 @ =0x0867797C
	mov r5, #0x80
	lsl r5, r5, #4
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _080606D8 @ =0x06010800
	ldr r1, _080606DC @ =0x0867817C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _080606E0 @ =0x06011000
	ldr r1, _080606E4 @ =0x0867897C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _080606E8 @ =0x06011800
	ldr r1, _080606EC @ =0x0867917C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _080606F0 @ =0x06012000
	ldr r1, _080606F4 @ =0x0867997C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _080606F8 @ =0x06012800
	ldr r1, _080606FC @ =0x0867A17C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _08060700 @ =0x06013000
	ldr r1, _08060704 @ =0x0867B17C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _08060708 @ =0x06013800
	ldr r1, _0806070C @ =0x0867A97C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _08060710 @ =0x06014000
	ldr r1, _08060714 @ =0x0868487C
	mov r4, #0xC0
	lsl r4, r4, #1
	add r2, r4, #0
	bl sub_080752B0
	ldr r0, _08060718 @ =0x06014180
	ldr r1, _0806071C @ =0x086849FC
	add r2, r4, #0
	bl sub_080752B0
	ldr r0, _08060720 @ =0x06014300
	ldr r1, _08060724 @ =0x08684B7C
	add r2, r4, #0
	bl sub_080752B0
	ldr r0, _08060728 @ =0x06014480
	ldr r1, _0806072C @ =0x0867FC5C
	add r2, r5, #0
	bl sub_080752B0
	ldr r0, _08060730 @ =0x06014C80
	ldr r1, _08060734 @ =0x0868047C
	mov r2, #0x80
	lsl r2, r2, #5
	bl sub_080752B0
	ldr r0, _08060738 @ =0x06015C80
	ldr r1, _0806073C @ =0x0868187C
	mov r2, #0xA0
	lsl r2, r2, #3
	bl sub_080752B0
	ldr r0, _08060740 @ =0x06016180
	ldr r1, _08060744 @ =0x08681E7C
	mov r2, #0x80
	lsl r2, r2, #2
	bl sub_080752B0
	ldr r0, _08060748 @ =0x06016380
	ldr r1, _0806074C @ =0x08681D7C
	mov r2, #0x80
	lsl r2, r2, #1
	bl sub_080752B0
	ldr r0, _08060750 @ =0x06016480
	ldr r1, _08060754 @ =0x0868559C
	add r2, r5, #0
	bl sub_080752B0
	ldr r1, _08060758 @ =0x0201CFB0
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08060698: .4byte 0x05000200
_0806069C: .4byte gUnk_0867793C
_080606A0: .4byte 0x05000220
_080606A4: .4byte gUnk_0867795C
_080606A8: .4byte 0x050002A0
_080606AC: .4byte gUnk_0868467C
_080606B0: .4byte 0x05000240
_080606B4: .4byte gUnk_0867FC3C
_080606B8: .4byte 0x05000260
_080606BC: .4byte gUnk_0868045C
_080606C0: .4byte 0x05000280
_080606C4: .4byte gUnk_0868167C
_080606C8: .4byte 0x050002C0
_080606CC: .4byte gUnk_0868557C
_080606D0: .4byte 0x06010000
_080606D4: .4byte gUnk_0867797C
_080606D8: .4byte 0x06010800
_080606DC: .4byte gUnk_0867817C
_080606E0: .4byte 0x06011000
_080606E4: .4byte gUnk_0867897C
_080606E8: .4byte 0x06011800
_080606EC: .4byte gUnk_0867917C
_080606F0: .4byte 0x06012000
_080606F4: .4byte gUnk_0867997C
_080606F8: .4byte 0x06012800
_080606FC: .4byte gUnk_0867A17C
_08060700: .4byte 0x06013000
_08060704: .4byte gUnk_0867B17C
_08060708: .4byte 0x06013800
_0806070C: .4byte gUnk_0867A97C
_08060710: .4byte 0x06014000
_08060714: .4byte gUnk_0868487C
_08060718: .4byte 0x06014180
_0806071C: .4byte gUnk_086849FC
_08060720: .4byte 0x06014300
_08060724: .4byte gUnk_08684B7C
_08060728: .4byte 0x06014480
_0806072C: .4byte gUnk_0867FC5C
_08060730: .4byte 0x06014C80
_08060734: .4byte gUnk_0868047C
_08060738: .4byte 0x06015C80
_0806073C: .4byte gUnk_0868187C
_08060740: .4byte 0x06016180
_08060744: .4byte gUnk_08681E7C
_08060748: .4byte 0x06016380
_0806074C: .4byte gUnk_08681D7C
_08060750: .4byte 0x06016480
_08060754: .4byte gUnk_0868559C
_08060758: .4byte 0x0201CFB0
	thumb_func_end sub_08060578

