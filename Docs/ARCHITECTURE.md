# Architecture and responsibilities

## Engine integration
**Baseline:** Unreal Engine 5.x matching the installed Lyra Starter Game sample; pin exact minor version locally. Do not create a second game instance/ASC/team/health system.

- Lyra's existing Experience/Gameplay Feature grants ability sets and input mappings.
- `ULyraAbilitySystemComponent` is the single combat ability system; `ULyraHealthSet` and its Healing/Damage meta attributes are the authoritative health model.
- `ULyraTeamSubsystem` is the authoritative source for faction rules; teams are not trusted from client input.
- A new thin `WeparCombat` runtime plugin holds portable rules, Blueprint preview utilities, a target-selection component and server-owned cast-state component.
- Ability implementations should extend Lyra's `ULyraGameplayAbility` when integrating into the host project; keep dependency direction clean and avoid copying Lyra source files to this repository.
- Each player pawn gains target and cast components via a derived Blueprint or project-owned subclass; do not change Lyra's shared samples unnecessarily.

## What is implemented now
- `WeparRules.h`: C++17 deterministic preview math and targeting/cast predicates. **No engine dependency**; CI compiles it separately.
- `UWeparRulesBlueprintLibrary`: Blueprint access to preview math; **does not apply effects**.
- `UWeparTargetingComponent`: locally optimistic selection + server correction, owner-only replication. **Does not grant targeting rights or prove team validity**.
- `UWeparCastStateComponent`: replicated server-time cast bar and server-only interruption timer. **Does not directly cancel GAS ability**.
- Portable tests. These are not Unreal or network functional tests.

## Integration caveats Codex must fix explicitly
- Implement server-side GAS ability state and invoke `CancelAbility/CancelAbilities` on interrupt, as well as `CastState.InterruptCast`; ensure timer callbacks cannot resolve a canceled ability.
- Synchronize target ID through the same authoritative ability activation / event payload when needed; targeting RPC on Pawn and ability RPC on PlayerState may race across channels.
- Ensure cast ability applies `GameplayEffect` to Lyra's *Healing/Damage* meta attributes rather than direct SetHealth hacks that bypass death/hit events.
- Assign valid team IDs from `LyraTeamSubsystem`, not client editable values.
- Configure `WeparCombat` module and plugin in the locally installed Lyra project, and verify correct engine version.
- Actual GUI (`CommonUI`/UMG) and binary `.uasset` level and input assets must be produced in Editor (possibly with Editor Python utilities); no assets are shipped in this repository.

## Risks
- The Lyra code sample is frequently revised with engine upgrades; verify exact APIs against the local version before adapting.
- Server-only cast timer with a client UI is an initial reliable prototype. Improved client prediction can follow after combat validation.
- Never claim network test passes from a deterministic math test or single-player PIE session.

## References
- https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine
- https://dev.epicgames.com/documentation/en-us/unreal-engine/abilities-in-lyra-in-unreal-engine
- https://dev.epicgames.com/documentation/en-us/unreal-engine/using-gameplay-abilities-in-unreal-engine
