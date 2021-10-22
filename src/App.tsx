import React from 'react';
import './App.css';
import Main from './Components/Main';
import Sidebar from './Components/Sidebar';

export default function App() {
  return (
    <div className="App">
      <Sidebar />
      <Main />
    </div>
  );
}
