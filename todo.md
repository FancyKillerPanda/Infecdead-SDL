# Infecdead

### Bug Fixes

- [ ] Profile slow render performance.




### Features

#### General
- [ ] Saving and loading of game world.
	- [ ] Should be able to save to/load from one of the three profiles.



#### Progression
##### Stage 0
- [ ] Initial phone call conversation (from friend to you):
	  Friend: "Hello, it's <friend_name>! I need your help"
	  Me:     "What's going on?"
	  Friend: "There's an infection going around, it's turning the citizens into zombies!"
	  Me:     "I'm on my way"

##### Stage 1
- [ ] Fade through black to initial town hall scene.
- [ ] Cannot leave town hall until talk with friend.
- [ ] Conversation goes something like:
	  Me:     "Wow, it's worse than I thought. Where is everyone?"
	  Friend: "I'm so glad you made it. There are people scattered all across the town, I need
			   you to go and rescue them. I'm working on an antidote here."
	  Me:     "I'll try - no, I will - get them all!"

- [ ] Chest in the town hall, with a small weapon.

- [ ] UI should indicate current objective.
- [ ] UI should have a point counter, which goes up with each kill. Later on, curing should increase this more.

##### Stage 2
- [ ] Exit town hall.
	- [ ] Outside should be an indicator of where to bring the hostages (rounded rectangle?).
	- [ ] Maybe a tooltip outlining exactly what to do?

- [ ] Bring the hostages back to the indicated area.
	- [ ] Zombies deal damage to the player.
	- [ ] Zombies deal damage to the hostages.
	- [ ] Hostages move towards the player, and away from nearby zombies.

- [ ] Different weapons could be available.
	- [ ] Pistol:
		  - Range: short
		  - Cooldown: medium
		  - Number of shots: medium
		  - Damage: high
	- [ ] Rifle:
		  - Range: long
		  - Cooldown: long
		  - Number of shots: low
		  - Damage: high
	- [ ] Slingshot (shoots multiple projectiles):
		  - Range: short
		  - Cooldown: medium
		  - Number of shots: medium
		  - Damage: low (per projectile)
- [ ] Weapons should have a stun mode, which only temporarily disables the zombies.
	  - [ ] Perhaps a blue projectile instead of the normal black one?
	  - [ ] To make the choice between shoot and stun more relevant, maybe stun can fail
			and enrage the zombie (for a limited time?). Enraged zombies would move faster
			and do more damage. Or perhaps enraged zombies can infect hostages.

- [ ] UI should indicate number of hostages saved (perhaps in the objective tooltip?).
- [ ] UI should indicate number of zombies killed (perhaps in the objective tooltip?).
- [ ] Minimap should show location of remaining hostages.

##### Stage 3
- [ ] Objective should say something like "I should go talk to <friend_name> now".
- [ ] Return to the town hall and speak with the friend:
	  Friend: "Great job, you did it."
	  Me: "How else can I help?"
	  Friend: "I'm developing a cure for the infection, but I'm missing some key ingredients.
			   Can you get me some <products> from around the town?"

##### Stage 4
- [ ] Products should be in chests around the town (e.g. wheat seeds from farms, beakers from the lab).
- [ ] Player should place retrieved items in a town hall chest (marked out in UI).

- [ ] One building might have its lights out, player can choose to go find a torch.
- [ ] Player has limited inventory space, so they must choose what to keep with them.

- [ ] Once all the items have been retrieved, friend starts talking again.

##### Stage 5
- [ ] Dialogue:
	  Friend: "Excellent. I have everything I need now."
	  Friend: "That should do it. Here, take this and try cure as many zombies as possible."

- [ ] Weapons should now have a third mode (shoot, stun, cure).
	- [ ] Curing process may take a second or two.
	- [ ] Curing may fail.
- [ ] Player must clear the map of zombies, whether that be by killing or by curing.
	- [ ] Maybe curing gives more reward points than killing.
- [ ] Zombies should be able to re-infect cured hostages.

- [ ] Alternatively, perhaps curing is a melee attack, which incentivises stunning the zombie first
	  (and bringing into play the enragement mechanism).
