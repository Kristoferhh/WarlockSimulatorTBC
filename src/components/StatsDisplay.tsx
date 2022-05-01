import { nanoid } from "nanoid";
import { useTranslation } from "react-i18next";
import { useSelector } from "react-redux";
import {
  getPlayerHitPercent,
  getPlayerHitRating,
  isPetActive,
} from "../Common";
import { RootState } from "../redux/Store";
import { ItemSet, PetName, StatConstant } from "../Types";

export default function StatsDisplay() {
  const playerState = useSelector((state: RootState) => state.player);
  const { t } = useTranslation();

  function getStamina(): number {
    let stamina = Object.values(playerState.stats)
      .map((obj) => obj.stamina || 0)
      .reduce((a, b) => a + b);
    let staminaModifier = Object.values(playerState.stats)
      .map((obj) => obj.staminaModifier || 1)
      .reduce((a, b) => a * b);

    if (playerState.auras.bloodPact) {
      let bloodPactModifier =
        parseInt(playerState.settings.improvedImpSetting) || 0;

      // If the player is using an imp, the imp is active, and the player
      // has more points in the Improved Imp talent than the improved imp setting then use that instead
      if (
        isPetActive(playerState.talents, playerState.settings, false, false) &&
        playerState.settings.petChoice === PetName.IMP
      ) {
        bloodPactModifier = Math.max(
          bloodPactModifier,
          playerState.talents.improvedImp
        );
      }

      stamina += 70 * (0.1 * bloodPactModifier);
    }

    staminaModifier *= 1 + (0.03 * playerState.talents.demonicEmbrace || 0);

    return stamina * staminaModifier;
  }

  function getIntellect(): number {
    return (
      Object.values(playerState.stats)
        .map((obj) => obj.intellect || 0)
        .reduce((a, b) => a + b) *
      Object.values(playerState.stats)
        .map((obj) => obj.intellectModifier || 1)
        .reduce((a, b) => a * b)
    );
  }

  function getSpellPenetration(): number {
    return Object.values(playerState.stats)
      .map((obj) => obj.spellPenetration || 0)
      .reduce((a, b) => a + b);
  }

  function getSpirit(): number {
    let spiritModifier = Object.values(playerState.stats)
      .map((obj) => obj.spiritModifier || 1)
      .reduce((a, b) => a * b);

    if (playerState.talents.demonicEmbrace > 0) {
      spiritModifier *= 1 - 0.01 * playerState.talents.demonicEmbrace;
    }

    return (
      Object.values(playerState.stats)
        .map((obj) => obj.spirit || 0)
        .reduce((a, b) => a + b) * spiritModifier
    );
  }

  function getHealth(): number {
    return (
      (playerState.stats.base.health! +
        getStamina() * StatConstant.healthPerStamina) *
      (1 + (0.01 * playerState.talents.felStamina || 0))
    );
  }

  function getMana(): number {
    let mana = Object.values(playerState.stats)
      .map((obj) => obj.mana || 0)
      .reduce((a, b) => a + b);
    return (
      (mana + getIntellect() * StatConstant.manaPerIntellect) *
      (1 + (0.01 * playerState.talents.felIntellect || 0))
    );
  }

  function getSpellPower(): number {
    let spellPower = Object.values(playerState.stats)
      .map((obj) => obj.spellPower || 0)
      .reduce((a, b) => a + b);

    if (playerState.auras.felArmor) {
      spellPower += 100 * (0.1 * playerState.talents.demonicAegis);
    }
    if (playerState.auras.powerOfTheGuardianWarlock) {
      spellPower += 33 * parseInt(playerState.settings.warlockAtieshAmount);
    }
    if (playerState.auras.prayerOfSpirit) {
      spellPower +=
        getSpirit() *
        0.05 *
        parseInt(playerState.settings.improvedDivineSpirit);
    }
    if (playerState.sets[ItemSet.Spellfire] === 3) {
      spellPower += getIntellect() * 0.07;
    }
    if (
      playerState.auras.wrathOfAirTotem &&
      playerState.settings.improvedWrathOfAirTotem === "yes"
    ) {
      spellPower += 20;
    }

    return spellPower;
  }

  function getShadowPower(): string {
    const shadowPower = Object.values(playerState.stats)
      .map((obj) => obj.shadowPower || 0)
      .reduce((a, b) => a + b)
      .toString();

    return `${shadowPower} (${
      parseInt(shadowPower) + Math.round(getSpellPower())
    })`;
  }

  function getFirePower(): string {
    const firePower = Object.values(playerState.stats)
      .map((obj) => obj.firePower || 0)
      .reduce((a, b) => a + b)
      .toString();

    return `${firePower} (${
      parseInt(firePower) + Math.round(getSpellPower())
    })`;
  }

  function getCrit(): string {
    let critRating = Math.round(
      Object.values(playerState.stats)
        .map((obj) => obj.critRating || 0)
        .reduce((a, b) => a + b)
    );
    if (playerState.auras.powerOfTheGuardianMage) {
      critRating += 28 * parseInt(playerState.settings.mageAtieshAmount);
    }

    let critPercent =
      Math.round(
        (critRating / StatConstant.critRatingPerPercent +
          StatConstant.baseCritChancePercent) *
          100
      ) / 100;
    critPercent += playerState.talents.devastation || 0;
    critPercent += playerState.talents.backlash || 0;
    critPercent += playerState.talents.demonicTactics || 0;
    critPercent += getIntellect() * StatConstant.critPercentPerIntellect;
    if (playerState.auras.moonkinAura) critPercent += 5;
    if (playerState.auras.judgementOfTheCrusader) critPercent += 3;
    if (playerState.auras.totemOfWrath)
      critPercent += 3 * parseInt(playerState.settings.totemOfWrathAmount);
    if (playerState.auras.chainOfTheTwilightOwl) critPercent += 2;

    return `${critRating} (${critPercent.toFixed(2)}%)`;
  }

  function getHit(): string {
    let hitRating = getPlayerHitRating(playerState);
    let hitPercent =
      Math.round(getPlayerHitPercent(playerState, hitRating) * 100) / 100;

    return `${hitRating} (${hitPercent.toFixed(2)}%)`;
  }

  function getHaste(): string {
    let hasteRating = Object.values(playerState.stats)
      .map((obj) => obj.hasteRating || 0)
      .reduce((a, b) => a + b);
    let hastePercent =
      Math.round((hasteRating / StatConstant.hasteRatingPerPercent) * 100) /
      100;

    return `${hasteRating} (${hastePercent.toFixed(2)}%)`;
  }

  function getShadowAndFireModifier(): number {
    let modifier = 1;

    if (playerState.auras.curseOfTheElements) {
      modifier *=
        1.1 + 0.01 * parseInt(playerState.settings.improvedCurseOfTheElements);
    }

    // Master Demonologist & Soul Link
    if (
      playerState.talents.demonicSacrifice === 0 ||
      playerState.settings.sacrificePet === "no"
    ) {
      if (playerState.talents.masterDemonologist > 0) {
        switch (playerState.settings.petChoice) {
          case PetName.SUCCUBUS:
            modifier *= 1 + 0.02 * playerState.talents.masterDemonologist;
            break;
          case PetName.FELGUARD:
            modifier *= 1 + 0.01 * playerState.talents.masterDemonologist;
            break;
        }
      }

      if (playerState.talents.soulLink === 1) {
        modifier *= 1.05;
      }
    }

    if (playerState.auras.ferociousInspiration) {
      modifier *= Math.pow(
        1.03,
        parseInt(playerState.settings.ferociousInspirationAmount)
      );
    }

    return modifier;
  }

  function getShadowModifier(): string {
    let modifier =
      Object.values(playerState.stats)
        .map((obj) => obj.shadowModifier || 1)
        .reduce((a, b) => a * b) *
      getShadowAndFireModifier() *
      (1 + (0.02 * playerState.talents.shadowMastery || 0));

    if (
      playerState.talents.demonicSacrifice === 1 &&
      playerState.settings.sacrificePet === "yes"
    ) {
      switch (playerState.settings.petChoice) {
        case PetName.SUCCUBUS:
          modifier *= 1.15;
          break;
        case PetName.FELGUARD:
          modifier *= 1.1;
          break;
      }
    }

    return `${Math.round(modifier * 100)}%`;
  }

  function getFireModifier(): string {
    let modifier =
      Object.values(playerState.stats)
        .map((obj) => obj.fireModifier || 1)
        .reduce((a, b) => a * b) * getShadowAndFireModifier();

    if (
      playerState.talents.demonicSacrifice === 1 &&
      playerState.settings.sacrificePet === "yes" &&
      playerState.settings.petChoice === PetName.IMP
    ) {
      modifier *= 1.15;
    }

    modifier *= 1 + (0.02 * playerState.talents.emberstorm || 0);

    return `${Math.round(modifier * 100)}%`;
  }

  function getMp5(): number {
    let mp5 = Object.values(playerState.stats)
      .map((obj) => obj.mp5 || 0)
      .reduce((a, b) => a + b);

    if (playerState.auras.vampiricTouch) {
      mp5 += parseInt(playerState.settings.shadowPriestDps) * 0.25;
    }

    return mp5;
  }

  function getEnemyArmor(): number {
    let armor = parseInt(playerState.settings.enemyArmor);

    if (playerState.auras.faerieFire) {
      armor -= 610;
    }

    if (
      (playerState.auras.sunderArmor &&
        playerState.auras.exposeArmor &&
        playerState.settings.improvedExposeArmor === "2") ||
      (playerState.auras.exposeArmor && !playerState.auras.sunderArmor)
    ) {
      armor -=
        2050 * (1 + 0.25 * parseInt(playerState.settings.improvedExposeArmor));
    } else if (playerState.auras.sunderArmor) {
      armor -= 520 * 5;
    }

    if (playerState.auras.curseOfRecklessness) {
      armor -= 800;
    }

    if (playerState.auras.annihilator) {
      armor -= 600;
    }

    return Math.max(0, armor);
  }

  const stats: {
    name: string;
    value: () => string;
    condition?: () => boolean;
  }[] = [
    { name: "Health", value: () => Math.round(getHealth()).toString() },
    { name: "Mana", value: () => Math.round(getMana()).toString() },
    { name: "Stamina", value: () => Math.round(getStamina()).toString() },
    { name: "Intellect", value: () => Math.round(getIntellect()).toString() },
    { name: "Spirit", value: () => Math.round(getSpirit()).toString() },
    {
      name: "Spell Power",
      value: () => Math.round(getSpellPower()).toString(),
    },
    { name: "Shadow Power", value: () => getShadowPower() },
    { name: "Fire Power", value: () => getFirePower() },
    { name: "Crit Rating", value: () => getCrit() },
    { name: "Hit Rating", value: () => getHit() },
    { name: "Haste Rating", value: () => getHaste() },
    { name: "Shadow Dmg %", value: () => getShadowModifier() },
    { name: "Fire Dmg %", value: () => getFireModifier() },
    { name: "MP5", value: () => Math.round(getMp5()).toString() },
    {
      name: "Spell Penetration",
      value: () => getSpellPenetration().toString(),
      condition: () => getSpellPenetration() > 0,
    },
    {
      name: "Enemy Armor",
      value: () => Math.round(getEnemyArmor()).toString(),
      condition: () =>
        isPetActive(playerState.talents, playerState.settings, true, true),
    },
  ];

  return (
    <ul className="character-stats">
      {stats
        .filter(
          (stat) => stat.condition === undefined || stat.condition() === true
        )
        .map((stat) => (
          <li key={nanoid()}>
            <p className="character-stat">{t(stat.name)}</p>
            <p className="character-stat-val">{stat.value()}</p>
          </li>
        ))}
    </ul>
  );
}
