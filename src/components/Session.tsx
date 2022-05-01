import { useEffect } from "react";
import { useDispatch, useSelector } from "react-redux";
import {
  getAurasStats,
  getBaseStats,
  getEnchantsStats,
  getGemsStats,
  getItemSetCounts,
  getItemsStats,
} from "../Common";
import {
  setAurasStats,
  setBaseStats,
  setEnchantsStats,
  setGemsStats,
  setItemSetCounts,
  setItemsStats,
} from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { Race } from "../Types";

export default function Session() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  useEffect(() => {
    dispatch(setBaseStats(getBaseStats(playerStore.settings.race as Race)));
    dispatch(setAurasStats(getAurasStats(playerStore.auras)));
    dispatch(setItemsStats(getItemsStats(playerStore.selectedItems)));
    dispatch(
      setGemsStats(
        getGemsStats(playerStore.selectedItems, playerStore.selectedGems)
      )
    );
    dispatch(
      setEnchantsStats(
        getEnchantsStats(
          playerStore.selectedItems,
          playerStore.selectedEnchants
        )
      )
    );
    dispatch(setItemSetCounts(getItemSetCounts(playerStore.selectedItems)));
    ($(".tablesorter") as any).tablesorter();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return <div></div>;
}
