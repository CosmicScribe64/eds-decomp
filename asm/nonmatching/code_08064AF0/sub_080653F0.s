	thumb_func_start sub_080653F0
sub_080653F0: @ 0x080653F0
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _08065618 @ =0x08706528
	add r1, r4, #0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806561C @ =0x08706DE8
	add r1, r4, #0
	add r1, #0x80
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065620 @ =0x08706B68
	mov r2, #0x80
	lsl r2, r2, #1
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065624 @ =0x08706668
	mov r2, #0xC0
	lsl r2, r2, #1
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065628 @ =0x087063E8
	mov r2, #0x80
	lsl r2, r2, #2
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806562C @ =0x087068E8
	mov r2, #0xA0
	lsl r2, r2, #2
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065630 @ =0x08706A28
	mov r2, #0xC0
	lsl r2, r2, #2
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065634 @ =0x08706CA8
	mov r2, #0xE0
	lsl r2, r2, #2
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065638 @ =0x087067A8
	mov r2, #0x80
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806563C @ =0x08705808
	mov r2, #0x90
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065640 @ =0x08706348
	mov r2, #0xA0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065644 @ =0x08705768
	mov r2, #0xB0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065648 @ =0x087059E8
	mov r2, #0xC0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806564C @ =0x08705B28
	mov r2, #0xD0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065650 @ =0x087058A8
	mov r2, #0xE0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065654 @ =0x08705D08
	mov r2, #0xF0
	lsl r2, r2, #3
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065658 @ =0x08706028
	mov r2, #0x80
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806565C @ =0x08705E48
	mov r2, #0x88
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065660 @ =0x08705DA8
	mov r2, #0x90
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065664 @ =0x08705C68
	mov r2, #0x98
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065668 @ =0x08705A88
	mov r2, #0xA0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806566C @ =0x08706168
	mov r2, #0xA8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065670 @ =0x08705F88
	mov r2, #0xB0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065674 @ =0x087060C8
	mov r2, #0xB8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065678 @ =0x087062A8
	mov r2, #0xC0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806567C @ =0x08706208
	mov r2, #0xC8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065680 @ =0x08705EE8
	mov r2, #0xD0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065684 @ =0x08705BC8
	mov r2, #0xD8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065688 @ =0x08705948
	mov r2, #0xE0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806568C @ =0x08705048
	mov r2, #0xE8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065690 @ =0x087052C8
	mov r2, #0xF0
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065694 @ =0x08705628
	mov r2, #0xF8
	lsl r2, r2, #4
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08065698 @ =0x08705188
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _0806569C @ =0x087056C8
	mov r2, #0x84
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _080656A0 @ =0x08705408
	mov r2, #0x88
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _080656A4 @ =0x08704DE8
	mov r2, #0x8C
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _080656A8 @ =0x08704FA8
	mov r2, #0x90
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _080656AC @ =0x08704D48
	mov r2, #0x94
	lsl r2, r2, #5
	add r1, r4, r2
	mov r2, #0x40
	bl CpuSet
	ldr r0, _080656B0 @ =0x08704E88
	mov r1, #0x98
	lsl r1, r1, #5
	add r4, r4, r1
	add r1, r4, #0
	mov r2, #0x30
	bl CpuSet
	pop {r4}
	pop {r0}
	bx r0
_08065618: .4byte gUnk_08706528
_0806561C: .4byte gUnk_08706DE8
_08065620: .4byte gUnk_08706B68
_08065624: .4byte gUnk_08706668
_08065628: .4byte gUnk_087063E8
_0806562C: .4byte gUnk_087068E8
_08065630: .4byte gUnk_08706A28
_08065634: .4byte gUnk_08706CA8
_08065638: .4byte gUnk_087067A8
_0806563C: .4byte gUnk_08705808
_08065640: .4byte gUnk_08706348
_08065644: .4byte gUnk_08705768
_08065648: .4byte gUnk_087059E8
_0806564C: .4byte gUnk_08705B28
_08065650: .4byte gUnk_087058A8
_08065654: .4byte gUnk_08705D08
_08065658: .4byte gUnk_08706028
_0806565C: .4byte gUnk_08705E48
_08065660: .4byte gUnk_08705DA8
_08065664: .4byte gUnk_08705C68
_08065668: .4byte gUnk_08705A88
_0806566C: .4byte gUnk_08706168
_08065670: .4byte gUnk_08705F88
_08065674: .4byte gUnk_087060C8
_08065678: .4byte gUnk_087062A8
_0806567C: .4byte gUnk_08706208
_08065680: .4byte gUnk_08705EE8
_08065684: .4byte gUnk_08705BC8
_08065688: .4byte gUnk_08705948
_0806568C: .4byte gUnk_08705048
_08065690: .4byte gUnk_087052C8
_08065694: .4byte gUnk_08705628
_08065698: .4byte gUnk_08705188
_0806569C: .4byte gUnk_087056C8
_080656A0: .4byte gUnk_08705408
_080656A4: .4byte gUnk_08704DE8
_080656A8: .4byte gUnk_08704FA8
_080656AC: .4byte gUnk_08704D48
_080656B0: .4byte gUnk_08704E88
	thumb_func_end sub_080653F0

