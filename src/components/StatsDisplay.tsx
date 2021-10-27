import { useSelector } from "react-redux"
import { shouldDisplayPetSetting } from "../Common";
import { Auras } from "../data/Auras";
import { RootState } from "../redux/Store"
import { StatConstant } from "../Types";


export default function StatsDisplay() {
  const playerState = useSelector((state: RootState) => state.player);

  function getStamina(): number {
    let stamina = playerState.stats.stamina;
    let staminaModifier = playerState.stats.staminaModifier;

    if (playerState.auras.bloodPact) {
      let bloodPactModifier = parseInt(playerState.settings.improvedImpSetting);

      // If the player is using an imp, the imp is active, and the player has more points in the Improved Imp talent than the improved imp setting then use that instead
      if (shouldDisplayPetSetting(playerState.talents, playerState.settings, false) && playerState.settings.petChoice === '0') {
        bloodPactModifier = Math.max(bloodPactModifier, playerState.talents.improvedImp);
      }

      stamina += 70 * (0.1 * bloodPactModifier);
    }

    staminaModifier *= 1 + (0.03 * playerState.talents.demonicEmbrace);

    return stamina * staminaModifier;
  }

  function getIntellect(): number {
    return playerState.stats.intellect * playerState.stats.intellectModifier;
  }

  function getSpirit(): number {
    let spiritModifier = playerState.stats.spiritModifier

    if (playerState.talents.demonicEmbrace > 0) {
      spiritModifier *= 1 - 0.01 * playerState.talents.demonicEmbrace;
    }

    return playerState.stats.spirit * spiritModifier;
  }

  function getHealth(): number {
    return (playerState.stats.health + getStamina() * StatConstant.healthPerStamina) * (1 + (0.01 * playerState.talents.felStamina));
  }

  function getMana(): number {
    return (playerState.stats.mana + getIntellect() * StatConstant.manaPerIntellect) * (1 + (0.01 * playerState.talents.felIntellect));
  }

  function getSpellPower(): number {
    let spellPower = playerState.stats.spellPower;

    if (playerState.auras.felArmor) {
      spellPower += 100 * (0.1 * playerState.talents.demonicAegis);
    }
    if (playerState.auras.powerOfTheGuardianWarlock) {
      spellPower += 33 * parseInt(playerState.settings.warlockAtieshAmount);
    }
    if (playerState.auras.prayerOfSpirit) {
      spellPower += getSpirit() * 0.05 * parseInt(playerState.settings.improvedDivineSpirit);
    }
    // Spellfire 3pc
    if (playerState.sets['552'] && playerState.sets['552'] === 3) {
      spellPower += getIntellect() * 0.07;
    }
    if (playerState.auras.wrathOfAirTotem && playerState.settings.improvedWrathOfAirTotem === 'yes') {
      spellPower += 20;
    }

    return spellPower;
  }

  function getShadowPower(): string {
    const shadowPower = playerState.stats.shadowPower.toString();

    return `${shadowPower} (${parseInt(shadowPower) + Math.round(getSpellPower())})`;
  }

  function getFirePower(): string {
    const firePower = playerState.stats.firePower.toString();

    return `${firePower} (${parseInt(firePower) + Math.round(getSpellPower())})`;
  }

  function getCrit(): string {
    let critRating = Math.round(playerState.stats.critRating);
    if (playerState.auras.powerOfTheGuardianMage) {
      critRating += 28 * parseInt(playerState.settings.mageAtieshAmount);
    }

    let critPercent = Math.round((critRating / StatConstant.critRatingPerPercent + StatConstant.baseCritChancePercent) * 100) / 100;
    critPercent += playerState.talents.devastation;
    critPercent += playerState.talents.backlash;
    critPercent += playerState.talents.demonicTactics;
    critPercent += getIntellect() * StatConstant.critPercentPerIntellect;
    if (playerState.auras.moonkinAura) critPercent += 5;
    if (playerState.auras.judgementOfTheCrusader) critPercent += 3;
    if (playerState.auras.totemOfWrath) critPercent += 3 * parseInt(playerState.settings.totemOfWrathAmount);
    if (playerState.auras.chainOfTheTwilightOwl) critPercent += 2;

    return `${critRating} (${critPercent.toFixed(2)}%)`;
  }

  function getHit(): string {
    let hitRating = playerState.stats.hitRating;
    // Mana-Etched 2pc
    if (playerState.sets['658'] && playerState.sets['658'] >= 2) hitRating += 35;

    let hitPercent = Math.round((hitRating / StatConstant.hitRatingPerPercent) * 100) / 100;
    if (playerState.auras.inspiringPresence) hitPercent++;
    if (playerState.auras.totemOfWrath) hitPercent += 3 * parseInt(playerState.settings.totemOfWrathAmount);

    return `${hitRating} (${hitPercent.toFixed(2)}%)`;
  }

  function getHaste(): string {
    let hasteRating = playerState.stats.hasteRating;
    let hastePercent = Math.round((hasteRating / StatConstant.hasteRatingPerPercent) * 100) / 100;

    return `${hasteRating} (${hastePercent.toFixed(2)}%)`
  }

  function getShadowAndFireModifier(): number {
    let modifier = 1;

    if (playerState.auras.curseOfTheElements) {
      modifier *= (1.1 + (0.01 * parseInt(playerState.settings.improvedCurseOfTheElements)));
    }

    // Master Demonologist & Soul Link
    if (playerState.talents.demonicSacrifice === 0 || playerState.settings.sacrificePet === 'no') {
      if (playerState.talents.masterDemonologist > 0) {
        switch (playerState.settings.petChoice) {
          case '2': // Succubus
            modifier *= (1 + (0.02 * playerState.talents.masterDemonologist));
            break;
          case '4': // Felguard
            modifier *= (1 + (0.01 * playerState.talents.masterDemonologist));
            break;
        }
      }

      if (playerState.talents.soulLink === 1) {
        modifier *= 1.05;
      }
    }

    if (playerState.auras.ferociousInspiration) {
      modifier *= Math.pow(1.03, parseInt(playerState.settings.ferociousInspirationAmount));
    }

    return modifier;
  }

  function getShadowModifier(): string {
    let modifier = playerState.stats.shadowModifier * getShadowAndFireModifier() * (1 + (0.02 * playerState.talents.shadowMastery));

    if (playerState.talents.demonicSacrifice === 1 && playerState.settings.sacrificePet === 'yes') {
      switch (playerState.settings.petChoice) {
        case '2': // Succubus
          modifier *= 1.15;
          break;
        case '4': // Felguard
          modifier *= 1.1;
          break;
      }
    }

    return `${Math.round(modifier * 100)}%`;
  }

  function getFireModifier(): string {
    let modifier = playerState.stats.fireModifier * getShadowAndFireModifier();

    if (playerState.talents.demonicSacrifice === 1 && playerState.settings.sacrificePet === 'yes' && playerState.settings.petChoice === '0') {
      modifier *= 1.15;
    }

    modifier *= 1 + (0.02 * playerState.talents.emberstorm);

    return `${Math.round(modifier * 100)}%`;
  }

  function getMp5(): number {
    let mp5 = playerState.stats.mp5;

    if (playerState.auras.vampiricTouch) {
      mp5 += parseInt(playerState.settings.shadowPriestDps) * 0.25;
    }

    return mp5;
  }

  function getEnemyArmor(): number {
    let armor = parseInt(playerState.settings.enemyArmor);

    if (playerState.auras.faerieFire) armor -= 610;
    if ((playerState.auras.sunderArmor && playerState.auras.exposeArmor && playerState.settings.improvedExposeArmor === '2') || (playerState.auras.exposeArmor && !playerState.auras.sunderArmor)) armor -= 2050 * (1 + 0.25 * parseInt(playerState.settings.improvedExposeArmor));
    else if (playerState.auras.sunderArmor) armor -= 520 * 5;
    if (playerState.auras.curseOfRecklessness) armor -= 800;
    if (playerState.auras.annihilator) armor -= 600;

    return Math.max(0, armor);
  }

  const stats: {name: string, value: () => string, condition?: () => boolean}[] = [
    { name: 'Health', value: () => Math.round(getHealth()).toString() },
    { name: 'Mana', value: () => Math.round(getMana()).toString() },
    { name: 'Stamina', value: () => Math.round(getStamina()).toString() },
    { name: 'Intellect', value: () => Math.round(getIntellect()).toString() },
    { name: 'Spirit', value: () => Math.round(getSpirit()).toString() },
    { name: 'Spell Power', value: () => Math.round(getSpellPower()).toString() },
    { name: 'Shadow Power', value: () => getShadowPower() },
    { name: 'Fire Power', value: () => getFirePower() },
    { name: 'Crit Rating', value: () => getCrit() },
    { name: 'Hit Rating', value: () => getHit() },
    { name: 'Haste Rating', value: () => getHaste() },
    { name: 'Shadow Dmg %', value: () => getShadowModifier() },
    { name: 'Fire Dmg %', value: () => getFireModifier() },
    { name: 'MP5', value: () => Math.round(getMp5()).toString() },
    { name: 'Enemy Armor', value: () => Math.round(getEnemyArmor()).toString(), condition: () => shouldDisplayPetSetting(playerState.talents, playerState.settings, true) }
  ]

  return (
    <ul className="character-stats">
      {
        stats.map((stat, i) =>
          (stat.condition === undefined || stat.condition() === true) &&
            <li key={i}>
              <p className='character-stat'>{stat.name}</p>
              <p className='character-stat-val'>{stat.value()}</p>
            </li>
        )
      }
    </ul>
  )
}