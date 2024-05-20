import asyncio
from bleak import BleakScanner, BleakClient
from gpiozero import RGBLED
import time

led = RGBLED(17, 27, 22) #  Pins 17, 27, and 22 for Red, Green and Blue

garage_door_state = None  # Initialize a variable to store the state

def set_led_color(state):
    if state == "false":
        led.color = (1, 0, 0)  # Red
    elif state == "true":
        led.color = (0, 1, 0)  # Green
    elif state == "error":
        led.color = (0, 0 , 1) # Blue
    else:
        print(f"Unknown state: {state}")  # White
        led.color = (1, 1 , 1)
        
async def handle_notification(sender: int, data: bytearray):
    """Callback function to handle characteristic notifications."""
    global garage_door_state

    garage_door_state = data.decode("ascii").lower()
    print(f"Received notification on characteristic {sender}: {garage_door_state}")
    set_led_color(garage_door_state)

async def main():
    devices = await BleakScanner.discover()
    for d in devices: # Look for device named "GarageStatus"
        if d.name == "GarageStatus":
            print("Found GarageStatus device!")
            
            address = d.address  # Access address directly from Bleak object
            async with BleakClient(address) as client:
                    
                # Find the characteristic with notify property
                for service in client.services:
                    for char in service.characteristics:
                        if char.properties == ['notify']:
                            notify_char = char
                            print(f"Found characteristic for notification: {notify_char}")
                            
                            # Enable notifications for the characteristic
                            await client.start_notify(notify_char, handle_notification)
                            # Wait for notifications (loop forever in this example)
                            while True:
                                await asyncio.sleep(1)  # Adjust wait time as needed
                            # Remember to stop notifications when done
                            # await client.stop_notify(notify_char)
            return  # Exit after finding the device

    print("GarageStatus device not found.")
    led.color = (1, 0 , 0)

asyncio.run(main())
