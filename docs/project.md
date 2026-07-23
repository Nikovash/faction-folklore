# Wrath Blast

## Vision

Wrath Blast is the first title in the **Faction Folklore** series and serves as
the player's introduction to the Luna faction of the MK-Dirge universe.

The game is a native, terminal-based dungeon crawler designed to fit entirely
within the storage limits of a standard 1.44 MB floppy disk.

Wrath Blast begins as a familiar dungeon crawler before gradually introducing
Luna's philosophy through increasingly unusual mechanics. Every rank teaches
exactly one major concept until the player ultimately confronts Luna's
greatest lesson:

> **Power borrowed is not power earned.**

Completion of the game awards the hidden Rank XIII title,
**Suicide King**, along with an unlock code that can be redeemed in
**Arena King** to unlock Luna's signature Wrath Deck.

---

# Core Gameplay Loop

Every floor follows the same fundamental gameplay loop.

```
Explore

↓

Fight

↓

Collect Loot

↓

Complete the Rank Trial

↓

Become Stronger

↓

Advance to the Next Rank
```

Although the basic gameplay remains recognizable throughout the game, every
rank permanently changes how the player approaches future floors.

The objective is to teach mechanics gradually while continuously changing the
player's expectations.

---

# Player Progression

Wrath Blast does **not** feature traditional RPG progression.

The player becomes stronger only during the current run.

Everything earned during a run—including:

- Equipment
- Wrath abilities
- Titles
- Statistics
- Inventory

—is lost upon death.

The only permanent progression is:

- Player knowledge
- Mechanical mastery
- Successfully earned unlock codes

Victory is achieved through understanding rather than grinding.

---

# Death

Death is permanent.

When the player dies:

- The current run immediately ends.
- The player returns to Rank I.
- All equipment is lost.
- All titles are lost.
- All Wrath abilities are lost.
- All progression is reset.

The only things that remain are:

- The player's own knowledge.
- Unlock codes previously earned.

---

# Rank Progression

## Rank I

### Purpose

Teach the player the fundamentals of the dungeon crawler.

### New Mechanics

- Movement
- Combat
- Loot
- Healing
- Exploration
- Exit Stair

### Objective

Reach the exit.

---

## Rank II

### Theme

**Wrath Awakens**

### Introduces

- Wrath resource
- First Wrath abilities

The player learns that aggression creates power.

---

## Rank III

### Theme

**Pressure**

### Introduces

- Stronger enemy combinations
- Resource management

---

## Rank IV

### Theme

**Sacrifice**

### Introduces

- Destroying equipment
- Trading power for progression
- Permanent decisions within a run

---

## Rank V

### Theme

**Memory**

### Introduces

- Returning enemies
- Repeating encounters
- Contradictory lore

---

## Rank VI

### Theme

**Momentum**

### Introduces

- Combat chains
- Punishment for hesitation
- Maintaining offensive pressure

---

## Rank VII

### Theme

**Recurrence**

### Introduces

Reset Rooms.

When the player leaves certain rooms they return to their original state.

Possible resets include:

- Enemies
- Loot
- Traps
- Puzzle states

The player must begin planning routes rather than clearing rooms one at a time.

---

## Rank VIII

### Theme

**Trials**

### Introduces

Floor Objectives.

Finding the exit is no longer enough.

The player must complete one or more objectives before progression is allowed.

Possible objectives include:

- Destroy Altars
- Recover Relics
- Kill Named Enemy
- Burn Memories

---

## Rank IX

### Theme

**Judgment**

### Introduces

Floor Bosses.

Bosses become one objective among several rather than replacing exploration.

---

## Rank X

### Theme

**Recognition**

The player earns their first Luna Title.

Titles grant powerful passive abilities for the remainder of the run.

---

## Rank XI

### Title

**One-Eyed Jack**

The player gains significant offensive capability while sacrificing vision.

Possible effects include:

- Reduced field of view
- Directional vision
- Hidden enemy information
- Limited situational awareness

The player becomes stronger while knowing less.

---

## Rank XII

### Title

**Bed-Post Queen**

The player temporarily ascends to near-divine power.

Possible benefits include:

- Massive health
- Massive Wrath reserves
- Incredible relics
- Extraordinary combat ability
- Near immortality

Enemies become equally terrifying.

The purpose of this rank is psychological.

The player should genuinely begin believing they have surpassed the game's
difficulty.

---

## Final Boss

Immediately before the final encounter:

**Bed-Post Queen is stripped away.**

Every temporary advantage disappears.

Only true mastery remains.

The final boss is a comprehensive examination of every mechanic introduced
throughout the previous twelve ranks.

Victory is earned through understanding—not borrowed power.

---

## Rank XIII

### Title

**Suicide King**

Awarded only after defeating the final boss.

Rank XIII is never playable.

Instead it represents complete mastery of Luna's trial.

The player receives:

- Rank XIII
- Suicide King
- Arena King unlock code

---

# Enemy Types

Enemy Categories

- Melee
- Ranged
- Elite
- Memory Echoes
- Wrath Users
- Named Bosses

## Enemy Design Principles

Every enemy should reinforce Luna's philosophy.

Enemies should:

- Encourage movement
- Punish hesitation
- Reward aggression
- Become more dangerous through sacrifice
- Feel increasingly willing to destroy themselves to defeat the player

---

# Loot

Loot exists in several categories.

## Equipment

- Weapons
- Armor
- Charms

## Consumables

- Healing
- Wrath
- Utility

## Relics

Rare items with powerful passive effects.

Relics should encourage risk rather than safety.

## Artifacts

Extremely rare items capable of fundamentally changing a run.

Artifacts should encourage experimentation instead of guaranteed victory.

---

# Wrath System

Wrath is Luna's primary resource.

Wrath is generated through aggressive play.

Examples include:

- Taking damage
- Killing enemies
- Destroying objects
- Sacrificing equipment
- Completing dangerous objectives

Wrath is spent on powerful abilities.

Wrath should reward commitment rather than caution.

The safest decision should rarely be the strongest one.

---

# Save System

Wrath Blast uses no traditional save system.

A run is expected to be completed in a single session.

Death immediately ends the run.

Future versions may support suspend-and-resume functionality, but never
checkpoint saves or permanent gameplay progression.

---

# Unlock Codes

Successful completion awards:

- Rank XIII
- Suicide King
- Arena King unlock code

Arena King recognizes this code and unlocks Luna's exclusive Wrath Deck.

Each future Faction Folklore title will award its own faction-specific deck.

---

# Technical Architecture

## Language

- ISO C17

## Target Platforms

- Windows
- macOS
- Linux

## Rendering

- ANSI Terminal
- Unicode Box Drawing
- CP437-compatible fallback

## Build Goals

- Native executable
- Cross-platform
- No external runtime
- No game engine
- Data-driven content
- Extremely small executable

## Storage Goal

The complete game must fit within the capacity of a standard 1.44 MB floppy
disk.

Shared systems may be reused across future Faction Folklore titles while each
game remains a completely standalone experience.

---

# Design Pillars

Every feature added to Wrath Blast should reinforce one or more of these
principles.

## Simplicity

The player should always understand what happened.

## Escalation

Every rank introduces exactly one major mechanic.

## Consequence

Every decision should matter.

## Mastery

Success comes from player skill—not permanent progression.

## Sacrifice

Power should always come with meaningful cost.

## Replayability

No two successful runs should feel identical.

---

# Non-Negotiable Design Rules

1. Death ends the run.
2. Every run begins at Rank I.
3. No gameplay advantages carry between runs.
4. Mastery comes from player knowledge—not character levels.
5. Every rank teaches exactly one major mechanic.
6. Borrowed power will always be taken away.
7. Every mechanic should reinforce Luna's philosophy.
8. Every byte matters.
9. The game must remain fully playable offline.
10. The complete game must fit on a standard 1.44 MB floppy disk.
