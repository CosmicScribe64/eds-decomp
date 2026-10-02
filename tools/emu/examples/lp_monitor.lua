-- Example mGBA 0.10 Lua script for `tools/emu.py lua`.
-- Logs both players' life points whenever they change, saves a screenshot at each change, and stops the
-- run (EMU_STOP) after the first change. OUTDIR is the run directory (set by the harness).
-- Try: tools/emu.py lua tools/emu/examples/lp_monitor.lua --state duel_main1 \
--        --input tools/emu/states/duel_turn2.txt
local LP = { 0x020192E4, 0x020192E4 + 0xD64 }   -- struct DuelPlayer.lifePoints, players 0 and 1 (include/duel.h)
local last = { emu:read16(LP[1]), emu:read16(LP[2]) }
local frames = 0
console:log(string.format("start: LP %d / %d, phase byte %d", last[1], last[2], emu:read8(0x02015EE8)))

callbacks:add("frame", function()
    frames = frames + 1
    for i = 1, 2 do
        local v = emu:read16(LP[i])
        if v ~= last[i] then
            console:log(string.format("frame %d: player %d LP %d -> %d (phase byte %d)", frames, i - 1, last[i], v,
                                      emu:read8(0x02015EE8)))
            emu:screenshot(string.format("%s/lp_change_f%05d.png", OUTDIR, frames))
            last[i] = v
            EMU_STOP = 1
        end
    end
end)
