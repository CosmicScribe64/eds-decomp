---
title: How fast other GBA decomps went
type: question
status: draft
confidence: medium
sources: [web-research-2026-10-01]
updated: 2026-10-02
---
# How fast other GBA decomps went

Question (2026-10-01): other projects seem to get further in less time. How, and what should EDS change?

Data was pulled on 2026-10-01 from the decomp.dev JSON (`https://decomp.dev/projects?format=json` with `Accept: application/json`), its `?mode=history` pages, and the GitHub API. Start dates are first commits. Contributor counts come from the GitHub contributors API. Numbers for khcom, knidl and Crash XS were spot-checked by hand; the rest come from a research agent and are marked where unverified.

## GBA projects on decomp.dev

| Project | Start | People | Code % now | Days to 50% / ~100% | Notes |
|---|---|---|---|---|---|
| Kingdom Hearts: CoM (Pheenoh/khcom) | 2026-08-30 | 1 | 100 | 5 / 14 | 5,370 functions, about 2,680 commits, "seven interrupted agents" |
| Crash Bandicoot XS | 2026-09 (repo 2025-10) | 3 | 99.7 | 16 / 18 | Built on Sonic Advance 2 tooling; Claude Code is the primary method |
| Kirby: Nightmare in Dream Land | 2026-08-20 | 4 | 99.5 | 18 / 38 | 145 Claude co-authored commits |
| Beyblade VForce | restarted 2026-08-18 | 1 | 100 | ? / 37 | Repo from 2017; heavy Claude use |
| Klonoa (testyourmine/kleod) | 2026-05-18 | 2 | 100 | ? / 135 | Listed only at 98.5% |
| Fire Emblem 8 JP | 2026-06-05 | 6 | 100 | 4 / 36 | Ported from the US decomp |
| Klonoa (Dream-Atelier) | 2026-03-14 | 2 | 51.3 | 154 / - | |
| Sonic Battle | 2026-08-04 | 1 | 55.1 | 42 / - | |
| Frogger's Adventures (Konami) | 2026-05-25 | 2 | 42.5 | - | "Agentic project" |
| WarioWare Inc. | 2026-04-15 | 1 | 30.8 | - | "fake match galore" |
| Golden Sun | 2026-05 (2023 disassembly) | 5 | 24.4 | - | |
| Fire Emblem 8 US | 2018-02-16 | 23 | 99.8 | 5.25 yr / 7.8 yr | Pre-AI |
| **EDS (this project)** | 2026-09-29 | 1 | **60.9** | 50% on day 3 | 1,706/1,976 functions |

> [!note] Update 2026-10-02
> The EDS row above is the 2026-10-01 snapshot. EDS reached **100%** (1,976/1,976 functions, 100% of code bytes) on 2026-10-02 (commit `d77fcef`, [[overview]]). That was the fourth day of the project, which started on 2026-09-29 according to the first [[log]] entries; the public git history begins on 2026-10-01. That is about 4 days to ~100%, against 14 for khcom, the fastest project in the table.

Older human-only GBA projects took years: Metroid Zero Mission 4.7 years to 99.9%, Sonic Advance 3 at 84.5% after 3 years (it bans AI contributions).

## What the comparison shows

- **EDS's early pace is among the fastest recorded.** On day 3 khcom was at 22.9%; EDS was at 60.7%.
- **The fastest projects finished in 2 to 5 weeks.** The difference is in the tail, not the start.
- **Many "new" projects are older than they look.** Ports of a sister decomp (FE8 JP, FE7 US), reused tooling (Crash XS), old repos restarted (Beyblade), forks (Wario Land 4), or projects listed on decomp.dev only near 100% (kleod at 98.5%).
- **Slow projects are less visible.** Klonoa (Dream-Atelier) took 5 months to reach 51%; Frogger, Golden Sun and WarioWare are below 45%.
- **Fake matches are normal in the fast runs.**
- **Throughput.** khcom averaged about 84 commits a day with several agents in parallel. EDS runs on a Pro plan and spent this week's quota in 3 days, so model budget is a real constraint (inference from usage, not from the other projects' plans).

## How the fast ones handle the tail (with sources)

- Similar-function retrieval as few-shot examples, across all worktrees and other decomps. Snowboard Kids 1 (N64, 2,145 functions in 84 days) and Frogger (grep of 23 other GBA decomps; git history of asm-to-C commits as verified pairs). [blog.chrislewis.au](https://blog.chrislewis.au/decompiling-a-nintendo-64-game-in-84-days/), [frog-adv-temple-decomp](https://github.com/JRickey/frog-adv-temple-decomp)
- The strongest model for the hardest functions, with a few attempts each. Codex beat Claude on Snowboard Kids; Fable added matches on functions that always failed, all within 3 attempts. Claude "more or less gives up" above about 1,000 instructions. [long tail post](https://blog.chrislewis.au/the-long-tail-of-llm-assisted-decompilation/), [Fable 5 post](https://gambiconf.substack.com/p/fable-5-does-the-smartest-llm-decompile)
- The permuter with a deadline the agent can see, started whenever an agent improves a draft (Snowboard Kids 1, mizuchi).
- Fixing parameter widths to what callers pass, real `static inline` helpers, and linker symbols for fixed bases removed 91 register pins in Frogger with no permuter runs (`docs/depin-worklist.md`).
- agbcc diagnostics: the Klonoa fork of agbcc adds `-finstrument-src-locs`, `-fdump-reg-lifetimes` and related dumps ([Dream-Atelier/agbcc](https://github.com/Dream-Atelier/agbcc)). Stock agbcc has the GCC `-dg`/`-dl` RTL dumps (read in `toplev.c`, not yet run).
- About 5% of Snowboard Kids 1 matches needed human experts.

## Shared code

No decomp or disassembly of any other Konami Yu-Gi-Oh! GBA game was found. EDS's sound driver is Konami's own ([[sound-engine]]), so pret's m4a code does not apply. Two other Konami GBA decomps exist (Castlevania Aria of Sorrow, Boktai 2) but both use m4a; engine sharing with EDS is unverified.

## Changes for EDS, ranked by gain per effort

1. Spend model budget on the tail only, with the strongest model and a cap of 3 to 6 attempts per function, several functions in parallel. Check whether Fable has its own weekly quota on this plan.
2. Build a cross-project agbcc corpus (khcom, knidl, Klonoa, FE8, pret, tmc, mzm) in an ignored folder and extend `tools/similar.py` to search it, giving agents the five nearest matched neighbours.
3. Add first-mismatch offset and matching-prefix length to `tools/check.py`, so large functions can be fixed region by region.
4. Start a deadline-bounded permuter run automatically when an agent improves a draft, instead of only the nightly queue.
5. Run the de-pin playbook on register-allocation failures: caller-side parameter widths, `static inline` helpers, symbol bases.
6. Triage the 272 remaining functions by class: pure register colouring goes to the permuter, structural diffs go to agents.
7. Try the `-dg`/`-dl` RTL dumps on the hardest few functions.
8. Post the hardest 20 to 50 as decomp.me scratches when stuck.

See also [[decomp-workflow]], [[matching-tricks]], [[rom-free-workflow]].
