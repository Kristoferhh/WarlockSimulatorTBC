import React from 'react';
import './App.css';
import Main from './Components/Main';
import Sidebar from './Components/Sidebar';
import Session from './Session';

export default function App() {
  return (
    <div className="App">
      <Sidebar />
      <Main />
      <Session />
    </div>
  );
}
