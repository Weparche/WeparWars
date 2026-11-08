# Unreal Editor / local machine checklist

1. Install Epic Games Launcher, a compatible Unreal Engine 5 minor version, and the corresponding **Lyra Starter Game** project. Enable C++ support with Visual Studio 2022 (Desktop development with C++ + game development with C++).
2. Clone WeparWars. Run `powershell -ExecutionPolicy Bypass -File Tools/InstallPlugin.ps1 -LyraProjectRoot "C:\Path\To\LyraStarterGame"`. Script refuses to overwrite existing plugin.
3. Open Lyra .uproject; enable WeparCombat; build Development Editor Win64. Save compiler output, warnings and engine minor version.
4. Create `/Game/Wepar/` content folder and `L_WeparArena` map based on existing Lyra environment or appropriately licensed free local Fab art.
5. Create `B_WeparMonk`, `B_WeparWarrior` (or project-owned subclasses) sharing Lyra's ASC, health, teams, movement; attach `WeparTargetingComponent` and `WeparCastStateComponent`.
6. Create `GA_WeparInfuse`, `GA_WeparHeal`, `GA_WeparStrike`, `GA_WeparSpike`, `GA_WeparDisrupt` as Lyra Gameplay Abilities. Bind server-owned cast completion to canonical GAS effects. Interrupt canceled abilities, not just bars.
7. Create `DA_WeparMonkAbilities`, `DA_WeparWarriorAbilities` and an Experience / Game Feature to grant them; do not duplicate core Lyra ASC or HealthSet.
8. Create input mapping with Tab enemy targeting, F1/F2 ally/self, 1–8 skills. Cycle target deterministically on the local client; validate on authority.
9. CommonUI/UMG HUD `WBP_WeparHUD`: two friendly party HP frames (clickable), target frame, cast bar using server-world time, slots 1–8, cooldown and temporary timer/ping indicator.
10. Remove visible shooter weapon and shooting reticle. Assign existing legally usable character meshes/animations; no custom model generation required.
11. Set 4 clients, 2 teams, 1 arena, separate process if practical. Run listen-server and dedicated-server tests when available.
12. Capture evidence: screenshot of 4 clients, console logs showing authoritative heal/interrupt, no UE compile errors. Then test simulated packet latency and packet loss.
13. Submit results to `Docs/STATUS.md` or PR description: engine version, commit hash, actual tests, blockers, screenshots.

**Important:** Plugin is a code skeleton, not a ready-to-play `.uproject`. Do not claim this checklist has been completed until there is local evidence.
