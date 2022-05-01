import './css/App.css'
import Main from './components/Main'
import Sidebar from './components/Sidebar'
import Session from './components/Session'

export default function App() {
  return (
    <div className='App'>
      <Sidebar />
      <Main />
      <Session />
    </div>
  )
}
