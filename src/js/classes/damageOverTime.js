// Damage over time auras
class DamageOverTime {
	constructor(player) {
		this.player = player;
		this.durationTotal = 0;
		this.tickTimerTotal = 3;
		this.tickTimerRemaining = 0;
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.ticksRemaining = 0;
		this.snapshots = false;
		this.dmg = 0;
		this.spellPower = 0;
		this.modifier = 1;
		this.active = false;
		this.school = null;
		this.name = null;
		this.varName = null;
		this.coefficient = 0;
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
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.snapshots = true;
		this.dmg = 1035;
		this.modifier = 1 + (0.01 * player.talents.contagion);
		this.school = "shadow";
		this.name = "Corruption";
		this.varName = "corruption";
		this.coefficient = 0.936 + (0.12 * player.talents.empoweredCorruption);
		player.damageBreakdown[this.varName] = player.damageBreakdown[this.varName] || {"name": this.name};
	}
}

class UnstableAfflictionDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 18;
		this.tickTimerTotal = 3;
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.snapshots = true;
		this.dmg = 1155;
		this.school = "shadow";
		this.name = "Unstable Affliction";
		this.varName = "unstableAffliction";
		this.coefficient = 1.2;
		player.damageBreakdown[this.varName] = player.damageBreakdown[this.varName] || {"name": this.name};
	}
}

class SiphonLifeDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 30;
		this.tickTimerTotal = 3;
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.snapshots = true;
		this.dmg = 695;
		this.school = "shadow";
		this.name = "Siphon Life";
		this.varName = "siphonLife";
		this.coefficient = 1;
		player.damageBreakdown[this.varName] = player.damageBreakdown[this.varName] || {'name': this.name};
	}
}

class ImmolateDot extends DamageOverTime {
	constructor(player) {
		super(player);
		this.durationTotal = 15;
		this.tickTimerTotal = 3;
		this.ticksTotal = this.durationTotal / this.tickTimerTotal;
		this.snapshots = true;
		this.dmg = 615;
		this.school = "fire";
		this.name = "Immolate";
		this.varName = "immolate";
		this.coefficient = 0.65;
		player.damageBreakdown[this.varName] = player.damageBreakdown[this.varName] || {"name": this.name};
	}
}