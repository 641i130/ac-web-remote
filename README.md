# ac-web-remote
A cheap alternative to the expensive WiFi modules for industrial AC mini-split indoor units. (With script-able scheduling!)

# Parts

- NodeMCU (or ESP8266/ESP32 equivalent board)
- Wire
- PNP 2N2222 or Equivlant Transistor (This helps pulse the IR LEDS correctly)
- 1 or more IR 5mm LEDS (This guide uses 2)
- Breakout PCB

# Wire Diagram

![](https://raw.githubusercontent.com/641i130/ac-web-remote/main/images/smol_fritzing.png)

# Code

This was developed using **PlatformIO**. It makes Arduino programming a lot easier! The interface was updated to use Fahrenheit. If you use Celsius, theres spots to comment out the Fahrenheit and swap it out with Celcius.

# Credits

- [The IRremote Library](https://github.com/crankyoldgit/IRremoteESP8266)

- [The Interface](https://github.com/ael-code/daikin-control)

