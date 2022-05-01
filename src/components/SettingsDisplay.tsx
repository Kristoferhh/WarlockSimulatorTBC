import { nanoid } from "nanoid";
import { useTranslation } from "react-i18next";
import { useDispatch, useSelector } from "react-redux";
import { getBaseStats, isPetActive } from "../Common";
import { Races } from "../data/Races";
import { modifySettingValue, setBaseStats } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { PetName, Setting } from "../Types";

export default function SettingsDisplay() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();
  const { t } = useTranslation();

  function settingModifiedHandler(setting: string, value: string) {
    dispatch(modifySettingValue({ setting: setting as Setting, value: value }));
  }

  return (
    <section id="sim-settings">
      <fieldset>
        <legend>{t("Rotation Options")}</legend>
        <input
          id="sim-chooses-option"
          onChange={(e) =>
            settingModifiedHandler(Setting.rotationOption, e.target.value)
          }
          type="radio"
          name="rotationOption"
          value="simChooses"
          checked={playerStore.settings.rotationOption === "simChooses"}
        />
        <label htmlFor="sim-chooses-option">
          {t("Simulation chooses spells for me")}
        </label>
        <br />
        <input
          id="user-chooses-option"
          onChange={(e) =>
            settingModifiedHandler(Setting.rotationOption, e.target.value)
          }
          type="radio"
          name="rotationOption"
          value="userChooses"
          checked={playerStore.settings.rotationOption === "userChooses"}
        />
        <label htmlFor="user-chooses-option">{t("Choose spells myself")}</label>
      </fieldset>
      <ul>
        <li>
          <label className="settings-left">{t("Race")}</label>
          <select
            onChange={(e) => {
              settingModifiedHandler(Setting.race, e.target.value);
              dispatch(
                setBaseStats(
                  getBaseStats(
                    Races.find((race) => race.varName === e.target.value)!
                      .varName
                  )
                )
              );
            }}
            name="race"
            id="race-dropdown-list"
            className="settings-right"
            value={playerStore.settings.race}
          >
            <option value="gnome">{t("Gnome")}</option>
            <option value="human">{t("Human")}</option>
            <option value="orc">{t("Orc")}</option>
            <option value="undead">{t("Undead")}</option>
            <option value="bloodElf">{t("Blood Elf")}</option>
          </select>
        </li>
        <li>
          <label htmlFor="iterations" className="settings-left">
            {t("Iterations")}
          </label>
          <input
            id="iterations"
            onChange={(e) =>
              settingModifiedHandler(Setting.iterations, e.target.value)
            }
            value={playerStore.settings.iterations}
            step="1000"
            min="1000"
            type="number"
            name="iterations"
            className="settings-right"
          />
        </li>
        <li>
          <label htmlFor="min-fight-length" className="settings-left">
            {t("Min Fight Length")}
          </label>
          <input
            id="min-fight-length"
            onChange={(e) =>
              settingModifiedHandler(
                Setting["min-fight-length"],
                e.target.value
              )
            }
            value={playerStore.settings["min-fight-length"]}
            type="number"
            name="min-fight-length"
            className="settings-right"
          />
        </li>
        <li>
          <label htmlFor="max-fight-length" className="settings-left">
            {t("Max Fight Length")}
          </label>
          <input
            id="max-fight-length"
            onChange={(e) =>
              settingModifiedHandler(
                Setting["max-fight-length"],
                e.target.value
              )
            }
            value={playerStore.settings["max-fight-length"]}
            type="number"
            name="max-fight-length"
            className="settings-right"
          />
        </li>
        <li>
          <label htmlFor="target-level" className="settings-left">
            {t("Target Level")}
          </label>
          <input
            id="target-level"
            onChange={(e) =>
              settingModifiedHandler(Setting["target-level"], e.target.value)
            }
            value={playerStore.settings["target-level"]}
            type="number"
            name="target-level"
            className="settings-right"
          />
        </li>
        <li>
          <label htmlFor="target-shadow-resistance" className="settings-left">
            {t("Target Shadow Resistance")}
          </label>
          <input
            id="target-shadow-resistance"
            onChange={(e) =>
              settingModifiedHandler(
                Setting["target-shadow-resistance"],
                e.target.value
              )
            }
            value={playerStore.settings["target-shadow-resistance"]}
            type="number"
            name="target-shadow-resistance"
            className="settings-right"
          />
        </li>
        <li>
          <label htmlFor="target-fire-resistance" className="settings-left">
            {t("Target Fire Resistance")}
          </label>
          <input
            id="target-fire-resistance"
            onChange={(e) =>
              settingModifiedHandler(
                Setting["target-fire-resistance"],
                e.target.value
              )
            }
            value={playerStore.settings["target-fire-resistance"]}
            type="number"
            name="target-fire-resistance"
            className="settings-right"
          />
        </li>
        <li>
          <label className="settings-left">{t("Fight Type")}</label>
          <select
            onChange={(e) =>
              settingModifiedHandler(Setting.fightType, e.target.value)
            }
            value={playerStore.settings.fightType}
            name="fightType"
            id="fight-type"
            className="settings-right"
          >
            <option value="singleTarget">{t("Single Target")}</option>
            <option value="aoe">{t("AoE (Seed of Corruption)")}</option>
          </select>
        </li>
        {playerStore.settings.fightType === "aoe" && (
          <li
            id="enemy-amount"
            title="Including the target you're casting Seed of Corruption on"
          >
            <label className="settings-left">{t("Enemy Amount")}</label>
            <input
              name="enemyAmount"
              className="settings-right"
              onChange={(e) =>
                settingModifiedHandler(Setting.enemyAmount, e.target.value)
              }
              value={playerStore.settings.enemyAmount}
              step="1"
              min="1"
              type="number"
            />
          </li>
        )}
        <li id="automatically-open-sim-details">
          <label
            className="settings-left"
            htmlFor="automatically-open-sim-details"
          >
            {t("Show Damage & Aura Tables")}
          </label>
          <select
            className="settings-right"
            name="automatically-open-sim-details"
            onChange={(e) =>
              settingModifiedHandler(
                Setting["automatically-open-sim-details"],
                e.target.value
              )
            }
            value={playerStore.settings["automatically-open-sim-details"]}
          >
            <option value="yes">{t("Yes")}</option>
            <option value="no">{t("No")}</option>
          </select>
        </li>
        <li
          id="randomizeValues"
          title={t(
            "Chooses a random value between a minimum and a maximum value instead of taking the average of the two."
          )}
        >
          <label className="settings-left" htmlFor="randomizeValues">
            {t("Randomize instead of averaging")}
          </label>
          <select
            className="settings-right"
            name="randomizeValues"
            onChange={(e) =>
              settingModifiedHandler(Setting.randomizeValues, e.target.value)
            }
            value={playerStore.settings.randomizeValues}
          >
            <option value="no">{t("No")}</option>
            <option value="yes">{t("Yes")}</option>
          </select>
        </li>
        <li id="infinitePlayerMana">
          <label className="settings-left" htmlFor="infinitePlayerMana">
            {t("Infinite player mana?")}
          </label>
          <select
            className="settings-right"
            name="infinitePlayerMana"
            onChange={(e) =>
              settingModifiedHandler(Setting.infinitePlayerMana, e.target.value)
            }
            value={playerStore.settings.infinitePlayerMana}
          >
            <option value="no">{t("No")}</option>
            <option value="yes">{t("Yes")}</option>
          </select>
        </li>
        <li id="infinitePetMana">
          <label className="settings-left" htmlFor="infinitePetMana">
            {t("Infinite pet mana?")}
          </label>
          <select
            className="settings-right"
            name="infinitePetMana"
            onChange={(e) =>
              settingModifiedHandler(Setting.infinitePetMana, e.target.value)
            }
            value={playerStore.settings.infinitePetMana}
          >
            <option value="no">{t("No")}</option>
            <option value="yes">{t("Yes")}</option>
          </select>
        </li>
        <li id="petChoice">
          <label className="settings-left" htmlFor="petChoice">
            {t("Pet")}
          </label>
          <select
            className="settings-right"
            name="petChoice"
            onChange={(e) =>
              settingModifiedHandler(Setting.petChoice, e.target.value)
            }
            value={playerStore.settings.petChoice}
          >
            <option value={PetName.IMP}>{t("Imp")}</option>
            <option value={PetName.SUCCUBUS}>{t("Succubus")}</option>
            <option value={PetName.FELHUNTER}>{t("Felhunter")}</option>
            <option value={PetName.FELGUARD}>{t("Felguard")}</option>
          </select>
        </li>
        {playerStore.talents.demonicSacrifice === 1 && (
          <li id="sacrificePet">
            <label className="settings-left" htmlFor="sacrificePet">
              {t("Sacrifice pet?")}
            </label>
            <select
              className="settings-right"
              name="sacrificePet"
              onChange={(e) =>
                settingModifiedHandler(Setting.sacrificePet, e.target.value)
              }
              value={playerStore.settings.sacrificePet}
            >
              <option value="no">{t("No")}</option>
              <option value="yes">{t("Yes")}</option>
            </select>
          </li>
        )}
        {isPetActive(
          playerStore.talents,
          playerStore.settings,
          false,
          false
        ) && (
          <li id="petMode">
            <label className="settings-left" htmlFor="petMode">
              {t("Pet mode")}
            </label>
            <select
              className="settings-right"
              name="petMode"
              onChange={(e) =>
                settingModifiedHandler(Setting.petMode, e.target.value)
              }
              value={playerStore.settings.petMode}
            >
              <option value="0">{t("Passive")}</option>
              <option value="1">{t("Aggressive")}</option>
            </select>
          </li>
        )}
        {isPetActive(
          playerStore.talents,
          playerStore.settings,
          true,
          false
        ) && (
          <li id="prepopBlackBook">
            <label className="settings-left" htmlFor="prepopBlackBook">
              {t("Prepop Black Book")}?
            </label>
            <select
              className="settings-right"
              name="prepopBlackBook"
              onChange={(e) =>
                settingModifiedHandler(Setting.prepopBlackBook, e.target.value)
              }
              value={playerStore.settings.prepopBlackBook}
            >
              <option value="no">{t("No")}</option>
              <option value="yes">{t("Yes")}</option>
            </select>
          </li>
        )}
        {
          // Shattered Sun Pendant of Acumen equipped
          playerStore.selectedItems.neck === 34678 && (
            <>
              <li id="shattrathFaction">
                <label className="settings-left" htmlFor="shattrathFaction">
                  {t("Shattrath Faction")}
                </label>
                <select
                  className="settings-right"
                  name="shattrathFaction"
                  onChange={(e) =>
                    settingModifiedHandler(
                      Setting.shattrathFaction,
                      e.target.value
                    )
                  }
                  value={playerStore.settings.shattrathFaction}
                >
                  <option value="Aldor">{t("Aldor")}</option>
                  <option value="Scryers">{t("Scryers")}</option>
                </select>
              </li>
              <li id="shattrathFactionReputation">
                <label
                  className="settings-left"
                  htmlFor="shattrathFactionReputation"
                >
                  {t("Exalted with Shattrath Faction")}
                </label>
                <select
                  className="settings-right"
                  name="shattrathFactionReputation"
                  onChange={(e) =>
                    settingModifiedHandler(
                      Setting.shattrathFactionReputation,
                      e.target.value
                    )
                  }
                  value={playerStore.settings.shattrathFactionReputation}
                >
                  <option value="no">{t("No")}</option>
                  <option value="yes">{t("Yes")}</option>
                </select>
              </li>
            </>
          )
        }
        {
          // Display if pet is succubus, pet is aggressive, and pet is not being sacrificed.
          isPetActive(playerStore.talents, playerStore.settings, true, true) &&
            playerStore.settings.petChoice === PetName.SUCCUBUS && (
              <li id="lashOfPainUsage">
                <label className="settings-left" htmlFor="lashOfPainUsage">
                  {t("When to use Lash of Pain")}?
                </label>
                <select
                  className="settings-right"
                  name="lashOfPainUsage"
                  onChange={(e) =>
                    settingModifiedHandler(
                      Setting.lashOfPainUsage,
                      e.target.value
                    )
                  }
                  value={playerStore.settings.lashOfPainUsage}
                >
                  <option value="noISB">{t("When ISB is not up")}</option>
                  <option value="onCooldown">{t("On Cooldown")}</option>
                </select>
              </li>
            )
        }
        {isPetActive(playerStore.talents, playerStore.settings, true, true) && (
          <li id="enemyArmor">
            <label className="settings-left" htmlFor="enemyArmor">
              {t("Enemy Armor")}
            </label>
            <input
              className="settings-right"
              onChange={(e) =>
                settingModifiedHandler(Setting.enemyArmor, e.target.value)
              }
              value={playerStore.settings.enemyArmor}
              type="number"
              min="0"
              max="10000"
              name="enemyArmor"
            />
          </li>
        )}
        {playerStore.auras.curseOfTheElements === true && (
          <li id="improvedCurseOfTheElements">
            <label className="settings-left">{t("Malediction")}?</label>
            <select
              className="settings-right"
              name="improvedCurseOfTheElements"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.improvedCurseOfTheElements,
                  e.target.value
                )
              }
              value={playerStore.settings.improvedCurseOfTheElements}
            >
              <option value="0">{t("No")}</option>
              <option value="1">1/3</option>
              <option value="2">2/3</option>
              <option value="3">3/3</option>
            </select>
          </li>
        )}
        {playerStore.auras.powerInfusion && (
          <li id="powerInfusionAmount">
            <label className="settings-left" htmlFor="powerInfusionAmount">
              {t("Power Infusion amount")}
            </label>
            <select
              className="settings-right"
              name="powerInfusionAmount"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.powerInfusionAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.powerInfusionAmount}
            >
              {Array.from(Array(12), (e, i) => i + 1).map((number) => (
                <option value={number} key={nanoid()}>
                  {number}
                </option>
              ))}
            </select>
          </li>
        )}
        {playerStore.auras.bloodlust && (
          <li id="bloodlustAmount">
            <label className="settings-left" htmlFor="bloodlustAmount">
              {t("Bloodlust amount")}
            </label>
            <select
              className="settings-right"
              name="bloodlustAmount"
              onChange={(e) =>
                settingModifiedHandler(Setting.bloodlustAmount, e.target.value)
              }
              value={playerStore.settings.bloodlustAmount}
            >
              {Array.from(Array(15), (e, i) => i + 1).map((number) => (
                <option value={number} key={nanoid()}>
                  {number}
                </option>
              ))}
            </select>
          </li>
        )}
        {playerStore.auras.innervate && (
          <li id="innervateAmount">
            <label className="settings-left" htmlFor="innervateAmount">
              {t("Innervate amount")}
            </label>
            <select
              className="settings-right"
              name="innervateAmount"
              onChange={(e) =>
                settingModifiedHandler(Setting.innervateAmount, e.target.value)
              }
              value={playerStore.settings.innervateAmount}
            >
              {Array.from(Array(18), (e, i) => i + 1).map((number) => (
                <option value={number} key={nanoid()}>
                  {number}
                </option>
              ))}
            </select>
          </li>
        )}
        {playerStore.auras.prayerOfSpirit && (
          <li id="improvedDivineSpirit">
            <label className="settings-left" htmlFor="improvedDivineSpirit">
              {t("Improved Divine Spirit")}?
            </label>
            <select
              className="settings-right"
              name="improvedDivineSpirit"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.improvedDivineSpirit,
                  e.target.value
                )
              }
              value={playerStore.settings.improvedDivineSpirit}
            >
              <option value="0">{t("No")}</option>
              <option value="1">1/2</option>
              <option value="2">2/2</option>
            </select>
          </li>
        )}
        {playerStore.auras.powerOfTheGuardianMage && (
          <li id="mageAtieshAmount">
            <label className="settings-left" htmlFor="mageAtieshAmount">
              {t("Mage Atiesh amount")}
            </label>
            <select
              className="settings-right"
              name="mageAtieshAmount"
              onChange={(e) =>
                settingModifiedHandler(Setting.mageAtieshAmount, e.target.value)
              }
              value={playerStore.settings.mageAtieshAmount}
            >
              <option value="1">1</option>
              <option value="2">2</option>
              <option value="3">3</option>
              <option value="4">4</option>
            </select>
          </li>
        )}
        {playerStore.auras.powerOfTheGuardianWarlock && (
          <li id="warlockAtieshAmount">
            <label className="settings-left" htmlFor="warlockAtieshAmount">
              {t("Warlock Atiesh amount")}
            </label>
            <select
              className="settings-right"
              name="warlockAtieshAmount"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.warlockAtieshAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.warlockAtieshAmount}
            >
              <option disabled={true} value="0">{`>> ${t(
                "Do not count your own Atiesh"
              )} <<`}</option>
              <option value="1">1</option>
              <option value="2">2</option>
              <option value="3">3</option>
              <option value="4">4</option>
            </select>
          </li>
        )}
        {playerStore.auras.totemOfWrath && (
          <li id="totemOfWrathAmount">
            <label className="settings-left" htmlFor="totemOfWrathAmount">
              {t("Totem of Wrath amount")}
            </label>
            <select
              className="settings-right"
              name="totemOfWrathAmount"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.totemOfWrathAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.totemOfWrathAmount}
            >
              <option value="1">1</option>
              <option value="2">2</option>
              <option value="3">3</option>
              <option value="4">4</option>
            </select>
          </li>
        )}
        {playerStore.auras.chippedPowerCore && (
          <li id="chippedPowerCoreAmount">
            <label className="settings-left" htmlFor="chippedPowerCoreAmount">
              {t("Chipped Power Core amount")}
            </label>
            <input
              className="settings-right"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.chippedPowerCoreAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.chippedPowerCoreAmount}
              type="number"
              min="0"
              name="chippedPowerCoreAmount"
            />
          </li>
        )}
        {playerStore.auras.crackedPowerCore && (
          <li id="crackedPowerCoreAmount">
            <label className="settings-left" htmlFor="crackedPowerCoreAmount">
              {t("Cracked Power Core amount")}
            </label>
            <input
              className="settings-right"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.crackedPowerCoreAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.crackedPowerCoreAmount}
              type="number"
              min="0"
              name="crackedPowerCoreAmount"
            />
          </li>
        )}
        {playerStore.auras.ferociousInspiration && (
          <li id="ferociousInspirationAmount">
            <label
              className="settings-left"
              htmlFor="ferociousInspirationAmount"
            >
              {t("Ferocious Inspiration amount")}
            </label>
            <select
              className="settings-right"
              name="ferociousInspirationAmount"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.ferociousInspirationAmount,
                  e.target.value
                )
              }
              value={playerStore.settings.ferociousInspirationAmount}
            >
              <option value="1">1</option>
              <option value="2">2</option>
              <option value="3">3</option>
              <option value="4">4</option>
            </select>
          </li>
        )}
        {playerStore.auras.wrathOfAirTotem && (
          <li id="improvedWrathOfAirTotem">
            <label className="settings-left" htmlFor="improvedWrathOfAirTotem">
              {t("Elemental Shaman T4 2pc bonus")}?
            </label>
            <select
              className="settings-right"
              name="improvedWrathOfAirTotem"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.improvedWrathOfAirTotem,
                  e.target.value
                )
              }
              value={playerStore.settings.improvedWrathOfAirTotem}
            >
              <option value="no">{t("No")}</option>
              <option value="yes">{t("Yes")}</option>
            </select>
          </li>
        )}
        {playerStore.auras.vampiricTouch && (
          <li id="shadowPriestDps">
            <label className="settings-left" htmlFor="shadowPriestDps">
              {t("Shadow Priest Dps")}
            </label>
            <input
              className="settings-right"
              onChange={(e) =>
                settingModifiedHandler(Setting.shadowPriestDps, e.target.value)
              }
              value={playerStore.settings.shadowPriestDps}
              type="number"
              min="0"
              name="shadowPriestDps"
            />
          </li>
        )}
        {playerStore.auras.bloodPact && (
          <li id="improvedImpSetting">
            <label className="settings-left" htmlFor="improvedImpSetting">
              {t("Improved Imp")}?
            </label>
            <select
              className="settings-right"
              name="improvedImpSetting"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.improvedImpSetting,
                  e.target.value
                )
              }
              value={playerStore.settings.improvedImpSetting}
            >
              <option value="0">{t("No")}</option>
              <option value="1">1/3</option>
              <option value="2">2/3</option>
              <option value="3">3/3</option>
            </select>
          </li>
        )}
        {playerStore.auras.battleSquawk &&
          isPetActive(
            playerStore.talents,
            playerStore.settings,
            true,
            true
          ) && (
            <li>
              <label className="settings-left" htmlFor="battleSquawkAmount">
                {t("Battle Squawk amount")}
              </label>
              <select
                className="settings-right"
                id="battleSquawkAmount"
                onChange={(e) =>
                  settingModifiedHandler(
                    Setting.battleSquawkAmount,
                    e.target.value
                  )
                }
                value={playerStore.settings.battleSquawkAmount}
              >
                {Array.from(Array(5), (e, i) => i + 1).map((number) => (
                  <option value={number} key={nanoid()}>
                    {number}
                  </option>
                ))}
              </select>
            </li>
          )}
        {playerStore.auras.faerieFire &&
          isPetActive(
            playerStore.talents,
            playerStore.settings,
            true,
            true
          ) && (
            <li id="improvedFaerieFire">
              <label className="settings-left" htmlFor="improvedFaerieFire">
                {t("Improved Faerie Fire")}?
              </label>
              <select
                className="settings-right"
                name="improvedFaerieFire"
                onChange={(e) =>
                  settingModifiedHandler(
                    Setting.improvedFaerieFire,
                    e.target.value
                  )
                }
                value={playerStore.settings.improvedFaerieFire}
              >
                <option value="no">{t("No")}</option>
                <option value="yes">Yes</option>
              </select>
            </li>
          )}
        {playerStore.auras.exposeArmor &&
          isPetActive(
            playerStore.talents,
            playerStore.settings,
            true,
            true
          ) && (
            <li id="improvedExposeArmor">
              <label className="settings-left" htmlFor="improvedExposeArmor">
                {t("Improved Expose Armor")}?
              </label>
              <select
                className="settings-right"
                name="improvedExposeArmor"
                onChange={(e) =>
                  settingModifiedHandler(
                    Setting.improvedExposeArmor,
                    e.target.value
                  )
                }
                value={playerStore.settings.improvedExposeArmor}
              >
                <option value="0">{t("No")}</option>
                <option value="1">1/2</option>
                <option value="2">2/2</option>
              </select>
            </li>
          )}
        {playerStore.auras.exposeWeakness &&
          isPetActive(
            playerStore.talents,
            playerStore.settings,
            true,
            true
          ) && (
            <div>
              <li id="survivalHunterAgility">
                <label
                  className="settings-left"
                  htmlFor="survivalHunterAgility"
                >
                  {t("Survival Hunter Agility")}
                </label>
                <input
                  className="settings-right"
                  onChange={(e) =>
                    settingModifiedHandler(
                      Setting.survivalHunterAgility,
                      e.target.value
                    )
                  }
                  value={playerStore.settings.survivalHunterAgility}
                  type="number"
                  min="0"
                  name="survivalHunterAgility"
                />
              </li>
              <li id="exposeWeaknessUptime">
                <label className="settings-left" htmlFor="exposeWeaknessUptime">
                  {t("Expose Weakness Uptime")} %
                </label>
                <input
                  className="settings-right"
                  onChange={(e) =>
                    settingModifiedHandler(
                      Setting.exposeWeaknessUptime,
                      e.target.value
                    )
                  }
                  value={playerStore.settings.exposeWeaknessUptime}
                  type="number"
                  min="0"
                  name="exposeWeaknessUptime"
                />
              </li>
            </div>
          )}
        <li id="customIsbUptime">
          <label className="settings-left" htmlFor="customIsbUptime">
            {t("Use custom ISB uptime")} %?
          </label>
          <select
            className="settings-right"
            name="customIsbUptime"
            onChange={(e) =>
              settingModifiedHandler(Setting.customIsbUptime, e.target.value)
            }
            value={playerStore.settings.customIsbUptime}
          >
            <option value="yes">{t("Yes")}</option>
            <option value="no">{t("No")}</option>
          </select>
        </li>
        {playerStore.settings.customIsbUptime === "yes" && (
          <li id="custom-isb-uptime-value">
            <label htmlFor="customIsbUptimeValue" className="settings-left">
              {t("Custom ISB Uptime")} %
            </label>
            <input
              id="customIsbUptimeValue"
              onChange={(e) =>
                settingModifiedHandler(
                  Setting.customIsbUptimeValue,
                  e.target.value
                )
              }
              value={playerStore.settings.customIsbUptimeValue}
              type="number"
              name="customIsbUptimeValue"
              className="settings-right"
            />
          </li>
        )}
        <li>
          <label className="settings-left">
            {t(
              "Concurrent item sims amount (set to 0 to use the default amount)"
            )}
          </label>
          <input
            className="settings-right"
            onChange={(e) =>
              settingModifiedHandler(Setting.maxWebWorkers, e.target.value)
            }
            value={playerStore.settings.maxWebWorkers || 0}
            type="number"
          />
        </li>
      </ul>
    </section>
  );
}
