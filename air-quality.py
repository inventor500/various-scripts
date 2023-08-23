#! /usr/bin/env python3

"""
Fetch the current air quality.
"""

import json
import datetime
from typing import Union
from enum import Enum
import requests

# Global variables - set these to your location
# LATITUDE: str = "6 decimal latitude"
# LONGITUDE: str = "6 decimal longitude"
# STATE: str ="USPS state abbreviation"
LATITUDE: str = "43.055225" 
LONGITUDE: str = "-88.004085"
STATE: str ="WI"

# To get address data from zip:
# curl 'https://geocode.arcgis.com/arcgis/rest/services/World/GeocodeServer/findAddressCandidates?SingleLine=53213&f=json&outSR=%7B%22wkid%22%3A4326%7D&outFields=PlaceName%2CType%2CCity%2CRegion%2CCountry%2CAddr_type&category=City%2CRegion%2CPostal&countryCode=US&maxLocations=6' \
  # -H 'authority: geocode.arcgis.com' \
  # -H 'accept: */*' \
  # -H 'accept-language: en-US,en;q=0.9' \
  # -H 'dnt: 1' \
  # -H 'origin: https://www.airnow.gov' \
  # -H 'referer: https://www.airnow.gov/' \
  # -H 'sec-gpc: 1' \
  # -H 'user-agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36' \
  # --compressed

Type = Enum('Type', ['CURRENT', 'FORECAST'])

def get_data() -> list[dict[str, Union[str, int]]]:
    """Download the data."""
    endpoint: str = "https://airnowgovapi.com/reportingarea/get"
    headers: dict[str, str] = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; rv:102.0) Gecko/20100101 Firefox/102.0",
        "Origin": "https://www.airnow.gov",
        "DNT": "1", # Do not track header
        # Using a short Python script instead of visiting the website implies you do not want to be tracked
        "Sec-GPC": "1", # Global Privacy Control
        "Pragma": "no-cache" # Deprecated, but used on the website
    }
    data: dict = {
        "latitude": LATITUDE,
        "longitude": LONGITUDE,
        "stateCode": STATE,
        "maxDistance": 50
    }
    return requests.post(endpoint, headers=headers, data=data).json()

def parse_data_entry(data: dict[str, Union[str, int]]) -> dict[str, Union[str, datetime.date, int]]:
    """Put the data in a more useful format."""
    # Date/Time
    new_data: dict[str, Union[str, datetime.date, int]] = {}
    new_data['date'] = datetime.datetime.strptime(data['validDate'], "%m/%d/%y").date() # 'Murica time
    # if 'time' in data and data['time'] != "": # Time is reported only for the current day
    #     new_data['time'] = datetime.datetime.strptime(data['time'], "%H:%M")
    new_data['type'] = Type.CURRENT if data['dataType'] == 'O' else Type.FORECAST
    new_data['parameter'] = data['parameter'][0].upper() + data['parameter'][1:].lower()
    new_data['aqi'] = data['aqi'] if 'aqi' in data else "Unspecified"
    new_data['category'] = data['category']
    new_data['critical'] = 'yes' if data['isActionDay'] else 'no'
    return new_data

def parse_data(data: list[dict[str, Union[str, int]]]) -> tuple[str, str]:
    """Parse the returned JSON data."""
    today: datetime.date = datetime.date.today()
    today_report: str = f"Current Conditions ({datetime.datetime.now().strftime('%Y-%m-%d %H:%M')}):\n"
    forecast: str = "Forecast:\n"
    data_with_times = map(parse_data_entry, data)
    for item in data_with_times:
        if item['type'] == Type.CURRENT: # Only today's items
            today_report += f"{item['parameter']} AQI={item['aqi']} Status=\"{item['category']}\" Critical={item['critical']}\n"
        elif item['type'] == Type.FORECAST:
            forecast += f"Date={item['date'].isoformat()} {item['parameter']} Status=\"{item['category']}\" Critical={item['critical']}\n"
    return today_report, forecast
            

if __name__=='__main__':
    current, forecast = parse_data(get_data())
    print(current)
    print(forecast)

