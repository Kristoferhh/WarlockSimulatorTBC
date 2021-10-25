import { useEffect } from "react";
import { useDispatch, useSelector } from "react-redux";
import { itemMeetsSocketRequirements, ItemSlotToItemSlotKey } from "../Common";
import { Auras } from "../Data/Auras";
import { Enchants } from "../Data/Enchants";
import { Gems } from "../Data/Gems";
import { Items } from "../Data/Items";
import { Races } from "../Data/Races";
import { modifyPlayerStat } from "../Redux/PlayerSlice";
import { RootState } from "../Redux/Store";
import { ItemSlot, Stat } from "../Types";


export default function Session() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  useEffect(() => {
    addRaceStats();
    addAuraStats();
    addItemStats();
    addEnchantStats();

    function addRaceStats() {
      const selectedRace = Races.find(e => e.varName === playerStore.settings.race);

      if (selectedRace != null) {
        Object.entries(selectedRace.stats).forEach(([key, val]) => dispatch(modifyPlayerStat({
          stat: key as Stat,
          value: val,
          action: 'add'
        })));
      }
    }

    function addAuraStats() {
      Auras.forEach((auraGroup) => {
        auraGroup.auras.forEach((aura) => {
          if (playerStore.auras[aura.varName] === true) {
            for (const stat in aura.stats) {
              dispatch(modifyPlayerStat({
                stat: stat as Stat,
                value: aura.stats[stat as Stat]!!,
                action: 'add'
              }));
            }
          }
        });
      })
    }

    function addItemStats() {
      console.log(playerStore.selectedGems);
      for (const [itemSlot, itemId] of Object.entries(playerStore.selectedItems)) {
        if (itemId !== 0) {
          const itemObj = Items.find(e => e.id === itemId)!!;

          for (const [stat, value] of Object.entries(itemObj)) {
            if (playerStore.stats.hasOwnProperty(stat)) {
              dispatch(modifyPlayerStat({
                stat: stat as Stat,
                value: value,
                action: 'add'
              }));
            }
          }

          // Add stats from gems in the item
          const selectedGemsItemObj = playerStore.selectedGems[ItemSlotToItemSlotKey(false, itemSlot as ItemSlot)];
          if (selectedGemsItemObj && selectedGemsItemObj[itemId]) {
            selectedGemsItemObj[itemId].forEach((gem) => {
              addGemStats(gem[1]);
            });
            
            // Add stats from socket bonus
            if (itemMeetsSocketRequirements({itemId: itemId, selectedGems: playerStore.selectedGems})) {
              for (const [stat, value] of Object.entries(itemObj.socketBonus!!)) {
                dispatch(modifyPlayerStat({
                  stat: stat as Stat,
                  value: value,
                  action: 'add'
                }));
              }
            }
          }
        }
      }
    }

    function addEnchantStats() {
      for (const [itemSlot, enchantId] of Object.entries(playerStore.selectedEnchants)) {
        if (enchantId !== 0) {
          const enchantObj = Enchants[ItemSlotToItemSlotKey(true, itemSlot as ItemSlot)].find(e => e.id === enchantId);

          if (enchantObj) {
            for (const [stat, value] of Object.entries(enchantObj)) {
              if (playerStore.stats.hasOwnProperty(stat)) {
                dispatch(modifyPlayerStat({
                  stat: stat as Stat,
                  value: value,
                  action: 'add'
                }))
              }
            }
          }
        }
      }
    }

    function addGemStats(gemId: number) {
      const gem = Gems.find(e => e.id === gemId);
      
      if (gem && gem.stats) {
        for (const [stat, value] of Object.entries(gem.stats)) {
          dispatch(modifyPlayerStat({
            stat: stat as Stat,
            value: value,
            action: 'add'
          }));
        }
      }
    }
  }, []);

  return(<div></div>);
}