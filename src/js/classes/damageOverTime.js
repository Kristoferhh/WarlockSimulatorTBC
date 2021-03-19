// Damage over time auras
class DamageOverTime {
	constructor(player) {
		this.player = player;
		this.durationTotal = 0;
		this.tickTimerTotal = 3;
		this.tickTimerRemaining = 0;
		this.ticksRemaining = 0;
		this.snapshots = true;
		this.dmg = 0;
		this.spellPower = 0;
		this.modifier = 1;
		this.active = false;
		this.school = null;
		this.name = null;
		this.coefficient = 0;
	}

	setup() {
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.varName = camelCase(this.name);
		this.player.damageBreakdown[this.varName] = this.player.damageBreakdown[this.varName] || {"name": this.name};
	}

	apply(spellPower) {
		this.active = true;
		this.tickTimerRemaining = this.tickTimerTotal;
		this.ticksRemaining = this.ticksTotal;
		this.spellPower = spellPower;
	}

	tick(t) {
		if (this.active) {
			this.tickTimerRemaining = Math.max(0,this.tickTimerRemaining - t);

			if (this.tickTimerRemaining == 0) {
				let sp = 0;
				if (this.snapshots) {
					sp = this.spellPower;
				} else {
					sp = this.player.stats.spellPower + this.player.stats[this.school + "Power"];
				}

				let dmg = ((this.dmg + sp * this.coefficient) * this.modifier * this.player.stats[this.school + "Modifier"]) / this.ticksTotal;
				// Add bonus from ISB (without removing ISB stacks since it's a dot)
				if (this.school == "shadow" && this.player.talents.improvedShadowBolt > 0 && this.player.auras.improvedShadowBolt.active) {
					dmg *= this.player.auras.improvedShadowBolt.modifier;
				}

				// Check for Nightfall proc
				if (this.varName == "corruption" && this.player.talents.nightfall > 0) {
					if (random(1,100) <= this.player.talents.nightfall * 2) {
						this.player.auras.shadowTrance.apply();
					}
				}

				this.player.damageBreakdown[this.varName].damage = this.player.damageBreakdown[this.varName].damage + dmg || dmg;
				this.player.iterationDamage += dmg;
				this.player.combatLog(this.name + " " + Math.round(dmg));
				this.ticksRemaining--;
				this.tickTimerRemaining = this.tickTimerTotal;

				if (this.ticksRemaining <= 0) {
					this.active = false;
				}
			}
		}
	}
}

class CorruptionDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 18;
		this.tickTimerTotal = 3;
		this.dmg = 1035;
		this.modifier = 1 + (0.01 * player.talents.contagion);
		this.school = "shadow";
		this.name = "Corruption";
		this.coefficient = 0.936 + (0.12 * player.talents.empoweredCorruption);
		this.setup();
	}
}

class UnstableAfflictionDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 18;
		this.tickTimerTotal = 3;
		this.dmg = 1155;
		this.school = "shadow";
		this.name = "Unstable Affliction";
		this.coefficient = 1.2;
		this.setup();
	}
}

class SiphonLifeDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 30;
		this.tickTimerTotal = 3;
		this.dmg = 695;
		this.school = "shadow";
		this.name = "Siphon Life";
		this.coefficient = 1;
		this.setup();
	}
}

class ImmolateDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 15;
		this.tickTimerTotal = 3;
		this.dmg = 615;
		this.school = "fire";
		this.name = "Immolate";
		this.coefficient = 0.65;
		this.setup();
	}
}

class CurseOfAgonyDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 24;
		this.tickTimerTotal = 3;
		this.dmg = 1698;
		this.school = "shadow";
		this.name = "Curse of Agony";
		this.coefficient = 1.2;
		this.setup();
	}
}