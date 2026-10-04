	thumb_func_start DuelCmd_SetZoneLevelCheckFlag
DuelCmd_SetZoneLevelCheckFlag: @ 0x08013104
	push {r4, lr}
	ldr r2, _08013144 @ =0x020185C0
	ldrh r0, [r2]
	lsr r1, r0, #0xF
	mov r0, #0x94
	ldrh r4, [r2, #2]
	add r3, r4, #0
	mul r3, r0
	ldr r0, _08013148 @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0801314C @ =0x0201930C
	add r3, r3, r0
	mov r1, #1
	ldrb r0, [r2, #4]
	and r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r4, [r3, #8]
	and r0, r4
	orr r0, r1
	strb r0, [r3, #8]
	ldr r0, _08013150 @ =0x0000080D
	add r2, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
_08013144: .4byte 0x020185C0
_08013148: .4byte 0x00000D64
_0801314C: .4byte 0x0201930C
_08013150: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetZoneLevelCheckFlag

