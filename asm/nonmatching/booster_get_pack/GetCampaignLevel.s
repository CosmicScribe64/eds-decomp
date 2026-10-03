	thumb_func_start GetCampaignLevel
GetCampaignLevel: @ 0x08063EA0
	push {r4, lr}
	mov r4, #1
	bl IsCampaignLevel2Unlocked
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063EB0
	mov r4, #2
_08063EB0:
	bl IsCampaignLevel3Unlocked
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063EBC
	mov r4, #3
_08063EBC:
	bl IsCampaignLevel4Unlocked
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063EC8
	mov r4, #4
_08063EC8:
	bl IsCampaignLevel5Unlocked
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063ED4
	mov r4, #5
_08063ED4:
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end GetCampaignLevel

