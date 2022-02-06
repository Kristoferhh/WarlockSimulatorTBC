import json
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.expected_conditions import number_of_windows_to_be, visibility_of_all_elements_located, visibility_of_element_located
from selenium.webdriver.support.ui import WebDriverWait
from time import sleep


def open_wowhead_url_and_get_name(url, driver):
    driver.execute_script("window.open('" + url + "');")
    wowhead_name_css_selector = ".heading-size-1"
    wait.until(number_of_windows_to_be(2))
    for window_handle in driver.window_handles:
        if window_handle != main_window:
            driver.switch_to.window(window_handle)
            break
    item_name = WebDriverWait(driver, 30).until(visibility_of_element_located(
        (By.CSS_SELECTOR, wowhead_name_css_selector))).get_attribute("innerHTML").split('<span')[0]
    driver.execute_script("window.close();")
    driver.switch_to.window(main_window)
    return item_name


def add_localize_prefix_to_wowhead_url(wowhead_url):
    index_to_insert_localizer_string = wowhead_url.find('tbc.')
    return wowhead_url[:index_to_insert_localizer_string] + 'cn.' + wowhead_url[index_to_insert_localizer_string:]


with webdriver.Firefox() as driver:
    item_names = {}
    wait = WebDriverWait(driver, 10)
    driver.get('https://kristoferhh.github.io/WarlockSimulatorTBC/')
    item_slot_amount = 18
    WebDriverWait(driver, 30).until(visibility_of_all_elements_located(
        (By.CSS_SELECTOR, "#item-slot-selection-list li p")))
    for i in range(item_slot_amount):
        item_slot_element = driver.find_elements(
            By.CSS_SELECTOR, "#item-slot-selection-list li p")[i]
        item_slot_element.click()
        sleep(1)
        for item in driver.find_elements(By.CSS_SELECTOR, ".item-row-name"):
            # Get the item name as it is on the website and the wowhead url
            innerHTML = item.get_attribute("innerHTML")
            item_name = innerHTML.split("</a>")[1]
            wowhead_url = add_localize_prefix_to_wowhead_url(
                innerHTML.split('<a href="')[1].split('"></a>')[0])
            main_window = driver.current_window_handle

            # Name of some random head item with a meta socket and a non-meta socket.
            if item_name == "Hood of the Corruptor":
                # Get all the gem names. Loop 2 sockets, first for the meta gems and 2nd for the other gems
                for i in range(2):
                    item.find_elements_by_xpath(
                        '../td')[2].find_elements(By.CSS_SELECTOR, "div a")[i].click()
                    gems = driver.find_elements(By.CSS_SELECTOR, '.gem-name a')
                    for gem in gems:
                        gem_name = gem.get_attribute('innerHTML')
                        wowhead_url = add_localize_prefix_to_wowhead_url(
                            gem.get_attribute('href'))
                        item_names[gem_name] = open_wowhead_url_and_get_name(
                            wowhead_url, driver)
                    sleep(1)
                driver.find_element(By.ID, 'main').click()
                sleep(1)

            # Open the wowhead page in a new tab, get the localized item name and add it to the item name dictionary
            item_names[item_name] = open_wowhead_url_and_get_name(
                wowhead_url, driver)
    with open('export.json', 'w', encoding='utf-8') as f:
        f.write(json.dumps(obj=item_names, indent=4, ensure_ascii=False))
