import { nanoid } from "@reduxjs/toolkit";
import { useSelector } from "react-redux";
import { RootState } from "../redux/Store";

export default function CombatLog() {
  const uiState = useSelector((state: RootState) => state.ui);

  return (
    <div
      id="combat-log"
      style={{ display: uiState.combatLog.visible ? "" : "none" }}
    >
      {uiState.combatLog.data.map((entry) => (
        <p key={nanoid()}>{entry}</p>
      ))}
    </div>
  );
}
