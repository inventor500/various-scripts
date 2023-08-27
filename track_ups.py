#! /usr/bin/env python3

"""
Get JSON data representing the current status of a UPS package.
Usage: track_ups.py <tracking number>
This script only works intermittently. Excessive use may result in it no longer working.
"""

import sys
from atexit import register
from typing import Optional
from selenium import webdriver
# Work around a Selenium bug in detecting Gecko Driver
from selenium.webdriver.firefox.service import Service
from selenium.webdriver.firefox.options import Options
from selenium.webdriver.firefox.webdriver import WebDriver
from requests import Session # type: ignore
from requests.models import Response # type: ignore

def launch_browser(
        service: Optional[Service] = None
)-> WebDriver:
    """Launch the browser.
    The default service parameter works around a Selenium bug, courtesy of
    <https://aur.archlinux.org/packages/python-selenium>.
    """
    if service is None:
        service = Service(executable_path='/usr/bin/geckodriver', log_output='/dev/null')
        register(service.stop)
    options: Options = Options()
    # Enable Global Privacy Control
    options.set_preference('privacy.globalprivacycontrol.enabled', True)
    options.set_preference('privacy.globalprivacycontrol.functionality.enabled', True)
    options.add_argument("-headless")
    return webdriver.Firefox(service=service, options=options)

def create_session(browser: WebDriver) -> Session:
    """Create a requests sesison."""
    with Session() as session:
        for cookie in browser.get_cookies():
            session.cookies.set(cookie['name'], cookie['value'])
        session.headers.update({
            'referer': 'https://www.ups.com/track?loc=en_US&requester=ST/',
            'user-agent': browser.execute_script("return navigator.userAgent"),
            'X-XSRF-TOKEN': session.cookies.get('X-XSRF-TOKEN-ST'),
            'X-CSRF-TOKEN': session.cookies.get('X-XSRF-TOKEN-ST'),
            'sec-gpc': '1',
            'Accept': 'application/json',
            'Origin': 'https://www.ups.com',
            'sec-fetch-site': 'same-site',
            'sec-fetch-mode': 'cors',
            'sec-fetch-dest': 'empty',
            'authority': 'webapis.ups.com',
            'dnt': '1'})
        return session

def send_request(session: Session, tracking_numbers: list[str]) -> Response:
    """Send a request to the server."""
    return session.post('https://webapis.ups.com/track/api/Track/GetStatus?loc=en_US&requester=ST',
                        json={"Locale": "en_US", "TrackingNumber": tracking_numbers})

def main() -> None:
    """The main function."""
    tracking_numbers: list[str] = sys.argv[1:]
    browser = launch_browser()
    browser.get("https://www.ups.com/track?loc=en_US&requester=ST/")
    assert "Tracking" in browser.title
    # Selenium does not allow for easy manual requests
    request_session: Session = create_session(browser)
    response = send_request(request_session, tracking_numbers)
    print(response.text)
    browser.quit()

if __name__=="__main__":
    main()

