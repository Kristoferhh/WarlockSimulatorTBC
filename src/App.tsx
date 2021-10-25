import './App.css';
import Main from './Components/Main';
import Sidebar from './Components/Sidebar';
import Session from './Components/Session';

export default function App() {
  return (
    <div className="App">
      <Sidebar />
      <Main />
      <Session />
    </div>
  );
}
