import { useEffect } from "react";
import { useDispatch, useSelector } from "react-redux";
import { Races } from "./Data/Races";
import { modifyPlayerStat } from "./PlayerSlice";
import { RootState } from "./Store";
import { Stat } from "./Types";


export default function Session() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  useEffect(() => {
    addRaceStats();

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
  }, []);

  return(<div></div>);
}