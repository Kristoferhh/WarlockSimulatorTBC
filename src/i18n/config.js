import i18n from 'i18next';
import { initReactI18next } from 'react-i18next';

i18n.use(initReactI18next).init({
  fallbackLng: 'en',
  lng: localStorage.getItem('language') || 'en',
  resources: {
    zh: {
      translations: require('./locales/zh/translations.json')
    }
  },
  ns: ['translations'],
  defaultNS: 'translations'
});

export default i18n;