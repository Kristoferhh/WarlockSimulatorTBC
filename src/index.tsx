import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import { Store } from './redux/Store';
import { Provider } from 'react-redux';

ReactDOM.render(
  <React.StrictMode>
    <Provider store={Store}>
      <App />
    </Provider>
  </React.StrictMode>,
  document.getElementById('root')
);
