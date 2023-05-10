/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 *
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 *
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define RST_PIN D3	   // Configurable, see typical pin layout above
#define SS_RFID_PIN D4 // Configurable, see typical pin layout above
#define SS_LCD_PIN D8  // Configurable, see typical pin layout above

// LCD 5110 Software SPI (slower updates, more flexible pin options):
// D5 - Serial clock out (SCLK)
// D7 - Serial data out (DIN)
// D2 - Data/Command select (D/C)
// D8 - LCD chip select (CS)
// D0 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D2, D8, D0);

MFRC522 mfrc522(SS_RFID_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::MIFARE_Key key;

void setup()
{
	Serial.begin(9600); // Initialize serial communications with the PC
	while (!Serial);							   // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();					   // Init SPI bus
	mfrc522.PCD_Init();				   // Init MFRC522
	delay(4);						   // Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
	digitalWrite(SS_RFID_PIN, HIGH);
	digitalWrite(SS_LCD_PIN, LOW);

	display.begin();
	display.setContrast(100);

	display.clearDisplay();
	// text display tests
	display.setRotation(2);
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("\n\nRFID-RC522\nReader\n\n\n");
	Serial.println("\n\nRFID-RC522\nReader\n\n\n");
	display.display();
	delay(2000);

	digitalWrite(SS_LCD_PIN, HIGH);
	digitalWrite(SS_RFID_PIN, LOW);
}

void loop()
{
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent())
	{
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
	{
		return;
	}

	display.clearDisplay();
	// Dump debug info about the card; PICC_HaltA() is automatically called
	display.setCursor(0, 0);
	display.println(F("Card UID:"));

	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		if (mfrc522.uid.uidByte[i] < 0x10)
		{
			display.print(F(" 0"));
			Serial.print(F(" 0"));
		}
		else
		{
			display.print(F(" "));
			Serial.print(F(" "));
		}
		display.print(mfrc522.uid.uidByte[i], HEX);
		Serial.print(mfrc522.uid.uidByte[i], HEX);
	}
	display.println();
	Serial.println();

	// SAK
	display.print(F("Card SAK: "));
	Serial.print(F("Card SAK: "));
	if (mfrc522.uid.sak < 0x10)
	{
		display.print(F("0"));
		Serial.print(F("0"));
	}
	display.println(mfrc522.uid.sak, HEX);
	Serial.println(mfrc522.uid.sak, HEX);

	// (suggested) PICC type
	MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
	display.println(F("PICC type: "));
	Serial.println(F("PICC type: "));
	display.println(mfrc522.PICC_GetTypeName(piccType));
	Serial.println(mfrc522.PICC_GetTypeName(piccType));

	for (byte i = 0; i < 6; i++)
	{
		key.keyByte[i] = 0xFF;
	}

	digitalWrite(SS_LCD_PIN, LOW);
	digitalWrite(SS_RFID_PIN, HIGH);

	display.display();

	delay(2000);

	digitalWrite(SS_LCD_PIN, HIGH);
	digitalWrite(SS_RFID_PIN, LOW);

	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}