import i18n from 'i18next';
import { initReactI18next } from 'react-i18next';

i18n.use(initReactI18next).init({
  fallbackLng: 'en',
  lng: 'zh',
  resources: {
    en: {
      translations: require('./locales/en/translations.json')
    },
    zh: {
      translations: require('./locales/zh/translations.json')
    }
  },
  ns: ['translations'],
  defaultNS: 'translations'
});

i18n.language = ['en', 'zh'];

export default i18n;