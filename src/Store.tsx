import { configureStore } from "@reduxjs/toolkit";
import PlayerReducer from './PlayerSlice';

export const Store = configureStore({
  reducer: {
    player: PlayerReducer
  },
});

export type RootState = ReturnType<typeof Store.getState>;
export type AppDispatch = typeof Store.dispatch;