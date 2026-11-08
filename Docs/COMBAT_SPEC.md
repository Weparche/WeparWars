# Combat spec — 2v2 v0.1

## Match
- Exactly 4 humans: Blue (Monk+Warrior) vs Red (Monk+Warrior).
- One existing map; 1 round ends when both enemies die; add round reset later.
- No equipment progression, PvE, builds beyond fixed prototype bars, GvG, HA, lobbies or matchmaking yet.
- Vertical slice must have *both* readable cast bars and clickable party HP frames.

## UI / controls
- Tab = cycle enemy players by stable distance ordering; Shift+Tab = reverse.
- F1 = target self for self-only actions; F2 = teammate. Click friendly party bar to target teammate.
- Keys 1–8 activate slots; Escape cancels your current selection; configurable bindings later.
- Target selection cannot be the ultimate authority: server revalidates team, range, LoS, live status and cooldown at **cast resolution**.
- Display your HP, energy placeholder, 2-member party panel, enemy target HP, ally and enemy cast bars, spell bar and cooldowns.
- Combat camera: third-person orbit / shoulder, no shooter reticle or gun animation after final integration.
- Cast progress uses **replicated server-world-time start and duration**; client clocks do not decide hits.

## Initial abilities (BALANCE PLACEHOLDERS, not copied GW1 stats)
| Role | Skill | Cast | Effect |
| --- | --- | --- | --- |
| Monk | Infuse Health | 0.25s | Sacrifice half current caster HP; heal other living ally for 2× HP lost (max HP cap); no self target |
| Monk | Healing Touch | 0.75s | Restore 45 HP to ally/self, capped |
| Monk | Protective Ward | 0.25s | Prevent 35 damage from next single hit, if possible in milestone 1 |
| Warrior | Strike | 0.35s | 30 damage to enemy in melee range |
| Warrior | Spike | 0.50s | 80 damage to enemy in melee range; cooldown 6s |
| Warrior | Disrupt | 0.20s | Interrupt an actively casting enemy in melee range; cooldown 8s |

8 UI slots can include placeholders / disabled slots at first; no need to invent 8 unique skills each.

## Rules of authority
1. Client predicts **selection/UI/cast animation**, never final health or interrupt result.
2. Local input triggers a Lyra/GAS ability on owning character. Ability activation and cooldown validated through GAS.
3. Authority resolves cast at server time. Revalidate target (including race from another actor channel), teams using LyraTeamSubsystem, line of sight, range, alive status, silenced/dead, resources and cooldown.
4. All health changes through Lyra existing `ULyraHealthSet` / GameplayEffects; **do not install a second health attribute source**.
5. Infuse sacrifices HP at resolution; no cost if cast interrupted / invalid. Mutate cost+healing as one server-side resolution with a single canonical combat event.
6. Interrupt must explicitly cancel the GAS ability and prevent delayed cast completion.
7. Prevent double resolve, stale target, target-switch race, death-before-cast and late interrupt exploits.
8. No ability RPC or public function should allow arbitrary player-provided damage/heal amounts.

## Infuse edge cases
- Caster HP 100, target HP 20/200 -> caster 50, target 120.
- Caster HP 100, target HP 190/200 -> caster 50, target 200 (10 actual healing).
- If target dies, leaves range, or LoS fails before resolution: canceled without sacrifice.
- If interrupted before server cast deadline: no sacrifice and no healing.
- Multiple simultaneous attacks/heals resolve deterministically on the server according to processed event order.
- Range units in UE: centimeters, starter range 3000cm; validate throughout.
- Initial small multiplier 2.0 is a *tunable placeholder* for combat-feel testing, not a faithful skill recreation.

## Non-functional objectives
- Dedicated server architecture compatible, but milestone test is 4-client local PIE/listen server.
- Log ability request, cast-start, interrupt, success, health changes with server timestamp for replay/debug.
- Manual test with 0, 80, 150, 250 ms simulated RTT and packet loss 0%, 1% to detect desync. Latency cannot be inferred from smooth animations alone.
