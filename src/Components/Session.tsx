import { useEffect } from "react";
import { useDispatch, useSelector } from "react-redux";
import { itemMeetsSocketRequirements, ItemSlotToItemSlotKey } from "../Common";
import { Auras } from "../data/Auras";
import { Enchants } from "../data/Enchants";
import { Gems } from "../data/Gems";
import { Items } from "../data/Items";
import { Races } from "../data/Races";
import { modifyPlayerStat, setItemSetCount } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { ItemSet, ItemSlot, Stat } from "../Types";


export default function Session() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  useEffect(() => {
    addRaceStats();
    addAuraStats();
    addItemStats();
    addEnchantStats();
    ($('.tablesorter') as any).tablesorter();

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
      Auras.forEach((aura) => {
        if (playerStore.auras[aura.varName] === true) {
          for (const stat in aura.stats) {
            dispatch(modifyPlayerStat({
              stat: stat as Stat,
              value: aura.stats[stat as Stat]!!,
              action: 'add'
            }));
          }
        }
      })
    }

    function addItemStats() {
      let itemSetCounts: {[key: string]: number} = {};

      for (const [itemSlot, itemId] of Object.entries(playerStore.selectedItems)) {
        if (itemId !== 0 && itemId != null) {
          const itemObj = Items.find(e => e.id === itemId)!!;

          // Add stats from the item
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

          // Increment the set id counter if the item is part of a set
          if (itemObj.setId) {
            itemSetCounts[itemObj.setId.toString()] = itemSetCounts[itemObj.setId.toString()] + 1 || 1;
          }
        }
      }

      // Update the item sets state in the player store
      for (const [setId, count] of Object.entries(itemSetCounts)) {
        dispatch(setItemSetCount({
          setId: setId as ItemSet,
          count: count
        }));
      }
    }

    function addEnchantStats() {
      for (const [itemSlot, enchantId] of Object.entries(playerStore.selectedEnchants)) {
        if (enchantId !== 0 && Object.values(ItemSlot).includes(itemSlot as ItemSlot)) {
          const enchantObj = Enchants.find(e => e.id === enchantId);

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