import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import { Store } from './redux/Store';
import { Provider } from 'react-redux';
import '../node_modules/@fortawesome/fontawesome-free/js/all'

ReactDOM.render(
  <React.StrictMode>
    <Provider store={Store}>
      <App />
    </Provider>
  </React.StrictMode>,
  document.getElementById('root')
);
