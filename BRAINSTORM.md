# Project Boomerang - Developer Planning
An open-source video game project started by four college dudes.

## Table of Contents
1. [Summary](#summary)
1. [Genre](#genre)
1. [Gameplay](#gameplay)
1. [Combat](#combat)
1. [Skill Tree](#skill-tree)
1. [Other Features](#other-features)
1. [Miscellanious Notes](#miscellanious-notes)
1. [Dependancies](#dependancies)
1. [Enemy AI](#enemy-ai)
1. [Data Management](#data-management)
1. [To Be Planned](#to-be-planned)

***

### Summary
The goal of Project Boomerang is to create a fun, open-source video game that draws on inspiration from games enjoyed by the developers.

This game is meant to be easily accessible for new players but also maintain a high skill ceiling so more seasoned players will continue to have fun. To do this, the core mechanics of the game will remain simple, but will be expanded upon by new skills and equipment that players can collect.

While the initial development will focus on singleplayer, it remains a goal to create a fun multiplayer experience. In doing this, our aim is to enjoy a video game with friends that may not play many other video games, which explains our decision in how we want the learning curve to be.

Various major features such as a skill tree and upgradable equipment will be implemented to promote a certain level of customization. This allows players to enjoy their time in this make-believe world how they want to, and hopefully provide a niche role to different players when multiplayer features roll around.

In short, the goal of Project Boomerang is to create a fun, relaxed game that players can easily pick up and keep coming back to.

At the end of each section will be a link to the table of contents for easier navigation of this file:<br>
[Table Of Contents](#table-of-contents)

***

### Genre
##### Adventure - fantasy/medieval & steampunk
The intended genre of the game is adventure taking place in a cross between fantasy, medieval, and steampunk settings. The reason for this is that we love: fireballs, swords, medieval systems, explosives, and technical weaponry. Having all of these elements will allow for a variety of situations and hopefully a dynamic experience.

[Table Of Contents](#table-of-contents)

***

### Gameplay
Project Boomerang will adopt a 2D, top-down, third-person style.

The objective of the player will be to follow various quests found in and around a home-base. These quests will lead the player into dungeons and forests filled with enemies and cool loot. While each quest may not be connected, they will each work together to create a cohesive world and eventually unveil an overarching story.

For developers, we will have to be concerned with a quest tree. When to trigger the availability of different quests and how they correspond to each other.

Besides the home-base, which the player will start at, the world will be procedurally generated (likely utilizing Perlin Noise) to create unique experiences for each player. We will use world seeds to make sure players can enjoy the same thing if they wish.

[Table Of Contents](#table-of-contents)

***

### Combat
This form of combat will utilize three core mechanics: attack, block, and dodge. By making these three mechanics the core of the combat, this will allow for an easier learning curve and higher mastery of just a few mechanics.

Combat will be maintained by using a health system and a potential mana/stamina system for unique abilities.

Other mechanics will be introduced as the player continues, such as mechanics introduced from the skill tree, special upgrades, or equipment: counter attacks and combo attacks.

[Table Of Contents](#table-of-contents)

***

### Skill Tree
Skill trees are something important and fun for our team, so it's only fitting to have one.

The skill tree will have four main branches, each with different sub-branches to provide higher specialization. Each branch may also cross over with closely related branches so that interesting combinations can be created.

The skill tree will be completed using skill points unlocked through leveling or other various means. While there will be a limit to the amount of points available, these points can be reset at any time in case the player wishes to change their style of play. This allows the player to explore all the available options without restarting a character, but not become more powerful than would be fun to play.

Here is a list of the tree's categories:
- Technician, specializes in using resources to become powerful
  - Damage
  - Support
- Magician, specializes in wielding various magics to fit each situation
  - Damage
  - Support
- Melee, specializes in close-up martial arts to outlast or eliminate enemies
  - Durability
  - Damage
- Range, specializes in maintaing a safe distance to kite and destroy foes
  - Utility?
  - Damage

[Table Of Contents](https://github.com/wooandrew/boomerang/master/PB_Brainstorming.md#table-of-contents)

***

### Other Features
This section contains a list of various minor or major features that aren't yet deserving of their own section.

#### Fast Travel
Fast travel is important for a game that intends on have a large world to explore. It will likely be implemented either through some sort of magic teleportation or horse-powered movement systems (like carriage rides).

#### Gathering
The world will be filled with various collectables that the player can pick up and use on their quests.

#### Smithing
The player will be able to use collectable resources to craft and upgrade different equipment. This crafting system will be something where the player can select an item and craft it if they have the required resources.

#### Farming
A crop/resource farming system will be created to provide the player with items to give stat boosts or bonus experience for a certain time, allowing for potential small boosts in different large encounters.

#### Goods
Still needs planning, but a currency or barter system will be used, potentially in a marketplace, to let the player reallocate collected resources into something else they would prefer. Experience can potentially be used as a currency.

#### Quest Board
In home base, there will be a quest board that the player can find posts of different contracts on. The player can complete these and return to the quest board or NPCs to collect the reward (and potential story information).

#### Powerful Final Boss
It would be satisfying.

#### Trophy System
Random trinkets can be collected on quests to keep as proof of things that have been done.

#### Achievements
Achievements will be rewarded at different milestones to give some form of satisfaction and boasting power.

#### Stats Counter
A stats counter will be created for various things such as enemies killed, highest dps, highest damage number, and other things to provide interesting information that the player could use or be proud of.

#### Chickens
There will be chickens. Everywhere.

#### Boomerangs
There will also be boomerangs. There may be a boomerang for each specialization tree. There may also be super boomerangs from the final boss. The final boss might even BE a boomerang.

[Table Of Contents](#table-of-contents)

***

### Miscellanious Notes

[Table Of Contents](#table-of-contents)

***

### Dependancies
This section contains a list of dependancies and what their use is. Hyperlinks to the dependancies and more references will be included later.

- GLFW (OpenGL Wrapper)
- GLAD (GL Loader)
- GLM (GL Math)
- FreeType (Font Rendering)
- PortAudio (Audio library)
- myGUI (UI)


[Table Of Contents](#table-of-contents)

***

### Enemy AI
The goal with the enemy AI is to make something very interactive, but not too complicated. There will be attack patterns that are triggered by different statuses relevant to the enemy, such as distance, health, or time since combat began. These patterns can either be single attacks or strings of attacks.

The enemy's tracking will likely be implemented utilizing some for of A* tracking or other method.

[Table Of Contents](#table-of-contents)

***

### Data Management
When saving the game, there will be a list of data required. When saving, we are going to use JSON data formatting and utilize binary encryption on it to ensure players cannot easily cheat their way through the game.

Here is the current list of data that we will need:

- World Data
  - World Seed
  - NPC Status
  - World Loot
  - Dungeon Data
    - Dungeon Progress/Completion
    - Dungeon Loot
- Player Data
  - Player Location
  - Player Inventory
  - Player Skill Tree
  - Player Level & Experience
  - Quest Progress
  - Any misc. player customizations
- Settings

[Table Of Contents](#table-of-contents)

***

### To Be Planned
This sections contains multiple feature ideas that we plan on implementing in the future but also need to develop more in-depth plans about.

- Netcode and updating the game
- Multiplayer functionality
- Fonts and menu art

[Table Of Contents](#table-of-contents)
