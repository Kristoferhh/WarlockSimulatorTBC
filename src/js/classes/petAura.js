class PetAura {
	constructor(pet) {
		this.pet = pet;
		this.duration = 0;
		this.durationRemaining = 0;
		this.active = false;
	}

	tick(t) {
		if (this.active) {
			this.durationRemaining = Math.max(0, this.durationRemaining - t);
			if (this.durationRemaining == 0) {
				this.fade();
			}
		}
	}

	apply() {
		this.active = true;
		this.durationRemaining = this.duration;
	}

	fade(endOfIteration = false) {
		this.active = false;
		this.durationRemaining = 0;
		if (!endOfIteration) this.pet.player.combatLog(this.name + " (" + this.stacks + ") fades from " + this.pet.name);
	}
}

class DemonicFrenzy extends PetAura {
	constructor(pet) {
		super(pet);
		this.name = "Demonic Frenzy";
		this.duration = 10;
		this.maxStacks = 10;
		this.stacks = 0;
	}

	apply() {
		if (this.stacks < this.maxStacks) {
			this.stacks++;
			this.pet.player.combatLog(this.pet.name + " gains " + this.name + " (" + this.stacks + "). Current AP: " + (this.pet.ap * (1 + (0.05 * this.stacks))) + ")");
		}
		super.apply();
	}

	fade(endOfIteration = false) {
		this.stacks = 0;
		super.fade(endOfIteration);
	}
}