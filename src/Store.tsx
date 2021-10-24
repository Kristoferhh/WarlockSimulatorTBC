import { configureStore } from "@reduxjs/toolkit";
import PlayerReducer from './PlayerSlice';
import UiReducer from './UiSlice';

export const Store = configureStore({
  reducer: {
    player: PlayerReducer,
    ui: UiReducer,
  },
});

export type RootState = ReturnType<typeof Store.getState>;
export type AppDispatch = typeof Store.dispatch;