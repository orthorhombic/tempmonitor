# tempmonitor
Arduino programs and schematics to construct a temperature monitor with the capability to use either type k thermocouples or inexpensive thermistors.

The main motivation of this is to provide a scalable system for precision temperature monitoring and logging across a wide variety of temperatures and simultaneously visualize the recent temperatures. Coming from a chemistry/materials science background, I envision myself using this for everything from sous vide cooking to firing ceramics to ensure the appropriate dwell time. A bill of materials and useful references are found below.

The contents of this repository are a series of experiments to individually test specific functionality. Not all of them work in their current state. The working components of each can be easily assembled into a fully working program depending on your objectives - I just haven't gotten the time yet. As an example, take the working code for temperature visualization and couple that with the schematics/code for the isolated thermocouple and you are ready to monitor everything from grilling to kiln temperatures. 

**Warning:** Before using any of these electronics near high voltage sources - check your circuit design and the chips for their maximum isolation. These are not yet designed to function around high voltage resistive heating elements or the voltages induced near induction cooktops.


# References

Thermoworks TX-1001X-OP & TX-1003X-AP Probe Steinhart-Hart Coefficients
* Calibration coefficients used to obtain accurate measurements from Thermoworks thermistor probes
* https://tvwbb.com/showthread.php?69233-Thermoworks-TX-1001X-OP-amp-TX-1003X-AP-Probe-Steinhart-Hart-Coefficients
* I have no affiliation with Thermoworks, but I am a proud owner of many of their products and believe them to be of a very high quality

The Cave Pearl Project
 * Great source of information on temperature monitoring/logging with arduino hardware and themistors
 * https://thecavepearlproject.org/

Zach Houser Thermocouple Isolation Schematic (PDF)
 * Used for inspiration
 * http://forum.arduino.cc/index.php?action=dlattach;topic=179760.0;attach=50121
 
MAX31855 Thermocouple Reader Linearization
 * Used to improve the accuracy of temperature measurements
 * https://github.com/heypete/MAX31855-Linearization
 
Cartesian Graph Function
 * Used for Arduino Mega
 * https://forum.arduino.cc/index.php?topic=368023.0
 
 
Parts:
 * ADUM5401 Digikey: ADUM5401WCRWZ-1-ND
  * Critical for the use of thermocouple isolation
  * thermocouple isolation enables the use of a grounded thermocouple with the MAX31855 (or newer) thermocouple sensors
 * DC0805T-10X-ND
 * 1528-1067-ND
 * 1568-1098-ND
 * 296-8744-1-ND
 * 478-8115-1-ND
 * 399-1170-1-ND
 * Banggood Item: 965164 Arduino Mega w/ Screen
  * Screen looks like: https://www.banggood.com/3_5-Inch-TFT-Color-Screen-Module-320-X-480-Support-Arduino-UNO-Mega2560-p-1022298.html 
  * Arduino looks like: https://www.banggood.com/Mega2560-R3-ATmega2560-16AU-Control-Board-With-USB-Cable-For-Arduino-p-73020.html?rmmds=search
 * RTC: https://www.banggood.com/3Pcs-DS3231-AT24C32-IIC-Real-Time-Clock-Module-For-Arduino-p-958134.html
 * DHT: https://www.banggood.com/AM2302-DHT22-Temperature-And-Humidity-Sensor-Module-For-Arduino-SCM-p-937403.html
 * 20x4 Display: https://www.banggood.com/5V-2004-20X4-204-2004A-LCD-Display-Module-Blue-Screen-For-Arduino-p-915064.html
 * Thermocouple Reader: https://www.banggood.com/MAX31855-K-Type-Thermocouple-Breakout-Board-Temperature-Measurement-Module-For-Arduino-p-1086523.html
  * Maxim has since come out with a better and more reliable thermocouple measurement chip: MAX31856
  * Thermocouples do no measure temperature - they measure temperature differences. Therefore, ensure your thermocouple connects to the block and that you have thermal contact between the electrical contacts and chip. If you do not, you will have drift. Consider powering the chip only when needed to minimize self heating.
 * Arduino Nano with expansion board: https://www.banggood.com/Funduino-Nano-Expansion-Board-ATmega328P-Nano-V3-Improved-Version-For-Arduino-p-1011009.html
  