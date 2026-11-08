# Codex handoff — implement first playable 2v2

## Role and objective
You are the lead Unreal Engine 5 C++ / Lyra gameplay engineer for WeparWars, a new original competitive fantasy arena inspired by the *mechanics* of classic tab-target MMORPG PvP. Deliver the **earliest genuinely playable 2v2**, not a concept mockup.

Repository: https://github.com/Weparche/WeparWars
Read README.md, Docs/COMBAT_SPEC.md, Docs/ARCHITECTURE.md and Docs/EDITOR_CHECKLIST.md **before coding**.

## Environment reconnaissance (first action)
- Detect OS, installed UE engine minor version, Visual Studio compiler/toolchains, local Lyra Starter Game folder and whether it compiles unchanged.
- If Lyra or UE is not installed, do not pretend you can build it. Prepare the code + automation and give the operator the minimum specific installation action.
- Do not download proprietary engine/sample/marketplace assets from unofficial mirrors or commit Epic-supplied binary assets to the public repo.
- Check whether Plugins/WeparCombat code compiles against the installed engine. Correct generated-header, replication, API, warnings and ownership issues; document changes.
- Run portable rules tests immediately (Tests/RulesTests.cpp); distinguish this from a real UE build.

## Implement in this exact order (work autonomously)
1. Establish a working local Lyra-derived game project and project-owned 2v2 mode/Experience. Preserve existing Lyra gameplay and avoid rewriting networking.
2. Create the 2-team 4-player session on a single map using Lyra teams, spawning and PlayerState. Exactly two players per side; support four local PIE clients and sensible respawn/reset for repeated tests.
3. Character roles: Monk and Warrior, each with one shared authoritative Lyra ASC + health state. Use available free compatible 3D character/environment assets; avoid original-copyrighted GW art.
4. Targeting: Tab/Shift+Tab enemies, party bar click and F1/F2 for allies; selected target and target casting readouts; server always validates team/range/LoS/aliveness at resolution. Fix target-selection ordering races across Pawn and PlayerState channels.
5. Combat: via Lyra GAS GameplayAbilities and proper GameplayEffects, not ad hoc client damage RPCs. Infuse cast 0.25s, sacrifices half current caster HP at cast resolution, heals other ally for 2× sacrificed HP (balance placeholder), capped at max. If interrupted / out of range / target dead, neither cost nor heal happens. Implement one Heal, Strike, Spike, and Disrupt with defined placeholder stats.
6. True server-authoritative cast start/finish/interrupt. Cancel the underlying GAS activation (not just replicated UI). Ensure no double-apply and no delayed effect after interruption.
7. HUD: dark fantasy readable aesthetic; 2 party HP bars, visible target HP/cast, player HP, 8 slots, cooldown indicators and cast progress. Prioritize clarity over particle spam. No default rifle.
8. Automated combat-rule tests AND appropriate Unreal functional tests; provide manual reproducible 4-client test. Verify at simulated RTT 80, 150, 250ms with latency and optional packet loss.
9. Integrate an attractive small map and licensed ready-made animations/VFX, then capture a real in-engine 2v2 screenshot; never present concept art as actual gameplay.
10. Iterate until true acceptance criteria below pass. Commit changes in understandable PR-sized chunks. No GvG, HA, PvE, matchmaking, leveling or monetization in this milestone.

## Acceptance criteria — must PROVE, not merely claim
- The host actually launches an Unreal scene with **4 independently controlled human clients** across 2 teams.
- Both Monks can target teammate via party frame; neither can Infuse self nor opponent.
- Warrior damages enemy by server-owned GAS effects. Caster health is actually reduced and allied target healed by Infuse exactly once (with max cap).
- An interrupt received before server cast completion prevents healing AND HP sacrifice, including under 150ms simulated RTT.
- Death, invalid LoS, invalid range, target switch and duplicated activation do not yield illegitimate effects.
- Both teams see correctly replicated HP/cast state. Party/UI state is not cosmetic hardcoding.
- Show actual build/test commands, test output, net logs, screenshot, commit hash; list failures honestly.
- Record work and next blockers in Docs/STATUS.md. If local Unreal unavailable, ship code/assets generator/checklist and report why playable test is unverified.

## Priorities
Correctness and gameplay feel > minimal but polished HUD > pretty fantasy asset environment > additional abilities. Parallelize independent tasks only when your environment safely supports it. Do not waste time on speculative architecture or full MMORPG backend.

## Notes
The current WeparCombat plugin supplies **pure math and replicated shell components**, not finished GAS abilities. It may require engine-specific corrections. Use existing Lyra's `ULyraHealthSet` for authoritative health and `ULyraTeamSubsystem` for team checks.
