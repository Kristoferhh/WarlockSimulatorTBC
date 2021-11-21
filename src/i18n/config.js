import i18n from 'i18next';
import { initReactI18next } from 'react-i18next';
import { Languages } from '../Types';

i18n.use(initReactI18next).init({
  fallbackLng: 'en',
  lng: localStorage.getItem('language') ||
    (navigator.languages.length > 0 &&
      navigator.languages[0].split('-').length > 0 &&
      Languages.find(e => e.iso === navigator.languages[0].split('-')[0]) ?
      navigator.languages[0].split('-')[0] : 'en'),
  resources: {
    zh: {
      translations: require('./locales/zh/translations.json')
    }
  },
  ns: ['translations'],
  defaultNS: 'translations'
});

export default i18n;