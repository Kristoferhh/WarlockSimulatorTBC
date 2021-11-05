import json
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.expected_conditions import number_of_windows_to_be, visibility_of_all_elements_located, visibility_of_element_located
from selenium.webdriver.support.ui import WebDriverWait
from time import sleep

with webdriver.Firefox() as driver:
    item_names = {}
    wait = WebDriverWait(driver, 10)
    driver.get('https://kristoferhh.github.io/WarlockSimulatorTBC/')
    item_slot_amount = 18
    WebDriverWait(driver, 30).until(visibility_of_all_elements_located((By.CSS_SELECTOR, "#item-slot-selection-list li p")))
    for i in range(item_slot_amount):
        item_slot_element = driver.find_elements(By.CSS_SELECTOR, "#item-slot-selection-list li p")[i]
        item_slot_element.click()
        sleep(1)
        for item in driver.find_elements(By.CSS_SELECTOR, ".item-row-name"):
            # Get the item name as it is on the website and the wowhead url
            innerHTML = item.get_attribute("innerHTML")
            item_name = innerHTML.split("</a>")[1]
            wowhead_url = innerHTML.split('<a href="')[1].split('"></a>')[0]
            index_to_insert_localizer_string = wowhead_url.find('tbc.')
            wowhead_url = wowhead_url[:index_to_insert_localizer_string] + 'cn.' + wowhead_url[index_to_insert_localizer_string:]
            main_window = driver.current_window_handle
            
            # Open the wowhead page in a new tab, get the localized item name and add it to the item name dictionary
            body = driver.find_element_by_tag_name('body')
            driver.execute_script("window.open('" + wowhead_url + "');")
            wowhead_name_css_selector = ".heading-size-1"
            wait.until(number_of_windows_to_be(2))
            for window_handle in driver.window_handles:
                if window_handle != main_window:
                    driver.switch_to.window(window_handle)
                    break
            localized_item_name = WebDriverWait(driver, 30).until(visibility_of_element_located((By.CSS_SELECTOR, wowhead_name_css_selector))).get_attribute("innerHTML").split('<span')[0]
            item_names[item_name] = localized_item_name
            driver.execute_script("window.close();")
            driver.switch_to.window(main_window)
    with open('export.json', 'w', encoding='utf-8') as f:
        f.write(json.dumps(obj=item_names, indent=4, ensure_ascii=False))