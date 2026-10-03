	thumb_func_start IsBelowCardCopyLimit
IsBelowCardCopyLimit: @ 0x080771A8
	push {r4, r5, r6, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	ldr r6, _08077208 @ =0x02011C20
	lsl r0, r4, #2
	add r0, r0, r6
	ldrb r2, [r0, #9]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1E
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1E
	add r1, r1, r0
	lsr r2, r2, #6
	add r5, r1, r2
	add r0, r4, #0
	bl GetCardCopyLimit
	add r3, r0, #0
	ldr r0, _0807720C @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08077210 @ =0x08622AB4
	add r0, r0, r1
	ldrh r2, [r0]
	ldr r1, _08077214 @ =0x0000040A
	cmp r2, r1
	bne _080771E2
	b _080772E8
_080771E2:
	cmp r2, r1
	bgt _0807725C
	cmp r2, #0x3D
	bne _080771EC
	b _0807736C
_080771EC:
	cmp r2, #0x3D
	bgt _08077226
	cmp r2, #0x22
	bne _080771F6
	b _080772F0
_080771F6:
	cmp r2, #0x22
	bgt _08077218
	cmp r2, #0
	bne _08077200
	b _0807737C
_08077200:
	cmp r2, #0xE
	bne _08077206
	b _0807738C
_08077206:
	b _08077456
_08077208: .4byte 0x02011C20
_0807720C: .4byte 0x000007FF
_08077210: .4byte gCardIdToNumber
_08077214: .4byte 0x0000040A
_08077218:
	cmp r2, #0x25
	bne _0807721E
	b _0807739C
_0807721E:
	cmp r2, #0x28
	bne _08077224
	b _080773AC
_08077224:
	b _08077456
_08077226:
	ldr r0, _08077240 @ =0x00000183
	cmp r2, r0
	bne _0807722E
	b _080773DC
_0807722E:
	cmp r2, r0
	bgt _08077244
	cmp r2, #0x3F
	bne _08077238
	b _080773BC
_08077238:
	cmp r2, #0x44
	bne _0807723E
	b _080773CC
_0807723E:
	b _08077456
_08077240: .4byte 0x00000183
_08077244:
	ldr r0, _08077254 @ =0x00000185
	cmp r2, r0
	bne _0807724C
	b _080773EC
_0807724C:
	ldr r0, _08077258 @ =0x000003EB
	cmp r2, r0
	beq _080772DC
	b _08077456
_08077254: .4byte 0x00000185
_08077258: .4byte 0x000003EB
_0807725C:
	ldr r0, _08077284 @ =0x000007F5
	cmp r2, r0
	bne _08077264
	b _0807740C
_08077264:
	cmp r2, r0
	bgt _080772A0
	sub r0, #0x25
	cmp r2, r0
	bne _08077270
	b _080773FC
_08077270:
	cmp r2, r0
	bgt _0807728C
	ldr r0, _08077288 @ =0x000004BA
	cmp r2, r0
	beq _0807731C
	add r0, #0x27
	cmp r2, r0
	bne _08077282
	b _08077374
_08077282:
	b _08077456
_08077284: .4byte 0x000007F5
_08077288: .4byte 0x000004BA
_0807728C:
	ldr r0, _0807729C @ =0x000007DE
	cmp r2, r0
	bne _08077294
	b _08077404
_08077294:
	add r0, #0x14
	cmp r2, r0
	beq _08077344
	b _08077456
_0807729C: .4byte 0x000007DE
_080772A0:
	ldr r0, _080772C0 @ =0x00000814
	cmp r2, r0
	bne _080772A8
	b _08077424
_080772A8:
	cmp r2, r0
	bgt _080772C4
	sub r0, #0x1C
	cmp r2, r0
	bne _080772B4
	b _08077414
_080772B4:
	add r0, #0x17
	cmp r2, r0
	bne _080772BC
	b _0807741C
_080772BC:
	b _08077456
	.align 2, 0
_080772C0: .4byte 0x00000814
_080772C4:
	ldr r0, _080772D8 @ =0x00000953
	cmp r2, r0
	bne _080772CC
	b _08077438
_080772CC:
	add r0, #2
	cmp r2, r0
	bne _080772D4
	b _08077440
_080772D4:
	b _08077456
	.align 2, 0
_080772D8: .4byte 0x00000953
_080772DC:
	lsl r0, r1, #1
	ldr r1, _080772E4 @ =0x08623DF4
	add r0, r0, r1
	b _08077442
_080772E4: .4byte gCardNumberToId
_080772E8:
	ldr r0, _080772EC @ =0x086245CA
	b _08077442
_080772EC: .4byte gUnk_086245CA
_080772F0:
	ldr r0, _08077314 @ =0x08624768
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r6
	ldrb r1, [r0, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	add r5, r5, r0
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1E
	add r5, r5, r1
	ldr r0, _08077318 @ =0x08623E38
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
	.align 2, 0
_08077314: .4byte gUnk_08624768
_08077318: .4byte gUnk_08623E38
_0807731C:
	ldr r0, _0807733C @ =0x08623E38
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r6
	ldrb r1, [r0, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	add r5, r5, r0
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1E
	add r5, r5, r1
	lsl r0, r2, #1
	ldr r1, _08077340 @ =0x08623DF4
	add r0, r0, r1
	b _08077442
	.align 2, 0
_0807733C: .4byte gUnk_08623E38
_08077340: .4byte gCardNumberToId
_08077344:
	ldr r0, _08077368 @ =0x08623E38
	ldrh r1, [r0]
	lsl r0, r1, #2
	add r0, r0, r6
	ldrb r2, [r0, #9]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	add r5, r5, r0
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1E
	add r5, r5, r2
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0xE
	add r1, r1, r6
	ldrb r1, [r1, #9]
	b _0807744A
	.align 2, 0
_08077368: .4byte gUnk_08623E38
_0807736C:
	ldr r0, _08077370 @ =0x086247B6
	b _08077442
_08077370: .4byte gUnk_086247B6
_08077374:
	ldr r0, _08077378 @ =0x08623E6E
	b _08077442
_08077378: .4byte gUnk_08623E6E
_0807737C:
	ldr r0, _08077388 @ =0x08623DF4
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_08077388: .4byte gCardNumberToId
_0807738C:
	ldr r0, _08077398 @ =0x08623E10
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _0807742A
_08077398: .4byte gUnk_08623E10
_0807739C:
	ldr r0, _080773A8 @ =0x08623E3E
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_080773A8: .4byte gUnk_08623E3E
_080773AC:
	ldr r0, _080773B8 @ =0x08623E44
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_080773B8: .4byte gUnk_08623E44
_080773BC:
	ldr r0, _080773C8 @ =0x08623E72
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_080773C8: .4byte gUnk_08623E72
_080773CC:
	ldr r0, _080773D8 @ =0x08623E7C
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _0807742A
_080773D8: .4byte gUnk_08623E7C
_080773DC:
	ldr r0, _080773E8 @ =0x086240FA
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_080773E8: .4byte gUnk_086240FA
_080773EC:
	ldr r0, _080773F8 @ =0x086240FE
	ldrh r0, [r0]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	b _08077446
_080773F8: .4byte gUnk_086240FE
_080773FC:
	ldr r0, _08077400 @ =0x08623DF4
	b _08077442
_08077400: .4byte gCardNumberToId
_08077404:
	ldr r0, _08077408 @ =0x08623E10
	b _08077426
_08077408: .4byte gUnk_08623E10
_0807740C:
	ldr r0, _08077410 @ =0x08623E3E
	b _08077442
_08077410: .4byte gUnk_08623E3E
_08077414:
	ldr r0, _08077418 @ =0x08623E44
	b _08077442
_08077418: .4byte gUnk_08623E44
_0807741C:
	ldr r0, _08077420 @ =0x08623E72
	b _08077442
_08077420: .4byte gUnk_08623E72
_08077424:
	ldr r0, _08077434 @ =0x08623E7C
_08077426:
	ldrh r0, [r0]
	lsl r0, r0, #2
_0807742A:
	add r0, r0, r6
	ldrb r0, [r0, #9]
	lsr r0, r0, #6
	add r5, r5, r0
	b _08077456
_08077434: .4byte gUnk_08623E7C
_08077438:
	ldr r0, _0807743C @ =0x086240FA
	b _08077442
_0807743C: .4byte gUnk_086240FA
_08077440:
	ldr r0, _08077464 @ =0x086240FE
_08077442:
	ldrh r0, [r0]
	lsl r0, r0, #2
_08077446:
	add r0, r0, r6
	ldrb r1, [r0, #9]
_0807744A:
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	add r5, r5, r0
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1E
	add r5, r5, r1
_08077456:
	mov r0, #0
	cmp r5, r3
	bge _0807745E
	mov r0, #1
_0807745E:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08077464: .4byte gUnk_086240FE
	thumb_func_end IsBelowCardCopyLimit

