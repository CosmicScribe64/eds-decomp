	thumb_func_start DeckEdit_LoadCardBoxTiles
DeckEdit_LoadCardBoxTiles: @ 0x08066164
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08066214 @ =0x08706F28
	add r1, r4, #0
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _08066218 @ =0x087070A8
	mov r2, #0xC0
	lsl r2, r2, #1
	add r1, r4, r2
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _0806621C @ =0x08707228
	mov r2, #0xC0
	lsl r2, r2, #2
	add r1, r4, r2
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _08066220 @ =0x087076A8
	mov r2, #0x90
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _08066224 @ =0x087073A8
	mov r2, #0xC0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _08066228 @ =0x08707528
	mov r2, #0xF0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0xC0
	bl CpuSet
	ldr r0, _0806622C @ =0x08707828
	mov r2, #0x90
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08066230 @ =0x087078A8
	mov r2, #0x98
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08066234 @ =0x08707928
	mov r1, #0xA0
	lsl r1, r1, #4
	add r5, r4, r1
	add r1, r5, #0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08066238 @ =0x08707AA8
	add r1, r5, #0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806623C @ =0x087079A8
	mov r2, #0xA8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08066240 @ =0x08707A28
	mov r1, #0xB0
	lsl r1, r1, #4
	add r4, r4, r1
	add r1, r4, #0
	mov r2, #0x40
	bl CpuSet
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08066214: .4byte gDeckEditCardBoxTiles0
_08066218: .4byte gDeckEditCardBoxTiles1
_0806621C: .4byte gDeckEditCardBoxTiles2
_08066220: .4byte gDeckEditCardBoxTiles3
_08066224: .4byte gDeckEditCardBoxTiles4
_08066228: .4byte gDeckEditCardBoxTiles5
_0806622C: .4byte gDeckEditSmallCardBoxTiles0
_08066230: .4byte gDeckEditSmallCardBoxTiles1
_08066234: .4byte gDeckEditSmallCardBoxTiles2
_08066238: .4byte gDeckEditSmallCardBoxTiles3
_0806623C: .4byte gDeckEditSmallCardBoxTiles4
_08066240: .4byte gDeckEditSmallCardBoxTiles5
	thumb_func_end DeckEdit_LoadCardBoxTiles

