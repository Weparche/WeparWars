# WeparWars — 2v2 PvP vertical slice

A **Guild Wars 1–inspired** (not affiliated with ArenaNet) competitive fantasy PvP prototype. Focus: responsive tab targeting, team frames, visible cast bars, interrupts, spikes, and a satisfying **Infuse Health** save.

> **Status:** Planning + deterministic C++ combat-rule kernel in this repository. **Not yet a playable Unreal project.** Epic's Lyra Starter Game and licensed art assets are **not** checked in.

## First playable acceptance scenario

Four local human players: **Blue Monk + Warrior** vs **Red Monk + Warrior**, one small arena. Warrior focuses a target while Monk sees the party HP bar fall, presses Infuse with a **0.25 s cast**, loses **50% current HP**, heals an allied teammate from sacrificed HP, and survives an enemy spike. Enemy interrupt can stop an active cast. Server authoritatively checks team, target, cast, cooldown, and result.

## Repository structure

- `Plugins/WeparCombat/` – portable rules + initial Unreal plugin code for integration by Codex
- `Tests/` – portable C++ rule tests runnable *without Unreal*
- `Docs/COMBAT_SPEC.md` – mechanics, constraints, input, target selection
- `Docs/ARCHITECTURE.md` – Lyra integration plan, replication and test strategy
- `Docs/EDITOR_CHECKLIST.md` – what must be configured in UE Editor
- `Docs/CODEX_HANDOFF.md` – exact next implementation prompt and exit criteria

## Quick start (Codex on Windows)

1. Install UE5 with **C++/Visual Studio 2022** toolchain.
2. Install or create a **Lyra Starter Game** project compatible with your installed engine version using Epic Games Launcher / Fab.
3. Copy `Plugins/WeparCombat/` into the **root of your local Lyra project**, not the other way around.
4. Enable `WeparCombat` plugin, generate IDE files, compile. Fix version/API differences; **do not claim Unreal build succeeded without actual build logs**.
5. Work through `Docs/CODEX_HANDOFF.md` and `Docs/EDITOR_CHECKLIST.md` to integrate abilities, HUD, and first local 4-player PIE session.

## Constraints

- Original code only. No copied Guild Wars names/art/audio/maps, no copied Epic sample assets or paid marketplace binaries in the repository.
- No pay-to-win, progression, GvG or HA for milestone one.
- Keep all damage/healing and cast completion server authoritative; predictable client-side cast UX is separate.
- No false claims of network correctness: validate against two- and four-client tests with added latency.
