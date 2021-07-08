# RFID_Reader 
Basic ESP8266 RFID reader that display results on a Nokia 5110 Display 


## Pinout used 

</br>


### RF522

| RFID-RF522 | ESP8266  | Description |
| :---       |:---      | :---        |
| SDA (SS)   | D4       | (Slave Select) - the pin on each device that the master can use to enable and disable specific devices. |
| SCK        | D5       | (Serial Clock) - The clock pulses which synchronize data transmission generated by the master |
| MOSI       | D7       | (Master Out Slave In) - The Master line for sending data to the peripherals |
| MISO       | D6       | (Master In Slave Out) - The Slave line for sending data to the master |
| IRQ        | -        | | 
| GND        | G        | |
| RST        | D3       | |
| 3.3V (VCC) | 3V3      | |

 
</br>
</br>
</br>

### LCD Nokia 5110

| LCD 5110   | ESP8266  | Description |
| :---       |:---      | :---        |
| RST        | D0       | This pin resets the module. It an active low pin (resets when 0V is provided)|
| CE (CS/SS) | D8       | This pin is made low (0V) to select this particular display when more than one SPI peripherals are used|
| DC         | D2       | This pin is used to switch between Data mode (high) and Command mode (low) |
| DIN (MOSI) | D7       | This is the input pin (MOSI) through which serial instructions are sent |
| CLK        | D5       | All SPI modules require a common clock, this clock source is supplied to this pin |
| VCC        | 3V3      | This pin is used to power the display the supply voltage is from 2.7V to 3.3V |
| BL         | 3V3 or - | This pin powers the backlight of the display (3.3V maximum) |
| GND        | G        | Connects to the ground of the circuit |

