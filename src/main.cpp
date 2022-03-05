#include <Arduino.h>
#include <Adafruit_I2CDevice.h>	
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>	// FastLED_NeoMatrix example for single NeoPixel Shield. By Marc MERLIN <marc_soft@merlins.org> Contains code (c) Adafruit, license BSD
#include <FastLED.h>
#include "TeensyTimerTool.h"	// fuer timer / interrupts via library
using namespace TeensyTimerTool;
#include "smileytongue24.h"
// Choose your prefered pixmap
//#include "heart24.h"
//#include "yellowsmiley24.h"
//#include "bluesmiley24.h"


//------ fuer midi-in via library --------
#include <MIDI.h>  // Add Midi Library
//Create an instance of the library with default name, serial port and settings
//midi::SerialMIDI<SerialPort, _Settings>::SerialMIDI [mit SerialPort=HardwareSerial, _Settings=midi::DefaultSerialSettings]
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
//-------------------------------------------


//------- fuer audio-in ----------
// #include <Audio.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>
// // GUItool: begin automatically generated code
// AudioInputAnalog         adc1(A4);       //xy=99,55
// AudioAnalyzeFFT1024      fft;            //xy=265,75
// AudioAnalyzePeak         peak1;          //xy=239,195
// AudioAnalyzeNoteFrequency notefreq1;      //xy=242.00000762939453,301.00000953674316
// AudioConnection          patchCord1(adc1, peak1);
// AudioConnection          patchCord2(adc1, fft);
// AudioConnection          patchCord3(adc1, notefreq1);
// GUItool: end automatically generated code
//-----------------------------------------------------

//-----fuer oled display -----------------
//#include <Wire.h>               // SCL pin 19, SDA pin 18   
// #include <Adafruit_SSD1306.h>   
// Adafruit_SSD1306 display(128, 64, &Wire, -1, 1000000);  // 1MHz I2C clock
//---------------------------------------


//=============================
//#define USELEDMATRIXCONFIG
//=============================
#ifdef USELEDMATRIXCONFIG
	#define LEDMATRIX
	#include "neomatrix_config.h"
#else
	FastLED_NeoMatrix* matrix;
#endif

// https://learn.adafruit.com/adafruit-neopixel-uberguide/neomatrix-library
// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.


//===============================

#define DATA_PIN            9 // auf teensy++2 -> 12 (C2)
#define TEST_PIN_D7         6  // internal LED
#define MIDI_RX_PIN         0  // auf teensy++2 -> 2 (D2)
#define LED1_PIN            14
#define LED2_PIN            15
#define LED3_PIN            16
#define LIPO_PIN            19 // auf teensy++2 -> 40 (F2)
#define SECONDSFORVOLTAGE	1
#define mw					22	// TODO: ausmerzen
#define mh				    23	// TODO: ausmerzen
#define MATRIX_WIDTH        22
#define MATRIX_HEIGHT       23
#define BRIGHTNESS			15 //50    // Max is 255, 32 is a conservative value to not overload a USB power supply (500mA) for 12x12 pixels.

#define MATRIX_TYPE         HORIZONTAL_ZIGZAG_MATRIX
#define MATRIX_SIZE         (MATRIX_WIDTH * MATRIX_HEIGHT)
#define NUMMATRIX			(MATRIX_WIDTH * MATRIX_HEIGHT)	// TODO: ausmerzen
#define NUMPIXELS           MATRIX_SIZE // TODO: ausmerzen
#define COLOR_ORDER         RGB
#define CHIPSET             WS2812B
#define anz_LEDs			278

CRGB leds[NUMMATRIX];
//=============================================

int progWhiteGoingBright_brightness = BRIGHTNESS;

// paths for progOutlinePath
const static int outlinePath1[] = { 30, 31, 29, 28, 27, 26, 36, 42, 43, 44, 45, 46, 25, 9, 8, 0, 1, 2, 4, 3, 16, 17, 56, 57, 91, 92, 101, 102, 111, 112, 121, 122, 162, 193, 229, 230, 262, 263, 274, 275, 276, 277, 270, 269, 254, 239, 240, 241, 242, 243, 244, 253, 252, 251, 250, 249, 211, 210, 176, 177, 178, 179, 175, 161, 152, 151, 142, 141, 132, 131, 77, 72, 73, 74, 75, 76, 37, 31 };
const static int outlinePath2[] = { 32, 33, 34, 35, 41, 71, 70, 69, 68, 67, 47, 24, 10, 7, 6, 5, 14, 15, 18, 55, 58, 90, 93, 100, 103, 110, 113, 120, 123, 163, 192, 194, 228, 231, 261, 264, 273, 272, 271, 268, 255, 238, 220, 219, 218, 217, 216, 215, 245, 246, 247, 248, 212, 209, 208, 207, 180, 174, 160, 153, 150, 143, 140, 133, 130, 77, 72, 73, 74, 38 };
const static int outlinePath3[] = { 39, 40, 72, 77, 78, 79, 80, 81, 66, 48, 23, 11, 12, 13, 19, 54, 59, 89, 94, 99, 104, 109, 114, 119, 124, 164, 191, 195, 227, 232, 260, 265, 266, 267, 256, 237, 221, 202, 203, 204, 205, 206, 215, 214, 213, 181, 173, 159, 154, 149, 144, 139, 134, 129 };
const static int outlinePath4[] = { 81, 82, 65, 49, 22, 21, 20, 53, 60, 88, 95, 98, 105, 108, 115, 118, 125, 164, 191, 195, 227, 233, 258, 257, 236, 222, 201, 202, 183, 172, 158, 155, 148, 145, 138, 135, 128 };
const static int outlinePath5[] = { 82, 65, 49, 50, 51, 61, 87, 96, 97, 106, 107, 116, 117, 126, 165, 190, 196, 226, 234, 235, 236, 222, 201, 184, 171, 157, 156, 147, 146, 137, 136, 127 };
const static int outlinePath6[] = { 82, 65, 64, 63, 62, 87, 96, 97, 106, 107, 116, 117, 126, 165, 190, 196, 225, 224, 223, 222, 201, 184, 171, 157, 156, 147, 146, 137, 136, 127 };
const static int outlinePath7[] = { 82, 83, 84, 85, 86, 96, 97, 106, 107, 116, 117, 126, 165, 190, 197, 198, 199, 200, 185, 171, 157, 156, 147, 146, 137, 136, 127 };
const static int outlinePath8[] = { 82, 83, 84, 85, 86, 96, 97, 106, 107, 116, 117, 126, 165, 189, 188, 187, 186, 185, 171, 157, 156, 147, 146, 137, 136, 127 };
const static int outlinePath9[] = { 82, 83, 84, 85, 86, 96, 97, 106, 107, 116, 117, 126, 166, 167, 168, 169, 170, 157, 156, 147, 146, 137, 136, 127 };

const static boolean DEBUG = false;

byte songID = 0; // 0 -> default loop
 
byte red2;
byte blue2;
int col1;
int col2;

#define green2 		255	//byte green2;
#define center_x 	10	//byte center_x;
#define center_y 	10	//byte center_y;

byte incomingMidiByte;
byte midiStatusByte;
byte midiDataByte1;
byte midiDataByte2;

int adc_value = 0;
float adc_voltage = 0.0;
float in_voltage = 0.0;
float ref_voltage = 3.3;
float R1 = 22000.0;
float R2 = 4700.0;
float voltageSmooth = 0.0;

boolean progStroboIsBlack = false;	// for strobo
byte secondsForVoltage = 0;

volatile unsigned int millisToReduceCPUSpeed = 0;
volatile unsigned int millisCounterTimer = 0;	// wird von den progs fürs timing bzw. delay-ersatz verwendet
volatile unsigned int millisCounterForProgChange = 0;		// achtung!! -> kann nur bis 65.536 zaehlen!!
volatile unsigned int millisCounterForSeconds = 0;
volatile unsigned int nextChangeMillis = 100000;		// start value = 10 sec
volatile boolean flag_processFastLED = false;
volatile boolean flag_update_display = true;	 // TODO: kann wohl wieder raus ...nur fuer oled display
volatile boolean nextChangeMillisAlreadyCalculated = false;
volatile byte nextSongPart = 0;
volatile byte prog = 0;
volatile boolean OneSecondHasPast = false;
//volatile byte ISR_received_USART_byte;
//volatile boolean ISR_USART_got_a_byte = false;

volatile unsigned int LED1_millis = 0;
volatile unsigned int LED2_millis = 0;
volatile unsigned int LED3_millis = 0;
volatile boolean LED1_on = false;
volatile boolean LED2_on = false;
volatile boolean LED3_on = false;

unsigned int lastLEDchange = millis();
int ledState = LOW;             // ledState used to set the LED --TODO: nur test mit interner LED

int zaehler = 0;
int progMatrixZaehler = 0;
int progScrollTextZaehler = MATRIX_WIDTH + 1;
int progScrollEnde;
boolean scannerGoesBack = false;
int stage = 0;

byte actualAnzahlLEDs = 0; // wird benutzt von fastBlinBling fuer die steigerung der anzahl LEDs

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

PeriodicTimer t1;

//===============================================
// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)   
#define LED_GREEN_LOW 		(15 << 5)  
#define LED_GREEN_MEDIUM 	(31 << 5)  
#define LED_GREEN_HIGH 		(63 << 5)  

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

static const uint8_t PROGMEM
mono_bmp[][8] = {
{   // 0: checkered 1
	B10101010,
	B01010101,
	B10101010,
	B01010101,
	B10101010,
	B01010101,
	B10101010,
	B01010101,
		},

{   // 1: checkered 2
	B01010101,
	B10101010,
	B01010101,
	B10101010,
	B01010101,
	B10101010,
	B01010101,
	B10101010,
		},

{   // 2: smiley
	B00111100,
	B01000010,
	B10100101,
	B10000001,
	B10100101,
	B10011001,
	B01000010,
	B00111100 },

{   // 3: neutral
	B00111100,
	B01000010,
	B10100101,
	B10000001,
	B10111101,
	B10000001,
	B01000010,
	B00111100 },

{   // 4; frowny
	B00111100,
	B01000010,
	B10100101,
	B10000001,
	B10011001,
	B10100101,
	B01000010,
	B00111100 },
};

static const uint16_t PROGMEM
// These bitmaps were written for a backend that only supported
// 4 bits per color with Blue/Green/Red ordering while neomatrix
// uses native 565 color mapping as RGB.  
// I'm leaving the arrays as is because it's easier to read
// which color is what when separated on a 4bit boundary
// The demo code will modify the arrays at runtime to be compatible
// with the neomatrix color ordering and bit depth.
RGB_bmp[][64] = {
	// 00: blue, blue/red, red, red/green, green, green/blue, blue, white
	{	
  0x100, 0x200, 0x300, 0x400, 0x600, 0x800, 0xA00, 0xF00,
  0x101, 0x202, 0x303, 0x404, 0x606, 0x808, 0xA0A, 0xF0F,
  0x001, 0x002, 0x003, 0x004, 0x006, 0x008, 0x00A, 0x00F,
  0x011, 0x022, 0x033, 0x044, 0x066, 0x088, 0x0AA, 0x0FF,
  0x010, 0x020, 0x030, 0x040, 0x060, 0x080, 0x0A0, 0x0F0,
  0x110, 0x220, 0x330, 0x440, 0x660, 0x880, 0xAA0, 0xFF0,
  0x100, 0x200, 0x300, 0x400, 0x600, 0x800, 0xA00, 0xF00,
  0x111, 0x222, 0x333, 0x444, 0x666, 0x888, 0xAAA, 0xFFF, },

  // 01: grey to white
  {	0x111, 0x222, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF,
0x222, 0x222, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF,
0x333, 0x333, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF,
0x555, 0x555, 0x555, 0x555, 0x777, 0x999, 0xAAA, 0xFFF,
0x777, 0x777, 0x777, 0x777, 0x777, 0x999, 0xAAA, 0xFFF,
0x999, 0x999, 0x999, 0x999, 0x999, 0x999, 0xAAA, 0xFFF,
0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xFFF,
0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, },

// 02: low red to high red
{	0x001, 0x002, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F,
0x002, 0x002, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F,
0x003, 0x003, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F,
0x005, 0x005, 0x005, 0x005, 0x007, 0x009, 0x00A, 0x00F,
0x007, 0x007, 0x007, 0x007, 0x007, 0x009, 0x00A, 0x00F,
0x009, 0x009, 0x009, 0x009, 0x009, 0x009, 0x00A, 0x00F,
0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00F,
0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, },

// 03: low green to high green
{	0x010, 0x020, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0,
0x020, 0x020, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0,
0x030, 0x030, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0,
0x050, 0x050, 0x050, 0x050, 0x070, 0x090, 0x0A0, 0x0F0,
0x070, 0x070, 0x070, 0x070, 0x070, 0x090, 0x0A0, 0x0F0,
0x090, 0x090, 0x090, 0x090, 0x090, 0x090, 0x0A0, 0x0F0,
0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0F0,
0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, },

// 04: low blue to high blue
{	0x100, 0x200, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00,
0x200, 0x200, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00,
0x300, 0x300, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00,
0x500, 0x500, 0x500, 0x500, 0x700, 0x900, 0xA00, 0xF00,
0x700, 0x700, 0x700, 0x700, 0x700, 0x900, 0xA00, 0xF00,
0x900, 0x900, 0x900, 0x900, 0x900, 0x900, 0xA00, 0xF00,
0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xF00,
0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, },

// 05: 1 black, 2R, 2O, 2G, 1B with 4 blue lines rising right
{	0x000, 0x200, 0x000, 0x400, 0x000, 0x800, 0x000, 0xF00,
  0x000, 0x201, 0x002, 0x403, 0x004, 0x805, 0x006, 0xF07,
0x008, 0x209, 0x00A, 0x40B, 0x00C, 0x80D, 0x00E, 0xF0F,
0x000, 0x211, 0x022, 0x433, 0x044, 0x855, 0x066, 0xF77,
0x088, 0x299, 0x0AA, 0x4BB, 0x0CC, 0x8DD, 0x0EE, 0xFFF,
0x000, 0x210, 0x020, 0x430, 0x040, 0x850, 0x060, 0xF70,
0x080, 0x290, 0x0A0, 0x4B0, 0x0C0, 0x8D0, 0x0E0, 0xFF0,
0x000, 0x200, 0x000, 0x500, 0x000, 0x800, 0x000, 0xF00, },

// 06: 4 lines of increasing red and then green
{ 0x000, 0x000, 0x001, 0x001, 0x002, 0x002, 0x003, 0x003,
0x004, 0x004, 0x005, 0x005, 0x006, 0x006, 0x007, 0x007,
0x008, 0x008, 0x009, 0x009, 0x00A, 0x00A, 0x00B, 0x00B,
0x00C, 0x00C, 0x00D, 0x00D, 0x00E, 0x00E, 0x00F, 0x00F,
0x000, 0x000, 0x010, 0x010, 0x020, 0x020, 0x030, 0x030,
0x040, 0x040, 0x050, 0x050, 0x060, 0x060, 0x070, 0x070,
0x080, 0x080, 0x090, 0x090, 0x0A0, 0x0A0, 0x0B0, 0x0B0,
0x0C0, 0x0C0, 0x0D0, 0x0D0, 0x0E0, 0x0E0, 0x0F0, 0x0F0, },

// 07: 4 lines of increasing red and then blue
{ 0x000, 0x000, 0x001, 0x001, 0x002, 0x002, 0x003, 0x003,
0x004, 0x004, 0x005, 0x005, 0x006, 0x006, 0x007, 0x007,
0x008, 0x008, 0x009, 0x009, 0x00A, 0x00A, 0x00B, 0x00B,
0x00C, 0x00C, 0x00D, 0x00D, 0x00E, 0x00E, 0x00F, 0x00F,
0x000, 0x000, 0x100, 0x100, 0x200, 0x200, 0x300, 0x300,
0x400, 0x400, 0x500, 0x500, 0x600, 0x600, 0x700, 0x700,
0x800, 0x800, 0x900, 0x900, 0xA00, 0xA00, 0xB00, 0xB00,
0xC00, 0xC00, 0xD00, 0xD00, 0xE00, 0xE00, 0xF00, 0xF00, },

// 08: criss cross of green and red with diagonal blue.
{	0xF00, 0x001, 0x003, 0x005, 0x007, 0x00A, 0x00F, 0x000,
0x020, 0xF21, 0x023, 0x025, 0x027, 0x02A, 0x02F, 0x020,
0x040, 0x041, 0xF43, 0x045, 0x047, 0x04A, 0x04F, 0x040,
0x060, 0x061, 0x063, 0xF65, 0x067, 0x06A, 0x06F, 0x060,
0x080, 0x081, 0x083, 0x085, 0xF87, 0x08A, 0x08F, 0x080,
0x0A0, 0x0A1, 0x0A3, 0x0A5, 0x0A7, 0xFAA, 0x0AF, 0x0A0,
0x0F0, 0x0F1, 0x0F3, 0x0F5, 0x0F7, 0x0FA, 0xFFF, 0x0F0,
0x000, 0x001, 0x003, 0x005, 0x007, 0x00A, 0x00F, 0xF00, },

// 09: 2 lines of green, 2 red, 2 orange, 2 green
{ 0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0,
0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, },

// 10: multicolor smiley face
{ 0x000, 0x000, 0x00F, 0x00F, 0x00F, 0x00F, 0x000, 0x000,
0x000, 0x00F, 0x000, 0x000, 0x000, 0x000, 0x00F, 0x000,
0x00F, 0x000, 0xF00, 0x000, 0x000, 0xF00, 0x000, 0x00F,
0x00F, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x00F,
0x00F, 0x000, 0x0F0, 0x000, 0x000, 0x0F0, 0x000, 0x00F,
0x00F, 0x000, 0x000, 0x0F4, 0x0F3, 0x000, 0x000, 0x00F,
0x000, 0x00F, 0x000, 0x000, 0x000, 0x000, 0x00F, 0x000,
0x000, 0x000, 0x00F, 0x00F, 0x00F, 0x00F, 0x000, 0x000, },
};

// Convert a BGR 4/4/4 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap, int16_t w, int16_t h) {
	// work around "a15 cannot be used in asm here" compiler bug when using an array on ESP8266
	// uint16_t RGB_bmp_fixed[w * h];
	static uint16_t* RGB_bmp_fixed = (uint16_t*)malloc(w * h * 2);
	for (uint16_t pixel = 0; pixel < w * h; pixel++) {
		uint8_t r, g, b;
		uint16_t color = pgm_read_word(bitmap + pixel);

		//Serial.print(color, HEX);
		b = (color & 0xF00) >> 8;
		g = (color & 0x0F0) >> 4;
		r = color & 0x00F;
		//Serial.print(" ");
		//Serial.print(b);
		//Serial.print("/");
		//Serial.print(g);
		//Serial.print("/");
		//Serial.print(r);
		//Serial.print(" -> ");
		// expand from 4/4/4 bits per color to 5/6/5
		b = map(b, 0, 15, 0, 31);
		g = map(g, 0, 15, 0, 63);
		r = map(r, 0, 15, 0, 31);
		//Serial.print(r);
		//Serial.print("/");
		//Serial.print(g);
		//Serial.print("/");
		//Serial.print(b);
		RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
		//Serial.print(" -> ");
		//Serial.println(RGB_bmp_fixed[pixel], HEX);
	}
	matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}

//TODO FIXEN!!!!
// In a case of a tile of neomatrices, this test is helpful to make sure that the
// pixels are all in sequence (to check your wiring order and the tile options you
// gave to the constructor).
void count_pixels() {
	matrix->clear();
	for (uint16_t i = 0; i < MATRIX_HEIGHT; i++) {
		for (uint16_t j = 0; j < MATRIX_WIDTH; j++) {
			matrix->drawPixel(j, i, i % 3 == 0 ? (uint16_t)LED_BLUE_HIGH : i % 3 == 1 ? (uint16_t)LED_RED_HIGH : (uint16_t)LED_GREEN_HIGH);
			// depending on the matrix size, it's too slow to display each pixel, so
			// make the scan init faster. This will however be too fast on a small matrix.
			matrix->show();
		}
	}
}

// Fill the screen with multiple levels of white to gauge the quality
void display_four_white() {
	matrix->clear();
	matrix->fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, LED_WHITE_HIGH);
	matrix->drawRect(1, 1, MATRIX_WIDTH - 2, MATRIX_HEIGHT - 2, LED_WHITE_MEDIUM);
	matrix->drawRect(2, 2, MATRIX_WIDTH - 4, MATRIX_HEIGHT - 4, LED_WHITE_LOW);
	matrix->drawRect(3, 3, MATRIX_WIDTH - 6, MATRIX_HEIGHT - 6, LED_WHITE_VERYLOW);
	matrix->show();
}

void display_bitmap(uint8_t bmp_num, uint16_t color) {
	static uint16_t bmx, bmy;
	//bmx = 4;
	//bmy = 4;
	
	// Clear the space under the bitmap that will be drawn as
	// drawing a single color pixmap does not write over pixels
	// that are nul, and leaves the data that was underneath
	matrix->fillRect(bmx, bmy, bmx + 8, bmy + 8, LED_BLACK);
	matrix->drawBitmap(bmx, bmy, mono_bmp[bmp_num], 8, 8, color);
	bmx += 8;
	if (bmx >= mw) bmx = 0;
	if (!bmx) bmy += 8;
	if (bmy >= mh) bmy = 0;
	matrix->show();
}
void progDisplay_bitmap(unsigned int durationMillis, byte nextPart, uint8_t bmp_num, uint16_t color) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		display_bitmap(bmp_num, color);
	}
	//---------------------------------------------------------------------
}

void display_rgbBitmap(uint8_t bmp_num) {
	static uint16_t bmx, bmy;

	fixdrawRGBBitmap(bmx, bmy, RGB_bmp[bmp_num], 8, 8);
	bmx += 8;
	if (bmx >= MATRIX_WIDTH) bmx = 0;
	if (!bmx) bmy += 8;
	if (bmy >= MATRIX_HEIGHT) bmy = 0;
	matrix->show();
}

void display_lines() {
	matrix->clear();

	// 4 levels of crossing red lines.
	matrix->drawLine(0, mh / 2 - 2, mw - 1, 2, LED_RED_VERYLOW);
	matrix->drawLine(0, mh / 2 - 1, mw - 1, 3, LED_RED_LOW);
	matrix->drawLine(0, mh / 2, mw - 1, mh / 2, LED_RED_MEDIUM);
	matrix->drawLine(0, mh / 2 + 1, mw - 1, mh / 2 + 1, LED_RED_HIGH);

	// 4 levels of crossing green lines.
	matrix->drawLine(mw / 2 - 2, 0, mw / 2 - 2, mh - 1, LED_GREEN_VERYLOW);
	matrix->drawLine(mw / 2 - 1, 0, mw / 2 - 1, mh - 1, LED_GREEN_LOW);
	matrix->drawLine(mw / 2 + 0, 0, mw / 2 + 0, mh - 1, LED_GREEN_MEDIUM);
	matrix->drawLine(mw / 2 + 1, 0, mw / 2 + 1, mh - 1, LED_GREEN_HIGH);

	// Diagonal blue line.
	matrix->drawLine(0, 0, mw - 1, mh - 1, LED_BLUE_HIGH);
	matrix->drawLine(0, mh - 1, mw - 1, 0, LED_ORANGE_MEDIUM);
	matrix->show();
}

void display_boxes() {
	matrix->clear();
	matrix->drawRect(0, 0, mw, mh, LED_BLUE_HIGH);
	matrix->drawRect(1, 1, mw - 2, mh - 2, LED_GREEN_MEDIUM);
	matrix->fillRect(2, 2, mw - 4, mh - 4, LED_RED_HIGH);
	matrix->fillRect(3, 3, mw - 6, mh - 6, LED_ORANGE_MEDIUM);
	matrix->show();
}

void display_circles() {
	matrix->clear();
	matrix->drawCircle(MATRIX_WIDTH / 2, MATRIX_HEIGHT / 2, 2, LED_RED_MEDIUM);
	matrix->drawCircle(MATRIX_WIDTH / 2 - 1 - min(MATRIX_WIDTH, MATRIX_HEIGHT) / 8, MATRIX_HEIGHT / 2 - 1 - min(MATRIX_WIDTH, MATRIX_HEIGHT) / 8, min(MATRIX_WIDTH, MATRIX_HEIGHT) / 4, LED_BLUE_HIGH);
	matrix->drawCircle(MATRIX_WIDTH / 2 + 1 + min(MATRIX_WIDTH, MATRIX_HEIGHT) / 8, MATRIX_HEIGHT / 2 + 1 + min(MATRIX_WIDTH, MATRIX_HEIGHT) / 8, min(MATRIX_WIDTH, MATRIX_HEIGHT) / 4 - 1, LED_ORANGE_MEDIUM);
	matrix->drawCircle(1, MATRIX_HEIGHT - 2, 1, LED_GREEN_LOW);
	matrix->drawCircle(MATRIX_WIDTH - 2, 1, 1, LED_GREEN_HIGH);
	if (min(MATRIX_WIDTH, MATRIX_HEIGHT) > 12) matrix->drawCircle(MATRIX_WIDTH / 2 - 1, mh / 2 - 1, min(MATRIX_HEIGHT / 2 - 1, MATRIX_WIDTH / 2 - 1), LED_CYAN_HIGH);
	matrix->show();
}

void display_resolution() {
	matrix->setTextSize(1);
	// not wide enough;
	if (MATRIX_WIDTH < 16) return;
	matrix->clear();
	// Font is 5x7, if display is too small
	// 8 can only display 1 char
	// 16 can almost display 3 chars
	// 24 can display 4 chars
	// 32 can display 5 chars
	matrix->setCursor(0, 0);
	matrix->setTextColor(matrix->Color(255, 0, 0));
	if (MATRIX_WIDTH > 10) matrix->print(MATRIX_WIDTH / 10);
	matrix->setTextColor(matrix->Color(255, 128, 0));
	matrix->print(MATRIX_WIDTH % 10);
	matrix->setTextColor(matrix->Color(0, 255, 0));
	matrix->print('x');
	// not wide enough to print 5 chars, go to next line
	if (MATRIX_WIDTH < 25) {
		if (MATRIX_HEIGHT == 13) matrix->setCursor(6, 7);
		else if (MATRIX_HEIGHT >= 13) {
			matrix->setCursor(MATRIX_WIDTH - 11, 8);
		}
		else {
			// we're not tall enough either, so we wait and display
			// the 2nd value on top.
			matrix->show();
			delay(2000);
			matrix->clear();
			matrix->setCursor(MATRIX_WIDTH - 11, 0);
		}
	}
	matrix->setTextColor(matrix->Color(0, 255, 128));
	matrix->print(MATRIX_HEIGHT / 10);
	matrix->setTextColor(matrix->Color(0, 128, 255));
	matrix->print(MATRIX_HEIGHT % 10);
	// enough room for a 2nd line
	if ((MATRIX_WIDTH > 25 && MATRIX_HEIGHT > 14) || MATRIX_HEIGHT > 16) {
		matrix->setCursor(0, MATRIX_HEIGHT - 7);
		matrix->setTextColor(matrix->Color(0, 255, 255));
		if (MATRIX_WIDTH > 16) matrix->print('*');
		matrix->setTextColor(matrix->Color(255, 0, 0));
		matrix->print('R');
		matrix->setTextColor(matrix->Color(0, 255, 0));
		matrix->print('G');
		matrix->setTextColor(matrix->Color(0, 0, 255));
		matrix->print("B");
		matrix->setTextColor(matrix->Color(255, 255, 0));
		// this one could be displayed off screen, but we don't care :)
		matrix->print("*");

		// We have a big array, great, let's assume 32x32 and add something in the middle
		if (MATRIX_HEIGHT > 24 && MATRIX_WIDTH > 25) {
			for (uint16_t i = 0; i < MATRIX_WIDTH; i += 8) fixdrawRGBBitmap(i, MATRIX_HEIGHT / 2 - 7 + (i % 16) / 8 * 6, RGB_bmp[10], 8, 8);
		}
	}

	matrix->show();
}

void display_scrollText() {
	uint8_t size = max(int(MATRIX_WIDTH / 8), 1);
	matrix->clear();
	matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
	matrix->setTextSize(1);
	matrix->setRotation(0);
	for (int8_t x = 7; x >= -42; x--) {
		yield();
		matrix->clear();
		matrix->setCursor(x, 0);
		matrix->setTextColor(LED_GREEN_HIGH);
		matrix->print("Hello");
		if (MATRIX_HEIGHT > 11) {
			matrix->setCursor(-20 - x, MATRIX_HEIGHT - 7);
			matrix->setTextColor(LED_ORANGE_HIGH);
			matrix->print("World");
		}
		matrix->show();
		delay(50);
	}

	matrix->setRotation(3);
	matrix->setTextSize(size);
	matrix->setTextColor(LED_BLUE_HIGH);
	for (int16_t x = 8 * size; x >= -6 * 8 * size; x--) {
		yield();
		matrix->clear();
		matrix->setCursor(x, MATRIX_WIDTH / 2 - size * 4);
		matrix->print("Rotate");
		matrix->show();
		// note that on a big array the refresh rate from show() will be slow enough that
		// the delay become irrelevant. This is already true on a 32x32 array.
		delay(50 / size);
	}
	matrix->setRotation(0);
	matrix->setCursor(0, 0);
	matrix->show();
}

// Scroll within big bitmap so that all of it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
void display_panOrBounceBitmap(uint8_t bitmapSize) {
	// keep integer math, deal with values 16 times too big
	// start by showing upper left of big bitmap or centering if the display is big
	int16_t xf = max(0, (MATRIX_WIDTH - bitmapSize) / 2) << 4;
	int16_t yf = max(0, (MATRIX_HEIGHT - bitmapSize) / 2) << 4;
	// scroll speed in 1/16th
	int16_t xfc = 6;
	int16_t yfc = 3;
	// scroll down and right by moving upper left corner off screen 
	// more up and left (which means negative numbers)
	int16_t xfdir = -1;
	int16_t yfdir = -1;

	for (uint16_t i = 1; i < 200; i++) {
		bool updDir = false;

		// Get actual x/y by dividing by 16.
		int16_t x = xf >> 4;
		int16_t y = yf >> 4;

		matrix->clear();
		// bounce 8x8 tri color smiley face around the screen
		if (bitmapSize == 8) fixdrawRGBBitmap(x, y, RGB_bmp[10], 8, 8);
		// pan 24x24 pixmap
		if (bitmapSize == 24) matrix->drawRGBBitmap(x, y, (const uint16_t*)bitmap24, bitmapSize, bitmapSize);
#ifdef BM32
		if (bitmapSize == 32) matrix->drawRGBBitmap(x, y, (const uint16_t*)bitmap32, bitmapSize, bitmapSize);
#endif
		matrix->show();

		// Only pan if the display size is smaller than the pixmap
		// but not if the difference is too small or it'll look bad.
		if (bitmapSize - MATRIX_WIDTH > 2) {
			xf += xfc * xfdir;
			if (xf >= 0) { xfdir = -1; updDir = true; };
			// we don't go negative past right corner, go back positive
			if (xf <= ((MATRIX_WIDTH - bitmapSize) << 4)) { xfdir = 1;  updDir = true; };
		}
		if (bitmapSize - MATRIX_HEIGHT > 2) {
			yf += yfc * yfdir;
			// we shouldn't display past left corner, reverse direction.
			if (yf >= 0) { yfdir = -1; updDir = true; };
			if (yf <= ((MATRIX_HEIGHT - bitmapSize) << 4)) { yfdir = 1;  updDir = true; };
		}
		// only bounce a pixmap if it's smaller than the display size
		if (MATRIX_WIDTH > bitmapSize) {
			xf += xfc * xfdir;
			// Deal with bouncing off the 'walls'
			if (xf >= (MATRIX_WIDTH - bitmapSize) << 4) { xfdir = -1; updDir = true; };
			if (xf <= 0) { xfdir = 1; updDir = true; };
		}
		if (MATRIX_HEIGHT > bitmapSize) {
			yf += yfc * yfdir;
			if (yf >= (MATRIX_HEIGHT - bitmapSize) << 4) { yfdir = -1; updDir = true; };
			if (yf <= 0) { yfdir = 1; updDir = true; };
		}

		if (updDir) {
			// Add -1, 0 or 1 but bind result to 1 to 1.
			// Let's take 3 is a minimum speed, otherwise it's too slow.
			xfc = constrain(xfc + random(-1, 2), 3, 16);
			yfc = constrain(xfc + random(-1, 2), 3, 16);
		}
		delay(10);
	}
}


//====================================================== war frueher mal in LEDMatrix.h
 //========== Andres remapping function =================
 //======================================================
const int MISSING_LED = 300;
uint16_t myRemapFn(uint16_t x, uint16_t y) {

    switch (y) {
    case 0:
        switch (x) {
        case 4: return 2;
        case 5: return 1;
        case 6: return 0;
        default: return MISSING_LED;
        }
        break;
    case 1:
        switch (x) {
        case 2: return 3;
        case 3: return 4;
        case 4: return 5;
        case 5: return 6;
        case 6: return 7;
        case 7: return 8;
        default: return MISSING_LED;
        }
        break;
    case 2:
        switch (x) {
        case 1: return 16;
        case 2: return 15;
        case 3: return 14;
        case 4: return 13;
        case 5: return 12;
        case 6: return 11;
        case 7: return 10;
        case 8: return 9;
        case 16: return 26;
        case 17: return 27;
        case 18: return 28;
        case 19: return 29;
        default: return MISSING_LED;
        }
        break;
    case 3:
        switch (x) {
        case 1: return 17;
        case 2: return 18;
        case 3: return 19;
        case 4: return 20;
        case 5: return 21;
        case 6: return 22;
        case 7: return 23;
        case 8: return 24;
        case 9: return 25;
        case 15: return 36;
        case 16: return 35;
        case 17: return 34;
        case 18: return 33;
        case 19: return 32;
        case 20: return 31;
        case 21: return 30;
        default: return MISSING_LED;
        }
        break;
    case 4:
        switch (x) {
        case 0: return 56;
        case 1: return 55;
        case 2: return 54;
        case 3: return 53;
        case 4: return 52;
        case 5: return 51;
        case 6: return 50;
        case 7: return 49;
        case 8: return 48;
        case 9: return 47;
        case 10: return 46;
        case 11: return 45;
        case 12: return 44;
        case 13: return 43;
        case 14: return 42;
        case 15: return 41;
        case 16: return 40;
        case 17: return 39;
        case 18: return 38;
        case 19: return 37;
        default: return MISSING_LED;
        }
        break;
    case 5:
        switch (x) {
        case 0: return 57;
        case 1: return 58;
        case 2: return 59;
        case 3: return 60;
        case 4: return 61;
        case 5: return 62;
        case 6: return 63;
        case 7: return 64;
        case 8: return 65;
        case 9: return 66;
        case 10: return 67;
        case 11: return 68;
        case 12: return 69;
        case 13: return 70;
        case 14: return 71;
        case 15: return 72;
        case 16: return 73;
        case 17: return 74;
        case 18: return 75;
        case 19: return 76;
        default: return MISSING_LED;
        }
        break;
    case 6:
        switch (x) {
        case 0: return 91;
        case 1: return 90;
        case 2: return 89;
        case 3: return 88;
        case 4: return 87;
        case 5: return 86;
        case 6: return 85;
        case 7: return 84;
        case 8: return 83;
        case 9: return 82;
        case 10: return 81;
        case 11: return 80;
        case 12: return 79;
        case 13: return 78;
        case 14: return 77;
        default: return MISSING_LED;
        }
        break;
    case 7:
        switch (x) {
        case 0: return 92;
        case 1: return 93;
        case 2: return 94;
        case 3: return 95;
        case 4: return 96;
        case 10: return 127;
        case 11: return 128;
        case 12: return 129;
        case 13: return 130;
        case 14: return 131;
        default: return MISSING_LED;
        }
        break;
    case 8:
        switch (x) {
        case 0: return 101;
        case 1: return 100;
        case 2: return 99;
        case 3: return 98;
        case 4: return 97;
        case 10: return 136;
        case 11: return 135;
        case 12: return 134;
        case 13: return 133;
        case 14: return 132;
        default: return MISSING_LED;
        }
        break;
    case 9:
        switch (x) {
        case 0: return 102;
        case 1: return 103;
        case 2: return 104;
        case 3: return 105;
        case 4: return 106;
        case 10: return 137;
        case 11: return 138;
        case 12: return 139;
        case 13: return 140;
        case 14: return 141;
        default: return MISSING_LED;
        }
        break;
    case 10:
        switch (x) {
        case 0: return 111;
        case 1: return 110;
        case 2: return 109;
        case 3: return 108;
        case 4: return 107;
        case 10: return 146;
        case 11: return 145;
        case 12: return 144;
        case 13: return 143;
        case 14: return 142;
        default: return MISSING_LED;
        }
        break;
    case 11:
        switch (x) {
        case 0: return 112;
        case 1: return 113;
        case 2: return 114;
        case 3: return 115;
        case 4: return 116;
        case 10: return 147;
        case 11: return 148;
        case 12: return 149;
        case 13: return 150;
        case 14: return 151;
        default: return MISSING_LED;
        }
        break;
    case 12:
        switch (x) {
        case 0: return 121;
        case 1: return 120;
        case 2: return 119;
        case 3: return 118;
        case 4: return 117;
        case 10: return 156;
        case 11: return 155;
        case 12: return 154;
        case 13: return 153;
        case 14: return 152;
        default: return MISSING_LED;
        }
        break;
    case 13:
        switch (x) {
        case 0: return 122;
        case 1: return 123;
        case 2: return 124;
        case 3: return 125;
        case 4: return 126;
        case 10: return 157;
        case 11: return 158;
        case 12: return 159;
        case 13: return 160;
        case 14: return 161;
        default: return MISSING_LED;
        }
        break;
    case 14:
        switch (x) {
        case 1: return 162;
        case 2: return 163;
        case 3: return 164;
        case 4: return 165;
        case 5: return 166;
        case 6: return 167;
        case 7: return 168;
        case 8: return 169;
        case 9: return 170;
        case 10: return 171;
        case 11: return 172;
        case 12: return 173;
        case 13: return 174;
        case 14: return 175;
        default: return MISSING_LED;
        }
        break;
    case 15:
        switch (x) {
        case 2: return 192;
        case 3: return 191;
        case 4: return 190;
        case 5: return 189;
        case 6: return 188;
        case 7: return 187;
        case 8: return 186;
        case 9: return 185;
        case 10: return 184;
        case 11: return 183;
        case 12: return 182;
        case 13: return 181;
        case 14: return 180;
        case 15: return 179;
        case 16: return 178;
        case 17: return 177;
        case 18: return 176;
        default: return MISSING_LED;
        }
        break;
    case 16:
        switch (x) {
        case 1: return 193;
        case 2: return 194;
        case 3: return 195;
        case 4: return 196;
        case 5: return 197;
        case 6: return 198;
        case 7: return 199;
        case 8: return 200;
        case 9: return 201;
        case 10: return 202;
        case 11: return 203;
        case 12: return 204;
        case 13: return 205;
        case 14: return 206;
        case 15: return 207;
        case 16: return 208;
        case 17: return 209;
        case 18: return 210;
        default: return MISSING_LED;
        }
        break;
    case 17:
        switch (x) {
        case 1: return 229;
        case 2: return 228;
        case 3: return 227;
        case 4: return 226;
        case 5: return 225;
        case 6: return 224;
        case 7: return 223;
        case 8: return 222;
        case 9: return 221;
        case 10: return 220;
        case 11: return 219;
        case 12: return 218;
        case 13: return 217;
        case 14: return 216;
        case 15: return 215;
        case 16: return 214;
        case 17: return 213;
        case 18: return 212;
        case 19: return 211;
        default: return MISSING_LED;
        }
        break;
    case 18:
        switch (x) {
        case 1: return 230;
        case 2: return 231;
        case 3: return 232;
        case 4: return 233;
        case 5: return 234;
        case 6: return 235;
        case 7: return 236;
        case 8: return 237;
        case 9: return 238;
        case 10: return 239;
        case 11: return 240;
        case 12: return 241;
        case 13: return 242;
        case 14: return 243;
        case 15: return 244;
        case 16: return 245;
        case 17: return 246;
        case 18: return 247;
        case 19: return 248;
        case 20: return 249;
        default: return MISSING_LED;
        }
        break;
    case 19:
        switch (x) {
        case 1: return 262;
        case 2: return 261;
        case 3: return 260;
        case 4: return 259;
        case 5: return 258;
        case 6: return 257;
        case 7: return 256;
        case 8: return 255;
        case 9: return 254;
        case 16: return 253;
        case 17: return 252;
        case 18: return 251;
        case 19: return 250;
        default: return MISSING_LED;
        }
        break;
    case 20:
        switch (x) {
        case 2: return 263;
        case 3: return 264;
        case 4: return 265;
        case 5: return 266;
        case 6: return 267;
        case 7: return 268;
        case 8: return 269;
        default: return MISSING_LED;
        }
        break;
    case 21:
        switch (x) {
        case 3: return 274;
        case 4: return 273;
        case 5: return 272;
        case 6: return 271;
        case 7: return 270;
        default: return MISSING_LED;
        }
        break;
    case 22:
        switch (x) {
        case 4: return 275;
        case 5: return 276;
        case 6: return 277;
        default: return MISSING_LED;
        }
        break;
    }
    return MISSING_LED;	// not neccessary but to avoid error
}
//==========================


//=====================================================================
//=========== HELPER FUNCTIONS ========================================
//=====================================================================

int getRandomColorValue() {	// dies erzeugt einen random-farb-anteil rot, grün oder blau
    int farbZahl = random(1, 4);
    int farbe = 0;
    switch (farbZahl) {
    case 1:
        farbe = 0; // 0
        break;
    case 2:
        farbe = 127; // 127
        break;
    case 3:
        farbe = 255;
        break;
    }
    return farbe;
}

byte r = getRandomColorValue();
byte g = getRandomColorValue();
byte b = getRandomColorValue();

// TODO: random 1 ... bis x => einschliesslich x oder exkl.?????

//#define LED_RED_HIGH
//#define LED_GREEN_HIGH
//#define LED_BLUE_HIGH
//#define LED_ORANGE_HIGH
//#define LED_PURPLE_HIGH
//#define LED_CYAN_HIGH
//#define LED_WHITE_HIGH
int getRandomColor() { // dies erzeugt einen random color wert für die indexed colors:
	int farbZahl = random(1, 7);
	int farbe = LED_BLACK;
	switch (farbZahl) {
	case 1:
		farbe = LED_WHITE_HIGH;
		break;
	case 2:
		farbe = LED_GREEN_HIGH;
		break;
	case 3:
		farbe = LED_BLUE_HIGH;
		break;
	case 4:
		farbe = LED_ORANGE_HIGH;
		break;
	case 5:
		farbe = LED_PURPLE_HIGH;
		break;
	case 6:
		farbe = LED_CYAN_HIGH;
		break;
	case 7:
		farbe = LED_RED_HIGH;
		break;
	}
	return farbe;
}

int getRandomColorIncludingBlack() {
	int farbZahl = random(1, 9);
	int farbe = LED_BLACK;
	switch (farbZahl) {
	case 1:
		farbe = LED_RED_HIGH;
		break;
	case 2:
		farbe = LED_GREEN_HIGH;
		break;
	case 3:
		farbe = LED_BLUE_HIGH;
		break;
	case 4:
		farbe = LED_ORANGE_HIGH;
		break;
	case 5:
		farbe = LED_PURPLE_HIGH;
		break;
	case 6:
		farbe = LED_CYAN_HIGH;
		break;
	case 7:
		farbe = LED_WHITE_HIGH;
		break;
	case 8:
		farbe = LED_BLACK;
		break;
	}
	return farbe;
}

// wird zB fuer ProgDisplayRGB benutzt
void setDurationAndNextPart(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
	}
	//---------------------------------------------------------------------
}

//==================================================================
//=========== FX programs ==========================================
//==================================================================
// TODO:
// snake, tetris, pong, pac man, scrolltext -> FastLED GFX zum laufen bekommen!!
// myRemapping table  von hier aus an die lib übergeben!

//--- progBlingBlingColoring -----
int progBlingBlingColoring_rounds = 0;
// leds werden zufällig mit der selben farbe eingeschaltet und einige wenige zufällig ausgeschaltet
// alle x sekunden wird die eine der drei farbkomponenten zufällig geändert
void progBlingBlingColoring(unsigned int durationMillis, byte nextPart, unsigned int msForColorChange, unsigned int msToReduceSpeed) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		
		progBlingBlingColoring_rounds = 0;

		//millisCounterTimer = del;
	}
	//---------------------------------------------------------------------

	if (millisToReduceCPUSpeed >= msToReduceSpeed) {	// ersatz für delay()
		millisToReduceCPUSpeed = 0;

		if (progBlingBlingColoring_rounds == 0) {
			r = getRandomColorValue();
			g = getRandomColorValue();
			b = getRandomColorValue();
		}

		//set random pixel to defined color
		leds[random(0, anz_LEDs)] = CRGB(r, g, b);	
		// delete 1 pixel sometimes
		if (random(0, 3) == 1) leds[random(0, anz_LEDs)] = CRGB::Black;

		FastLED.show();
	}

	// after DEL ms seconds change 1 part of the color randomly
	if (millisCounterTimer >= msForColorChange) {	//15000 // ersatz für delay()
		millisCounterTimer = 0;
		progBlingBlingColoring_rounds++;
		if (progBlingBlingColoring_rounds == 4) progBlingBlingColoring_rounds = 1;

		//randomSeed(analogRead(4));	// damit werden die pseudo zufallszahlen neu "gemischt" // analogread(4) erzeugt irgendeinen wert mit dem dann der zeiger in der random tabelle neu positioniert wird

		if (progBlingBlingColoring_rounds == 1) b = getRandomColorValue();
		else if (progBlingBlingColoring_rounds == 2) g = getRandomColorValue();
		else if (progBlingBlingColoring_rounds == 3) r = getRandomColorValue();
	}
}
void progBlingBlingColoring(unsigned int durationMillis, byte nextPart, unsigned int msForColorChange) {
	progBlingBlingColoring(durationMillis, nextPart, msForColorChange, 20);
}

void progFastBlingBling(unsigned int durationMillis, byte anzahl, byte nextPart, byte addLEDs, byte maxLEDs, unsigned int delayForAddingLEDs) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		//FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)9.65f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);

		actualAnzahlLEDs = anzahl;
	}
	//---------------------------------------------------------------------

	// anzahl LEDs ggf. erhoehen
	if (millisCounterTimer >= delayForAddingLEDs) {	//15000 // ersatz für delay()
		millisCounterTimer = 0;
		if (addLEDs > 0) {
			if (actualAnzahlLEDs + addLEDs <= maxLEDs) {
				actualAnzahlLEDs = actualAnzahlLEDs + addLEDs;
			}
		}
	}

	//---- jetzt LEDs ausgeben
	FastLED.clear();
	FastLED.setBrightness(255);
	//set random pixel to defined color
	for (int i = 0; i < actualAnzahlLEDs; i++) {
		leds[random(0, anz_LEDs)] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue()); //LED_RED_HIGH;
	}
	FastLED.show();
}

void progFastBlingBling(unsigned int durationMillis, byte anzahl, byte nextPart) {
	progFastBlingBling(durationMillis, anzahl, nextPart, 0, 0, 0);
}

void progFullColors(unsigned int durationMillis, byte nextPart, unsigned int del) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);

		millisCounterTimer = del; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= del) {	// ersatz für delay()
		millisCounterTimer = 0;

		r = getRandomColorValue();
		g = getRandomColorValue();
		b = getRandomColorValue();

		if (r == 0 && g == 0 && b == 0) {
			r = getRandomColorValue();
			g = getRandomColorValue();
			b = getRandomColorValue();
		}
		FastLED.showColor(CRGB(r, g, b));
	}
}

// only for ampere testing
void progWhiteGoingBright(unsigned int durationMillis, byte nextPart, unsigned int del) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		
		millisCounterTimer = del; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	//if (millis() - lastTimestamp > del) {
	if (millisCounterTimer >= del) {	// ersatz für delay()
		millisCounterTimer = 0;

		progWhiteGoingBright_brightness = progWhiteGoingBright_brightness + 5;
		if (progWhiteGoingBright_brightness > 255) progWhiteGoingBright_brightness = BRIGHTNESS;

		FastLED.setBrightness(progWhiteGoingBright_brightness);

		FastLED.showColor(CRGB(255, 255, 255));
		//lastTimestamp = millis();	// restart timer
	}
}

// TODO. scheint mir mit den 5000 millis buggy zu sein!?
void progFullColorsWithFading(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)2.25f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
	
		//millisCounterTimer = del; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	//if (millis() - lastTimestamp > 5000) {
	if (millisCounterTimer >= 5000) {	// ersatz für delay()
		millisCounterTimer = 0;

		byte lastRed = r;
		byte lastGreen = g;
		byte lastBlue = b;

		r = getRandomColorValue();
		g = getRandomColorValue();
		b = getRandomColorValue();

		int diff_r = r - lastRed;
		int diff_g = g - lastGreen;
		int diff_b = b - lastBlue;

		for (int i = 0; i < abs(diff_r); i++) {
			if (diff_r < 0) FastLED.showColor(CRGB(lastRed - i, lastGreen, lastBlue));
			else FastLED.showColor(CRGB(lastRed + i, lastGreen, lastBlue));
		}
		for (int i = 0; i < abs(diff_g); i++) {
			if (diff_g < 0) FastLED.showColor(CRGB(r, lastGreen - i, lastBlue));
			else FastLED.showColor(CRGB(r, lastGreen + i, lastBlue));
		}
		for (int i = 0; i < abs(diff_b); i++) {
			if (diff_b < 0) FastLED.showColor(CRGB(r, g, lastBlue - i));
			else FastLED.showColor(CRGB(r, g, lastBlue + i));
		}

		FastLED.showColor(CRGB(r, g, b));
		//lastTimestamp = millis();	// restart timer
	}
}

void progStrobo(unsigned int durationMillis, byte nextPart, unsigned int del, int red, int green, int blue) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.3f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		millisCounterTimer = del; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	//if (millis() - lastTimestamp > del) {
	if (millisCounterTimer >= del) {	// ersatz für delay()
		millisCounterTimer = 0;

		//--- switch color ---
		if (progStroboIsBlack) {
			FastLED.showColor(CRGB(red, green, blue));
			progStroboIsBlack = false;
		}
		else {
			FastLED.showColor(CRGB::Black);
			progStroboIsBlack = true;
		}
	}
}

void progMatrixScanner(unsigned int durationMillis, byte nextPart, unsigned int reduceSpeed) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)3.95f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
		
		millisToReduceCPUSpeed = 0;
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= reduceSpeed) {	// ersatz für delay()
		millisCounterTimer = 0;

			FastLED.clear();

			if (!scannerGoesBack) {

				zaehler++;
				if (zaehler >= 28) scannerGoesBack = true;

				matrix->drawLine(zaehler + 0, 0, zaehler + 0, MATRIX_HEIGHT, LED_RED_HIGH);
				matrix->drawLine(zaehler - 1, 0, zaehler - 1, MATRIX_HEIGHT, CRGB::White);
				matrix->drawLine(zaehler - 2, 0, zaehler - 2, MATRIX_HEIGHT, LED_RED_HIGH);
			}
			else {
				zaehler--;
				if (zaehler <= -6) scannerGoesBack = false;

				matrix->drawLine(zaehler + 0, 0, zaehler + 0, MATRIX_HEIGHT, CRGB::White);
				matrix->drawLine(zaehler - 1, 0, zaehler - 1, MATRIX_HEIGHT, LED_RED_HIGH);
				matrix->drawLine(zaehler - 2, 0, zaehler - 2, MATRIX_HEIGHT, CRGB::White);
			}
		FastLED.show();
	}
}
void progMatrixScanner(unsigned int durationMillis, byte nextPart) {
	progMatrixScanner(durationMillis, nextPart, 0);
}

void progStern(unsigned int durationMillis, unsigned int msForColorChange, unsigned char nextPart, unsigned char reduceSpeed) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)5.85f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		//--- init.:
		col1 = getRandomColor();
		col2 = getRandomColor();
	}
	//---------------------------------------------------------------------

	// change color every x seconds
	if (msForColorChange > 0) {
		if (millisCounterTimer >= msForColorChange) {	// ersatz für delay()
			millisCounterTimer = 0;
			col1 = getRandomColor();
			col2 = getRandomColor();
		}
	}
	//-------------------------------------

	if (millisToReduceCPUSpeed > reduceSpeed) {
		millisToReduceCPUSpeed = 0;

		zaehler++;
		if (zaehler >= 10) zaehler = 0;

		FastLED.clear();

		matrix->drawLine(center_x - zaehler, 0, center_x + zaehler, 22, col1);
		matrix->drawLine(center_x - zaehler + 1, 0, center_x + zaehler + 1, 22, col2);
		matrix->drawLine(0, zaehler + 1, 21, 22 - zaehler, col1);
		matrix->drawLine(0, zaehler, 21, 21 - zaehler, col2);
		matrix->drawLine(0, center_y + zaehler + 1, 21, center_y - zaehler + 1, col1);
		matrix->drawLine(0, center_y + zaehler, 21, center_y - zaehler, col2);
		matrix->drawLine(zaehler, 22, 22 - zaehler, 0, col1);
		matrix->drawLine(zaehler - 1, 22, 21 - zaehler, 0, col2);

		FastLED.show();
	}
}
void progStern(unsigned int durationMillis, unsigned char nextPart, unsigned char reduceSpeed) {
	progStern(durationMillis, 0, nextPart, reduceSpeed);
}
void progStern(unsigned int durationMillis, unsigned char nextPart) {
	progStern(durationMillis, 0, nextPart, 0);
}

void progBlack(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
	}
	//---------------------------------------------------------------------

	// FastLED.clear();	// hier nicht nötig
	// FastLED.show();
}

void progCircles(unsigned int durationMillis, byte nextPart, unsigned int msForChange, boolean clearEach) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		millisCounterTimer = msForChange; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= msForChange) {	// ersatz für delay()
		millisCounterTimer = 0;

		if (clearEach) {
			FastLED.clear();
			col1 = getRandomColor();
		}
		else {
			col1 = getRandomColorIncludingBlack();	// if not cleared -> black ist also an option :)
		}

		matrix->fillCircle(random(0, 21), random(0, 22), random(3, 10), col1);
		FastLED.show();
	}
}
void progCircles(unsigned int durationMillis, byte nextPart, unsigned int msForChange) {
	progCircles(durationMillis, nextPart, msForChange, true);
}

void progRandomLines(unsigned int durationMillis, byte nextPart, unsigned int msForChange, boolean clearEach) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.05f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		millisCounterTimer = msForChange; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= msForChange) {	// ersatz für delay()
		millisCounterTimer = 0;

		byte x1 = random(0, 21);
		byte x2 = random(0, 21);	

		if (clearEach) {
			FastLED.clear();
			col1 = getRandomColor();
		}
		else {
			col1 = getRandomColorIncludingBlack();	// if not cleared -> black ist also an option :)
		}

		matrix->drawLine(x1 - 1, 0, x2 - 1, 22, col1);
		matrix->drawLine(x1, 0, x2, 22, col1);
		matrix->drawLine(x1 + 1, 0, x2 + 1, 22, col1);
		FastLED.show();
	}
}
void progRandomLines(unsigned int durationMillis, byte nextPart, unsigned int msForChange) {
	progRandomLines(durationMillis, nextPart, msForChange, true);
}

void progMovingLines(unsigned int durationMillis, byte nextPart, unsigned int reduceSpeed) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)9.1f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
	}
	//---------------------------------------------------------------------

	if (millisToReduceCPUSpeed > reduceSpeed) {
		millisToReduceCPUSpeed = 0;
		FastLED.clear();

		switch (stage) {
			case 0:
				zaehler++;
				if (zaehler >= 26) {
					stage = 1;
					zaehler = 0;
					break;
				}
				matrix->drawLine(zaehler, 0, 25 - zaehler, 22, getRandomColor());
				break;

			case 1:
				zaehler++;
				if (zaehler >= 12) {
					stage = 2;
					zaehler = 12;
					break;
				}
				matrix->drawLine(25, zaehler, 0, 22 - zaehler, getRandomColor());
				break;

			case 2:
				zaehler--;
				if (zaehler <= 0) {
					stage = 3;
					zaehler = 25;
					break;
				}
				matrix->drawLine(25, zaehler, 0, 22 - zaehler, getRandomColor());
				break;

			case 3:
				zaehler--;
				if (zaehler <= 0) {
					stage = 4;
					zaehler = 0;
					break;
				}
				matrix->drawLine(zaehler, 0, 25 - zaehler, 22, getRandomColor());
				break;

			case 4:
				zaehler++;
				if (zaehler >= 11) {
					stage = 5;
					zaehler = 10;
					break;
				}
				matrix->drawLine(0, zaehler, 25, 22 - zaehler, getRandomColor());
				break;

			case 5:
				zaehler--;
				if (zaehler <= 0) {
					stage = 0;
					zaehler = 0;
					break;
				}
				matrix->drawLine(0, zaehler, 25, 22 - zaehler, getRandomColor());
				break;
		}

		FastLED.show();
	}
}
void progMovingLines(unsigned int durationMillis, byte nextPart) {
	progMovingLines(durationMillis, nextPart, 0);
}

void progOutline(unsigned int durationMillis, byte nextPart, unsigned int reduceSpeed) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)2.15f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
	}
	//---------------------------------------------------------------------

	if (millisToReduceCPUSpeed > reduceSpeed) {
		millisToReduceCPUSpeed = 0;

		int anz;
		FastLED.clear();

		if (!scannerGoesBack) {

			//for (int y = 0; y < 9; y++) {

			switch (zaehler) {
			case 0:
				anz = (sizeof(outlinePath1) / sizeof(outlinePath1[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath1[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 1:
				anz = (sizeof(outlinePath2) / sizeof(outlinePath2[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath2[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 2:
				anz = (sizeof(outlinePath3) / sizeof(outlinePath3[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath3[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 3:
				anz = (sizeof(outlinePath4) / sizeof(outlinePath4[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath4[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 4:
				anz = (sizeof(outlinePath5) / sizeof(outlinePath5[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath5[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 5:
				anz = (sizeof(outlinePath6) / sizeof(outlinePath6[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath6[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 6:
				anz = (sizeof(outlinePath7) / sizeof(outlinePath7[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath7[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 7:
				anz = (sizeof(outlinePath8) / sizeof(outlinePath8[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath8[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			case 8:
				anz = (sizeof(outlinePath9) / sizeof(outlinePath9[0]));
				for (int i = 0; i < anz; i++) {
					int test = outlinePath9[i];
					leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
				}
				break;
			}
			FastLED.show();

			zaehler++;
			if (zaehler >= 9) scannerGoesBack = true;
		}

		else {

		//for (int y = 8; y > -1; y--) {

		switch (zaehler) {
		case 0:
			anz = (sizeof(outlinePath1) / sizeof(outlinePath1[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath1[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 1:
			anz = (sizeof(outlinePath2) / sizeof(outlinePath2[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath2[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 2:
			anz = (sizeof(outlinePath3) / sizeof(outlinePath3[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath3[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 3:
			anz = (sizeof(outlinePath4) / sizeof(outlinePath4[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath4[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 4:
			anz = (sizeof(outlinePath5) / sizeof(outlinePath5[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath5[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 5:
			anz = (sizeof(outlinePath6) / sizeof(outlinePath6[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath6[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 6:
			anz = (sizeof(outlinePath7) / sizeof(outlinePath7[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath7[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 7:
			anz = (sizeof(outlinePath8) / sizeof(outlinePath8[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath8[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		case 8:
			anz = (sizeof(outlinePath9) / sizeof(outlinePath9[0]));
			for (int i = 0; i < anz; i++) {
				int test = outlinePath9[i];
				leds[test] = CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
			}
			break;
		}
		FastLED.show();

		zaehler--;
		if (zaehler <= 0) scannerGoesBack = false;
	}
	}
}
void progOutline(unsigned int durationMillis, byte nextPart) {
	progOutline(durationMillis, nextPart, 0);
}

//TODO: fixen
void progRunningPixel(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
	}
	//---------------------------------------------------------------------

	int last_x = -1;
	int last_y = -1;
	FastLED.setBrightness(5); // TODO: zurueck auf 155
	FastLED.clear();

 	for (int y = 0; y < MATRIX_HEIGHT; y++) {
		for (int x = 0; x < MATRIX_WIDTH; x++) {
            //leds.DrawLine(x, y, x, y, CRGB(getRandomColorValue(), getRandomColorValue(), getRandomColorValue()));
            //leds.DrawLine(last_x, last_y, last_x, last_y, CRGB::Black);
            matrix->drawLine(x, y, x, y, LED_RED_HIGH);
            matrix->drawLine(last_x, last_y, last_x, last_y, matrix->Color(0, 0, 0));
            last_x = x;
			last_y = y;
            matrix->show();
		}
	} 
}

void progShowText(String words, unsigned int durationMillis, int pos_x, int pos_y, int col, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		millisCounterTimer = 100; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= 100) {	// ersatz für delay()
		millisCounterTimer = 0;
		FastLED.setBrightness(5); // TODO: zurueck auf BRIGHTNESS?

		//uint8_t size = max(int(mw / 8), 1);
		matrix->clear();
		matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
		matrix->setTextSize(1);
		matrix->setRotation(0);

		yield();
		matrix->clear();
		matrix->setCursor(pos_x, pos_y);
		matrix->setTextColor(col);
		matrix->print(words);
		matrix->show();
	}
}

void progScrollText(String words, unsigned int durationMillis, int delay, int col, byte nextPart) {

    //--- standard-part um dauer und naechstes programm zu speichern ----
    if (!nextChangeMillisAlreadyCalculated) {
        FastLED.clear(true);
        // workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
        //nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
        nextChangeMillis = durationMillis;
        nextSongPart = nextPart;
        nextChangeMillisAlreadyCalculated = true;

		millisCounterTimer = delay; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!

		//--- init. :
		progScrollTextZaehler = MATRIX_WIDTH - 2;
		progScrollEnde = words.length() * 6;
    }
    //---------------------------------------------------------------------
	
	if (millisCounterTimer >= delay) {	// ersatz für delay()
		millisCounterTimer = 0;
		FastLED.setBrightness(5); // TODO: zurueck auf BRIGHTNESS?

		//uint8_t size = max(int(mw / 8), 1);
		matrix->clear();
		matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
		matrix->setTextSize(1);
		matrix->setRotation(0);

		progScrollTextZaehler--;
		if (progScrollTextZaehler < -progScrollEnde) progScrollTextZaehler = MATRIX_WIDTH - 2;

		yield();
		matrix->clear();
		matrix->setCursor(progScrollTextZaehler, 13);
		matrix->setTextColor(col);
		matrix->print(words);
		matrix->show();
	}
}

void progShowROOTS(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= 500) {	// ersatz für delay()
		millisCounterTimer = 0;
		FastLED.setBrightness(15); // TODO: zurueck auf BRIGHTNESS?

		//uint8_t size = max(int(mw / 8), 1);
		matrix->clear();
		matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
		matrix->setTextSize(1);
		matrix->setRotation(0);

		yield();
		matrix->clear();

		matrix->setCursor(0, 3);
		matrix->setTextColor(getRandomColor());
		matrix->print("R");

		matrix->setCursor(0, 8);
		matrix->setTextColor(getRandomColor());
		matrix->print("o");

		matrix->setCursor(4, 13);
		matrix->setTextColor(getRandomColor());
		matrix->print("o");

		matrix->setCursor(10, 14);
		matrix->setTextColor(getRandomColor());
		matrix->print("T");

		matrix->setCursor(15, 13);
		matrix->setTextColor(getRandomColor());
		matrix->print("s");

		matrix->show();
	}
}

int zaehlerWortArray;
void progWordArray(String words[], int anzWords, int msPerWord, unsigned int durationMillis, int col, byte nextPart) {
// hier kommen alle wörter einzeln nacheinander

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		zaehlerWortArray = 0;

		millisCounterTimer = msPerWord; // workaround, damit beim ersten durchlauf immer sofort LEDs aktiviert werden und nicht erst nachdem del abgelaufen ist!
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= msPerWord) {	// ersatz für delay()
		millisCounterTimer = 0;

		if (zaehlerWortArray <= anzWords) {
			FastLED.setBrightness(5); // TODO: zurueck auf 155

			matrix->clear();
			matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
			matrix->setTextSize(1);
			matrix->setRotation(0);

			yield();
			matrix->setCursor(2, 13);
			matrix->setTextColor(col);
			matrix->print(words[zaehlerWortArray]);
			matrix->show();

			//Serial.println(zaehlerWortArray);
			zaehlerWortArray++; // naechstes Wort
		}
	}
}

// TODO: progBlinkText
void progBlinkText(String words, unsigned int durationMillis, int col, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)1.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
		//		Serial.println(nextChangeMillis);
	}
	//---------------------------------------------------------------------

	//int last_x = -1;
	//int last_y = -1;
	FastLED.setBrightness(5); // TODO: zurueck auf 155

	//uint8_t size = max(int(mw / 8), 1);
	matrix->clear();
	matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
	matrix->setTextSize(1);
	matrix->setRotation(0);
	for (int8_t x = 23; x >= -90; x--) {
		yield();
		matrix->clear();
		matrix->setCursor(x, 13);
		matrix->setTextColor(col);
		matrix->print(words);
		matrix->show();
		delay(60);
	}
}

// TODO Fixen
void progFadeOut(unsigned int durationMillis, byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		//FastLED.clear();
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		nextChangeMillis = durationMillis;
		//nextChangeMillis = round((float)durationMillis / (float)9.0f);	// TODO: diesen wert eurieren und anpassen!!
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		r = getRandomColorValue();
		g = getRandomColorValue();
		b = getRandomColorValue();
		FastLED.showColor(CRGB(r, g, b)); 
		//FastLED.showColor(getRandomColor());
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= 100) {	// ersatz für delay()
		millisCounterTimer = 0;

		// delete 1 pixel sometimes
		//if (random(0, 3) == 1) leds[random(0, anz_LEDs)] = CRGB::Black;

		FastLED.show();
	}
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
	for (int i = 0; i < 16; i++) {
		currentPalette[i] = CHSV(random8(), 255, random8());
	}
}
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
	// 'black out' all 16 palette entries...
	fill_solid(currentPalette, 16, CRGB::Black);
	// and set every fourth one to white.
	currentPalette[0] = CRGB::White;
	currentPalette[4] = CRGB::White;
	currentPalette[8] = CRGB::White;
	currentPalette[12] = CRGB::White;

}
// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
	CRGB purple = CHSV(HUE_PURPLE, 255, 255);
	CRGB green = CHSV(HUE_GREEN, 255, 255);
	CRGB black = CRGB::Black;

	currentPalette = CRGBPalette16(
		green, green, black, black,
		purple, purple, black, black,
		green, green, black, black,
		purple, purple, black, black);
}
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p =
{
	CRGB::Red,
	CRGB::Gray, // 'white' is too bright compared to red and blue
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Red,
	CRGB::Gray,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Blue,
	CRGB::Black,
	CRGB::Black
};

/// HSV Rainbow
//extern const TProgmemRGBPalette16 MatrixColors_p PROGMEM =
//{
//	0xe6f2e6, 0xcce6cc, 0xb3d9b3, 0x99cc99,
//	0x80c080, 0x66b366, 0x4da64d, 0x339933,
//	0x198d19, 0x008000, 0x008000, 0x008000,
//	0x007000, 0x005000, 0x003000, 0x001000
//};
extern const TProgmemRGBPalette16 MatrixColors_p PROGMEM =
{
	0x001000, 0x003000, 0x005000, 0x007000,
	0x008000, 0x008000, 0x008000, 0x198d19,
	0x339933, 0x4da64d, 0x66b366, 0x80c080,
	0x99cc99, 0xb3d9b3, 0xcce6cc, 0xe6f2e6
};

//DEFINE_GRADIENT_PALETTE( white_to_green_to_black_p ) {
//	0, 230, 242, 230	/* at index 0, black(0,0,0) */
//		192, 0, 128, 0    /* at index 192, red(255,0,0) */
//		255, 0, 0, 0;			/* at index 255, white(255,255,255) */
//};

void FillLEDsFromPaletteColors(uint8_t colorInd) {

	//0 rainbow slow
	//1 rainbow fast (ohne fades)
	//2 rainbow fast (mit fades)
	//3 lila/grün Fast mit fades
	//4 blau/lila/rot/orange mit fades Fast
	//5 white fast ohne fades
	//6 white fast mit fades
	//7 blau/weiss slow mit fades
	//8 blau/lila/rot/orange mit fades slow
	//9 weiss/blau/beige fast ohne fades (interessante farben)
	//10 weiss/blau/beige fast mit fades (interessante farben)
	//11 weiss/grün fast mit fades

	uint8_t brightness = 255;

	for (int i = 0; i < anz_LEDs; i++) {
		leds[i] = ColorFromPalette(currentPalette, colorInd, brightness, currentBlending);
		colorInd += 3;
	}
}
void progPalette(unsigned int durationMillis, uint8_t paletteID, byte nextPart) {

//0 rainbow slow
//1 rainbow fast (ohne fades)
//2 rainbow fast (mit fades)
//3 lila/grün Fast mit fades
//4 blau/lila/rot/orange mit fades Fast
//5 white fast ohne fades
//6 white fast mit fades
//7 blau/weiss slow mit fades
//8 blau/lila/rot/orange mit fades slow
//9 weiss/blau/beige fast ohne fades (interessante farben)
//10 weiss/blau/beige fast mit fades (interessante farben)
//11 weiss/grün fast mit fades

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)5.85f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		// setup palette/Programm
		switch (paletteID) {
		case 0:
			currentPalette = RainbowColors_p;
			currentBlending = LINEARBLEND;
			break;
		case 1:
			currentPalette = RainbowStripeColors_p;   
			currentBlending = NOBLEND;
			break;
		case 2:
			currentPalette = RainbowStripeColors_p;   
			currentBlending = LINEARBLEND;
			break;
		case 3:
			SetupPurpleAndGreenPalette();   
			currentBlending = LINEARBLEND;
			break;
		case 4:
			SetupTotallyRandomPalette();   
			currentBlending = LINEARBLEND;
			break;
		case 5:
			SetupBlackAndWhiteStripedPalette();       
			currentBlending = NOBLEND;
			break;
		case 6:
			SetupBlackAndWhiteStripedPalette();       
			currentBlending = LINEARBLEND;
			break;
		case 7:
			currentPalette = CloudColors_p;           
			currentBlending = LINEARBLEND;
			break;
		case 8:
			currentPalette = PartyColors_p;           
			currentBlending = LINEARBLEND;
			break;
		case 9:
			currentPalette = myRedWhiteBluePalette_p; 
			currentBlending = NOBLEND;
			break;
		case 10:
			currentPalette = myRedWhiteBluePalette_p; 
			currentBlending = LINEARBLEND;
			break;
		case 11:
			currentPalette = MatrixColors_p;
			currentBlending = LINEARBLEND;
			break;
		}
	}
	//---------------------------------------------------------------------

	zaehler++;
	if (zaehler > 1000) zaehler = 0;
	FillLEDsFromPaletteColors(zaehler);

    FastLED.show();
}

extern const TProgmemRGBPalette16 matrixColors FL_PROGMEM =
{
	CRGB::LightGreen,
	CRGB::LightGreen,
	CRGB::LightGreen,
	CRGB::LightGreen,

	CRGB::Green,
	CRGB::Green,
	CRGB::Green,
	CRGB::Green,

	CRGB::LimeGreen,
	CRGB::LimeGreen,
	CRGB::LimeGreen,
	CRGB::LimeGreen,

	CRGB::DarkGreen,
	CRGB::DarkGreen,
	CRGB::DarkGreen,
	CRGB::DarkGreen
};

CRGB getMatrixColor(int index) {
	CRGB col = CRGB(0, 0, 0);
	switch (index) {
	case 0:
		return col = CRGB(0, 0, 0);
		break;
	case 1:
		return col = CRGB(1, 25, 1);
		break;
	case 2:
		return col = CRGB(1, 25, 1);
		break;
	case 3:
		return col = CRGB(1, 40, 1);
		break;
	case 4:
		return col = CRGB(1, 80, 1);
		break;
	case 5:
		return col = CRGB(1, 120, 1);
		break;
	case 6:
		return col = CRGB(1, 150, 1);
		break;
	case 7:
		return col = CRGB(1, 200, 1);
		break;
	case 8:
		return col = CRGB(5, 255, 5);
		break;
	case 9:
		return col = CRGB(10, 180, 10);
		break;
	case 10:
		return col = CRGB(10, 160, 10);
		break;
	case 11:
		return col = CRGB(20, 140, 20);
		break;
	case 12:
		return col = CRGB(30, 120, 30);
		break;
	case 13:
		return col = CRGB(50, 100, 50);
		break;
	case 14:
		return col = CRGB(100, 150, 100);
		break;
	case 15:
		return col = CRGB(180, 180, 180);
		break;
	}
	return col;
}

void progMatrixHorizontal(unsigned int durationMillis, byte nextPart, unsigned int reduceSpeed) {
	
	int colorIndex, offset, row, i;

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)5.85f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		zaehler = 0;
		progMatrixZaehler = 27;
		millisCounterTimer = 100;
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= reduceSpeed) {	// ersatz für delay()
		millisCounterTimer = 0;

		row = 0;
		offset = 0;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 2;
		offset = -20;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 4;
		offset = -15;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 6;
		offset = -8;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 8;
		offset = 0;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 10;
		offset = -14;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 12;
		offset = -21;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 14;
		offset = -9;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 16;
		offset = -1;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 18;
		offset = -16;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 20;
		offset = -23;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 22;
		offset = -11;
		colorIndex = 16;
		for (i = zaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}
		//--------------------------------------

		row = 1;
		offset = 0;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 3;
		offset = -20;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 5;
		offset = -15;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 7;
		offset = -8;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 9;
		offset = 0;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 11;
		offset = -14;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 13;
		offset = -21;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 15;
		offset = -9;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 17;
		offset = -1;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 19;
		offset = -16;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}

		row = 21;
		offset = -23;
		colorIndex = 16;
		for (i = progMatrixZaehler + offset; i > -1 + offset; i--) {
			colorIndex--;
			if (colorIndex < 2) colorIndex = 0;
			matrix->drawPixel(row, i, getMatrixColor(colorIndex));
		}
		//--------------------------

		FastLED.show();

		zaehler++;
		if (zaehler > 56) {
			zaehler = 0;
		}

		progMatrixZaehler++;
		if (progMatrixZaehler > 56) {
			progMatrixZaehler = 0;
		}
	}	
}
void progMatrixHorizontal(unsigned int durationMillis, byte nextPart) {
	progMatrixHorizontal(durationMillis, nextPart, 100);
}

void progMatrixVertical(unsigned int durationMillis, byte nextPart, unsigned int reduceSpeed) {

	int colorIndex, offset, row, i;
	const int colorIndexMin = 2;

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		FastLED.clear(true);
		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
		//nextChangeMillis = round((float)durationMillis / (float)5.85f);	// TODO: diesen wert eurieren und anpassen!!
		nextChangeMillis = durationMillis;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;

		zaehler = 0;
		progMatrixZaehler = 25; // (rand() % (40 + 1 - 15) + 15);//25;
		millisCounterTimer = 100;
	}
	//---------------------------------------------------------------------

	if (millisCounterTimer >= reduceSpeed) {	// ersatz für delay()
		millisCounterTimer = 0;

		// offsets müssen hier + sein!!

		row = 0;
		offset = 0;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 2;
		offset = 15;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 4;
		offset = 7;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 6;
		offset = 20;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 8;
		offset = 11;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 10;
		offset = 4;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 12;
		offset = 13;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 14;
		offset = 23;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 16;
		offset = 7;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 18;
		offset = 19;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 20;
		offset = 11;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 22;
		offset = 5;
		colorIndex = 16;
		for (i = 23 - zaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}
		////--------------------------------------

		row = 1;
		offset = 17;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 3;
		offset = 3;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 5;
		offset = 11;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 7;
		offset = 9;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 9;
		offset = 6;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 11;
		offset = 19;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 13;
		offset = 6;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 15;
		offset = 0;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 17;
		offset = 8;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 19;
		offset = 14;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 21;
		offset = 5;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		row = 23;
		offset = 20;
		colorIndex = 16;
		for (i = 23 - progMatrixZaehler + offset; i < 23 + offset; i++) {
			colorIndex--;
			if (colorIndex < 0) colorIndex = 0;
			matrix->drawPixel(i, row, getMatrixColor(colorIndex));
		}

		//--------------------------

		FastLED.show();

		zaehler++;
		if (zaehler > 60) {
			zaehler = 0; // (rand() % (4 + 1 - 0) + 0); // 0;
		}

		progMatrixZaehler++;
		if (progMatrixZaehler > 60) {
			progMatrixZaehler = 0; // (rand() % (4 + 1 - 0) + 0); // 0;
		}
	}
}
void progMatrixVertical(unsigned int durationMillis, byte nextPart) {
	progMatrixVertical(durationMillis, nextPart, 100);
}

void progGoTo(byte nextPart) {

	//--- standard-part um dauer und naechstes programm zu speichern ----
	if (!nextChangeMillisAlreadyCalculated) {
		nextChangeMillis = 0;
		nextSongPart = nextPart;
		nextChangeMillisAlreadyCalculated = true;
	}
}

// TODO: Fix progCLED
//=== progCLED =====================
uint8_t progCLED_hue;
int16_t progCLED_counter;
//void progCLED(unsigned int durationMillis, byte nextPart) {
//
//	//--- standard-part um dauer und naechstes programm zu speichern ----
//	if (!nextChangeMillisAlreadyCalculated) {
//		FastLED.clear(true);
//		// workaround: die eigentlichen millis werden korrigiert auf die faktische dauer
//		//nextChangeMillis = round((float)durationMillis / (float)2.25f);	// TODO: diesen wert eurieren und anpassen!!
//		nextChangeMillis = durationMillis;
//		nextSongPart = nextPart;
//		nextChangeMillisAlreadyCalculated = true;
//		//		Serial.println(nextChangeMillis);
//	}
//	//---------------------------------------------------------------------
//
//	int16_t x, y;
//	uint8_t h;
//	FastLED.clear();
//
//	h = progCLED_hue;
//	if (progCLED_counter < 1125) {
//		// ** Fill LED's with diagonal stripes
//		for (x = 0; x < (leds.Width() + leds.Height()); ++x)
//		{
//			matrix->DrawLine(x - leds.Height(), leds.Height() - 1, x, 0, CHSV(h, 255, 255));
//			h += 16;
//		}
//	}
//	else {
//		// ** Fill LED's with horizontal stripes
//		for (y = 0; y < leds.Height(); ++y)
//		{
//			leds.DrawLine(0, y, leds.Width() - 1, y, CHSV(h, 255, 255));
//			h += 16;
//		}
//	}
//	progCLED_hue += 4;
//
//	if (progCLED_counter < 375) leds.HorizontalMirror();
//	else if (progCLED_counter < 625) leds.VerticalMirror();
//	else if (progCLED_counter < 875) leds.QuadrantMirror();
//	else if (progCLED_counter < 1125) leds.QuadrantRotateMirror();
//	else if (progCLED_counter < 1500) leds.TriangleTopMirror();
//	else if (progCLED_counter < 1750) leds.TriangleBottomMirror();
//	else if (progCLED_counter < 2000) leds.QuadrantTopTriangleMirror();
//	else if (progCLED_counter < 2250) leds.QuadrantBottomTriangleMirror();
//
//	progCLED_counter++;
//	if (progCLED_counter >= 2250) progCLED_counter = 0;
//
//	FastLED.show();
//}

//==========================================================================

//void switchToSongAndPart(byte song, byte part) {
//    //--- start song ----
//    songID = song;
//    Serial.print("switched to song: ");
//	Serial.println(song);
//	switchToPart(part);
//}

void switchToPart(byte part) {

	prog = part;
	nextChangeMillisAlreadyCalculated = false;	// bool wieder fuer naechstes programm freigeben
	millisCounterTimer = 0;
	millisCounterForProgChange = 0;
	zaehler = 0;	// globalen zaehler auf null
	progScrollTextZaehler = MATRIX_WIDTH + 1;

	//--- initializeValues ---
	progBlingBlingColoring_rounds = 0;
	progCLED_hue = 0;
	progCLED_counter = 0;

	// if (DEBUG) {
	// 	Serial.print("switched to part: ");
	// 	Serial.println(part);
	// }
}

void switchToSong(byte song) {

	if (DEBUG) {
		Serial.println("-------------------");
		Serial.print("switched to song: ");
		Serial.println(song);
		Serial.println("-------------------");
	}
	//--- start song ----
	songID = song;
	switchToPart(0);
}

// MidiDatenAuswerten is the function that will be called by the Midi Library
// when a Continuous Controller message is received.
// It will be passed bytes for Channel, Controller Number, and Value
// It checks if the controller number is within the 22 to 27 range
void MidiDatenAuswerten(byte channel, byte number, byte value) {

	if (DEBUG) {
		Serial.print(channel);
		Serial.print("\t");
		Serial.print(number);
		Serial.print("\t");
		Serial.println(value);
	}

	// with midi byte 22 the song can be changed!
	if (number == 22 && value > 0) {
		if (DEBUG) {
			Serial.print("midi command to switch to song: ");
			Serial.println(value);
		}

		// digitalWrite(LED3_PIN, HIGH);
		// LED3_on = true;
		// LED3_millis = millis();

		switchToSong(value);
	}
	// with midi byte 23 the songpart can be changed!
	else if (number == 23 && value >= 0) {
		if (DEBUG) {
			Serial.print("midi command to switch to part: ");
			Serial.println(value);
		}

		// digitalWrite(LED3_PIN, HIGH);
		// LED3_on = true;
		// LED3_millis = millis();

		switchToPart(value);
	}
}

// void checkIncomingMIDI() {	// 21.01.22 TODO: umstellen auf interrupt!!
// 	boolean debug = false;
// 	do {
// 		if (Serial1.available()) {
// 			incomingMidiByte = Serial1.read(); //read first byte
// 			if (debug) Serial.println(incomingMidiByte);
// 			if (incomingMidiByte != 255) {	// wir filtern eigenartige 255er eingänge raus
// 				if (incomingMidiByte > 127) {	// es wurde ein statusbyte erkannt
// 					midiStatusByte = incomingMidiByte;
// 					midiDataByte1 = 0;	// neues Statusbyte => lösche alte datenbytes
// 					midiDataByte2 = 0;  // neues Statusbyte => lösche alte datenbytes
// 					if (debug) Serial.println(midiStatusByte);
// 				}
// 				else if (incomingMidiByte < 128) {	// es wurde ein datenbyte erkannt
// 					if (midiDataByte1 == 0) midiDataByte1 = incomingMidiByte;
// 					else if (midiDataByte2 == 0) midiDataByte2 = incomingMidiByte;
// 					if (debug) {
// 						Serial.print(midiStatusByte);
// 						Serial.print("\t");
// 						Serial.print(midiDataByte1);
// 						Serial.print("\t");
// 						Serial.println(midiDataByte2);
// 					}
// 					// with midi byte 22 the song can be changed!
// 					if (midiStatusByte == 185 && midiDataByte1 == 22 && midiDataByte2 > 0) {
// 						switchToSong(midiDataByte2);
// 						if (debug) {
// 							Serial.print("switch to song: ");
// 							Serial.println(midiDataByte2);
// 						}
// 					}
// 					// with midi byte 23 the songpart can be changed!
// 					else if (midiStatusByte == 185 && midiDataByte1 == 23 && midiDataByte2 >= 0) {
// 						switchToPart(midiDataByte2);
// 						if (debug) {
// 							Serial.print("switch to part: ");
// 							Serial.println(midiDataByte2);
// 						}
// 					}
// 					// hier war die idee sond und part gleichzeitig zu aendern
// 					//else if (midiStatusByte == 185 && midiDataByte1 > 30 && midiDataByte1 < 64 && midiDataByte2 >= 0) {
// 					//	midiDataByte1 = midiDataByte1 - 30;
// 					//	switchToSongAndPart(midiDataByte1, midiDataByte2);
// 					//	if (debug) {
// 					//		Serial.print("switched to song: ");
// 					//		Serial.print(midiDataByte1);
// 					//		Serial.print(" / part: ");
// 					//		Serial.println(midiDataByte2);
// 					//	}
// 					//}
// 				}
// 			}
// 		}
// 	} while (Serial1.available());
// }

// void checkIncomingMIDITEST() {
// 	//Serial.println("check");
// 	do {
// 		if (Serial1.available()) {
// 			incomingMidiByte = Serial1.read(); //read first byte
// 			Serial.println(incomingMidiByte);
// 			Serial.println("check");
// 		}
// 	} while (Serial1.available());
// }
//====================================================

const static char wordFeels[] = { "Feels" };
const static char wordLike[] = { "like" };
const static char wordI[] = { "i" };
const static char wordAm[] = { "am" };
const static char wordJust[] = { "just" };
const static char wordToo[] = { "too" };
const static char wordClose[] = { "close" };
const static char wordTo[] = { "to" };
const static char wordLove[] = { "love" };
const static char wordYou[] = { "you!" };
String wordArrTooCLose2[] = { wordFeels, wordLike, wordI, wordAm, wordJust, wordToo, wordClose, wordTo, wordLove, wordYou }; // ,'\0'
//==============================================

const static char castle_w1[] = { "i am" };
const static char castle_w2[] = { "headed" };
const static char castle_w3[] = { "straight" };
const static char castle_w4[] = { "for" };
const static char castle_w5[] = { "the" };
const static char castle_w6[] = { "castle" };
String wordArrCastle[] = { castle_w1, castle_w2, castle_w3, castle_w4, castle_w5, castle_w6 };
//==============================================

 
//void defaultLoop() {
//	//FastLED.setBrightness(BRIGHTNESS); // zur sicherheit in jedem loop neu auf default setzen. ggf. kann einzelner fx das überschreiben
//
//	switch (prog) {
//
//	case 0:
//		//progOutline(500000, 100);
//		//progMovingLines(50000, 100);
//		//progWhiteGoingBright(10000000, 100, 2000); // only for ampere testing
//		//progFullColors(3000047, 100, 10000);
//		progMatrixScanner(50000, 100);
//		//progBlingBlingColoring(2000, 100);//3    59,5hz
//        //progRunningPixel(100000, 100);
//        //progFastBlingBling(200000, 100); //20s -> 3:13
//		break;
//
//	case 100:
//		FastLED.clear();
//		switchToSong(0);	// SongID 0 == DEFAULT loop
//		break;
//	}
//}


//#0
void defaultLoop()  {

 	switch (prog) { 

	case 0:
		progScrollText("Nerds on Fire", 19500, 90, getRandomColor(), 5);

		//progBlingBlingColoring(60000, 5, 5000);
		//progFastBlingBling(60000, 1, 5, 1, 15, 2000);		 
		//progFastBlingBling(60000, 1, 5); //20s -> 3:13
		//progFullColors(60000, 5, 2000);
		//progWhiteGoingBright(60000, 5, 5000);
		//progStrobo(60000, 5, 45, 255, 255, 255); // Weisser strobo
		//progMatrixScanner(60000, 5, 25);
		//progStern(60000, 900, 5, 15);	
		//progCircles(60000, 5, 1000, true);
		//progRandomLines(60000, 5, 500, false);
		//progMovingLines(60000, 5, 10);
		//progOutline(60000, 50, 40);
		// TODO FIXEN //progRunningPixel(60000, 5);
		//count_pixels();	// TODO FIXEN
		//progMatrixHorizontal(60000, 5, 70);
		//progMatrixVertical(60000, 5, 80);

		//display_rgbBitmap(5); // cool: 5, 8, 9, 10
		//progShowROOTS(60000, 1);
		//progShowText("ROOTS", 60000, 1, 13, getRandomColor(), 1);
		//progScrollText("Pokerface by Lady Gaga", 60000, 60, getRandomColor(), 1);
		//progScrollText("Phil", 60000, 30, getRandomColor(), 1);
		//progPalette(60000, 11, 5);	// paletteID -> 0 - 11	// SCHNELL!
			//0 rainbow slow
			//1 rainbow fast (ohne fades)
			//2 rainbow fast (mit fades)
			//3 lila/grün Fast mit fades
			//4 blau/lila/rot/orange mit fades Fast
			//5 white fast ohne fades
			//6 white fast mit fades
			//7 blau/weiss slow mit fades
			//8 blau/lila/rot/orange mit fades slow
			//9 weiss/blau/beige fast ohne fades (interessante farben)
			//10 weiss/blau/beige fast mit fades (interessante farben)
			//11 weiss/grün fast mit fades
		//progFadeOut(16615, 20);
		//progWordArray(wordArrTooCLose2, 10, 570, 5714, getRandomColor(), 5);
		//progScrollText("Nerds on Fire", 10000, getRandomColor(), 4);
		//display_panOrBounceBitmap(8);	// 8: smiley panning around
		//display_bitmap(4, getRandomColor());
		//display_rgbBitmap(10); // cool: 5, 8, 9, 10
		break;

	case 5: // TODO: FIXEN am anfang doppelstreifen!?
		progMatrixHorizontal(30000, 10, 70);
		break;

	case 10: // TODO: FIXEN: bleibt manchmal einfach stehen?
		progStern(15000, 15, 15);
		break;

	case 15: // OK
		//progStrobo(5000, 20, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		progBlingBlingColoring(10000, 20, 500);
		//progCLED(10000, 4);	// matrix colors
		break;

	case 20: // OK
		progMatrixScanner(15000, 25);
		break;

	case 25: // OK
		progFullColors(15000, 35, 2000);
		break;

	//case 30:
	//	progStrobo(5000, 35, 50, 255, 255, 255);	// Weisser strobo
	//	break;

	case 35: // OK
		progCircles(15000, 40, 1000);
		break;

	case 40: // OK
		progFastBlingBling(15000, 5, 45); //20s -> 3:13
		break;

	case 45: 
		progOutline(10000, 50);
		break;

	case 50: 
		progMovingLines(10000, 55);
		break;

	case 55:
		progRandomLines(15000, 60, 500);
		break;
	
	case 60: 
		progBlingBlingColoring(60000, 100, 7625);
		break;
		
	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}
//==============================================

//#1 ... TODO: 30.05.2021: TODO: strobe am Ende zerlegen in kleinere Teile!
void LearnToFly() {

	switch (prog) {

	case 0: //0	text	13675
		progScrollText("Learn to fly by Foo Fighters", 13675, 75, getRandomColor(), 2);
		break;

	case 2: //2	intro	7050
		progStern(7050, 875, 5);
		break;

	case 5: //5	verse	28250
		progMatrixScanner(28250, 10);
		break;

	case 10: //10	chorus	24700
		progFullColors(24700, 15, 450);
		break;

	case 15: //15	intro 2	14125
		progStern(14125, 875, 20);
		break;

	case 20: // 20	verse 2	28225
		progRandomLines(28225, 25, 450);
		break;

	case 25: //25	chorus 2	30000
		progFastBlingBling(30000, 5, 30);
		break;

	case 30: //30	leise stelle	28250
		progBlingBlingColoring(28250, 35, 5000);
		break;

	case 35: //35	steigerung rein	3525
		progStrobo(3525, 40, 75, 255, 255, 255); // Weisser strobo
		break;

	case 40: // 40	chorus 3	24700
		progFullColors(24700, 45, 450);
		break;

	case 45: //45	chorus 4	21175
		progFastBlingBling(21175, 5, 50);
		break;

		// TODO: Strobo zerlegen in kleinere Teile!!!
	case 50: //50	abschluss chorus	14125
		progStrobo(14125, 55, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 55: //55	outro	17650
		progBlingBlingColoring(25000, 100, 5000); // 65535 is max for unsigned int!
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#2 ... TODO: optimieren + ende falsch (scrolltext)
void Castle() {	//TODO: weiter mit letztem chorus ...fastbling eher ans ende

	switch (prog) {

	case 0://0	text_straight for the castle	11075
		progScrollText("Castle by Halsey", 11075, 100, getRandomColor(), 1);
		break;

	case 1://1	vorspann	9225
		progBlingBlingColoring(9225, 5, 3000);
		break;

	case 5:// 5	intro	22150
		progCircles(22150, 10, 925);
		break;

	case 10://10	verse 1	14775
		progMatrixScanner(14775, 15);
		break;

	case 15://15	chor 1	16625
		progBlingBlingColoring(16625, 20, 3000);
		//progBlack(16615, 20);
		break;

	case 20://20	im headed straight for the castle	1850
		progWordArray(wordArrCastle, 6, 325, 1850, getRandomColor(), 25);
		break;

	case 25://25	chorus 1	12925
		progCircles(12925, 30, 450);
		break;

	case 30://30	im headed straight for the castle	1825
		progStrobo(1825, 35, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 35://35	chorus 1	14775
		progCircles(14775, 40, 450);
		break;

	case 40://40	verse 2	29550
		progMatrixScanner(29550, 45);
		break;

	case 45://45	chor 2	12925
		//progBlack(12925, 50);
		progBlingBlingColoring(12925, 50, 3000);
		break;

	case 50://50	im headed straight for the castle	1850
		progWordArray(wordArrCastle, 6, 325, 1850, getRandomColor(), 55);
		break;

	case 55://55	chorus 2	12900
		//progCircles(12900, 56, 450);
		progRandomLines(12900, 56, 450);
		break;

	case 56://56		1850
		progStrobo(1850, 58, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 58://58		14775
		//progCircles(14775, 60, 450);
		progRandomLines(14775, 60, 450);
		break;

	case 60://60	bridge	19850
		progStern(19850, 1850, 65);
		break;

	case 65://65	git kreischen 1	3700
		progStrobo(3700, 70, 50, 255, 255, 255); // Weisser strobo
		break;

	case 70://70	git kreischen 2	6000
		progStrobo(6000, 75, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 75://75	bridge weiter voc	14750
		progStern(14750, 925, 80);
		break;

	case 80://80	schrei	3700
		//progFastBlingBling(3700, 85);
		progStrobo(3700, 85, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 85://85	chorus 3	29550
		progCircles(29550, 90, 450, false);
		//progCircles(29550, 90, 450);
		break;

	case 90://90	chorus 4	28600
		//progOutline(28600, 95);
		progFastBlingBling(28600, 5, 95);
		break;

	case 95://95	text_straight for the castle	13850
		progWordArray(wordArrCastle, 100, 325, 60000, getRandomColor(), 9);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#3 -> 08.06.2021 OK!
void TooClose() {
	//FastLED.setBrightness(BRIGHTNESS); // zur sicherheit in jedem loop neu auf default setzen. ggf. kann einzelner fx das überschreiben

	switch (prog) {

	case 0: //0	text	11900
		progScrollText("Too close by Alex Clare", 11905, 75, getRandomColor(), 2);
		break;

	case 2: // 2	intro	25725
		progCircles(25725, 5, 950); 		// BPM sollten immer ein vielfaches von 25 sein!
		break;

	case 5: // 5	verse 2	45700
		progRandomLines(45700, 10, 475); 	// BPM sollten immer ein vielfaches von 25 sein!
		break;

	case 10: // 10	chorus 1	30475
		progFullColors(30475, 15, 950); 		// 30476
		break;

	case 15: // 15	uebergang zu verse 2	15250
		progCircles(15250, 20, 950); 		// 15238
		break;

	case 20: // 20	verse 2	30475
		progRandomLines(30475, 25, 475);		// 29025
		break;

	case 25: // 25	phil alleine	5725 (5675)
		progWordArray(wordArrTooCLose2, 10, 475, 5725, getRandomColor(), 30);
		break;

	case 30: // 30	chorus 2	24750
		progStern(24750, 1900, 35);				// 4558
		break;

	case 35: // 35	hardcore 1	15250
		progStrobo(15250, 40, 75, 255, 255, 255); // Weisser strobo // 11722
		break;

	case 40: // 40	 chorus 3	15225

		progFullColors(15225, 45, 950);		// 15238
		break;

	case 45: // 45	halftime	7625
		progFastBlingBling(7625, 5, 50);		// 395
		break;

	case 50: // 50	chorus weiter	7625
		progFullColors(7625, 55, 475);		// 7619
		break;

	case 55: // 55	hardcore 2	7600
		progStrobo(7600, 60, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue()); // 5861
		break;

	case 60: // 60	outro	17150
		progBlingBlingColoring(17150, 65, 7625); // 65535 is max for unsigned int!
		break;

	case 65: // 60	outro	17150
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#4 -> 08.06.2021: OK….. STOPs könnten noch eingebaut werden
void Pokerface() {

	switch (prog) {

	case 0://0	text	11100
		progScrollText("Pokerface by Lady Gaga", 11100, 75, getRandomColor(), 2);
		break;

	case 2: // 4525
		progPalette(4525, 4, 4);	// paletteID -> 0 - 10
		break;

	case 4://1	intro1	8075
		progPalette(8075, 3, 6);	// paletteID -> 0 - 10
		break;

	case 6://2	intro2	8075
		progPalette(8075, 2, 8);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

	case 8:// 5	verse	16125
		progCircles(16125, 10, 500);
		break;

	case 10://10	reggea	16125
		setDurationAndNextPart(16125, 15);
		display_rgbBitmap(9);
		break;

	case 15://15	pre chorus	16150
		progFullColors(16150, 20, 500);
		break;

	case 20://20	chorus	16125
		progStrobo(16125, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://25	verse 2	16125
		progCircles(16125, 30, 500);
		break;

	case 30://30	reggea	16150
		setDurationAndNextPart(16125, 35);
		display_rgbBitmap(9);
		//progPalette(16150, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://35	pre chorus	16125
		progFullColors(16125, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40://40	chorus	16150
		progStrobo(16150, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://45	pause	8050
		progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://50	gaga	16150
		progFastBlingBling(16150, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://55	pre chorus	16125
		progFullColors(16125, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://60	chorus	12100
		progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://65	chorus	4025
		progFastBlingBling(4025, 12, 70);
		break;

	case 70://70	ende schwarz	10000
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#5 -> todo: anfang scrolltext kommt ein 2. mal + checken
void UseSomebody() {

	switch (prog) {

	case 0://Text	20435
		progScrollText("Use Somebody by Kings of Leon", 20435, 75, getRandomColor(), 1);
		break;

	case 1://intro1		13913
		//progBlingBlingColoring(13913, 5);
		progPalette(13913, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://verse 1a		27826
		progPalette(27826, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

	case 5://verse 1b		27826
		progCircles(27826, 10, 425);
		break;

	case 10://chorus		27826
		progMovingLines(27826, 15);
		break;

	case 15://verse 2		13913
		progFullColors(13913, 20, 425);
		break;

	case 20://verse 2b		13913
		progStrobo(13913, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://chorus 2		27826
		progCircles(27826, 30, 425);
		break;

	case 30://bridge		13913
		//progMovingLines(13913, 35);
		progPalette(13913, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://solo		13913
		progFullColors(13913, 40, 425);
		//progCircles(13913, 40, 425);
		break;

	case 40://solo b		17391
		progStrobo(17391, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(17391, 40);
		break;

	case 45://outr		13913
		progMatrixScanner(13913, 70);
		//progBlack(13913, 50);
		break;

	//case 50://gaga	16134
	//	progFastBlingBling(16125, 7, 55);
	//	//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
	//	break;
	//case 55://pre chorus	16134
	//	progFullColors(16134, 60, 500);
	//	//progRandomLines(16134, 56, 450);
	//	break;
	//case 60://chorus	12101
	//	progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
	//	break;
	//case 65://chorus	4034
	//	progFastBlingBling(4025, 12, 70);
	//	break;

	case 70://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#6 -> ok aber am ende fehlen die texteinblendungen im letzten chorus
void NoRoots() {

	switch (prog) {

	case 0://klick		11897
		progScrollText("No Roots by Alice Merton", 11897, 75, getRandomColor(), 1);
		break;

	case 1://intro1		8276
		progPalette(8275, 7, 2);	// paletteID -> 0 - 10
		break;

	case 2://verse 1a		16552
		progPalette(16550, 6, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(16552, 5);
		break;

	case 5:// verse 1b		16552
		progCircles(16550, 6, 500);
		break;

	case 6://bridge		7759
		progMovingLines(7750, 7);
		break;

	case 7://ROOTS 1	1550	1552
		progShowROOTS(1550, 8);
		break;

	case 8://chorus a	2575	2586
		progFullColors(2575, 9, 500);
		break;

	case 9://ROOTS 2	1550	1552
		progShowROOTS(1550, 10);
		break;

	case 10://chorus a	3100	3103
		progFullColors(3100, 11, 500);
		break;

	case 11://chorus rooooots		7759
		progStrobo(7750, 12, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 12://ROOTS 1	1550	1552
		progShowROOTS(1550, 13);
		break;

	case 13://chorus a	2575	2586
		progFullColors(2575, 14, 500);
		break;

	case 14://ROOTS 2	1550	1552
		progShowROOTS(1550, 15);
		break;

	case 15://chorus a	3100	3103
		progFullColors(3100, 18, 500);
		break;

	case 18://chorus rooooots		8276
		progStrobo(8275, 20, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 20://verse 2a		16552
		progPalette(16550, 8, 25);	// paletteID -> 0 - 10
		break;

	case 25://verse 2b		16552
		progCircles(16550, 27, 500);
		break;

	case 27://bridge		7759
		//progMovingLines(8276, 35);
		progPalette(7750, 4, 29);	// paletteID -> 0 - 10
		break;

	case 29://ROOTS 1	1550	1552
		progShowROOTS(1550, 31);
		break;

	case 31://chorus a	2575	2586
		progFullColors(2575, 33, 500);
		break;

	case 33://ROOTS 2	1550	1552
		progShowROOTS(1550, 35);
		break;

	case 35://chorus a	3100	3103
		progFullColors(3100, 37, 500);
		break;

	case 37://chorus rooooots		7759
		progStrobo(7750, 39, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 39://ROOTS 1	1550	1552
		progShowROOTS(1550, 41);
		break;

	case 41://chorus a	2575	2586
		progFullColors(2575, 43, 500);
		break;

	case 43://ROOTS 2	1550	1552
		progShowROOTS(1550, 45);
		break;

	case 45://chorus a	3100	3103
		progFullColors(3100, 47, 500);
		break;

	case 47://chorus rooooots		8276
		progStrobo(8275, 49, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 49://zwischenspiel		18621
		//progPalette(18600, 2, 51);	// paletteID -> 0 - 10
		progMatrixHorizontal(18600, 51);
		break;

	case 51://solo		16552
		progMatrixScanner(16550, 53);
		break;

	case 53://chorus		8276
		//progFullColors(8276, 48, 525);
		progStern(8275, 500, 55);
		break;

	case 55://chorus		8276
		progFastBlingBling(8275, 7, 57);
		break;

	case 57://chorus		8276
		//progFullColors(8276, 50, 525);
		progStern(8275, 250, 59);
		break;

	case 59://chorus		7759
		//progStrobo(8276, 70, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		progFastBlingBling(7759, 12, 61);
		break;

	case 61://ende		10000
		//progBlack(10000, 100);
		//progShowText("ROOTS", 30000, 2, 13, getRandomColor(), 100);
		progShowROOTS(30000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#7 -> song-scrolltext schneller als sonst! ggf. noch optimieren?
void Firework() {

	switch (prog) {

	case 0://text		7500
		progScrollText("Firework by Katey Perry", 7500, 50, getRandomColor(), 5);
		break;

	case 5:// intro1		15484
		progStern(15475, 950, 10); 
		break;
	//----------------------------------

	case 10://verse 1a		15484
		progCircles(15475, 15, 500);
		break;

	case 15://verse 1b		15484
		progFullColors(15475, 20, 500);
		break;

	case 20://bridge		13548
		progPalette(13550, 4, 22);	// paletteID -> 0 - 10
		break;

	case 22://übergang		1935
		progStrobo(1925, 24, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progCircles(2125, 30, 500);
		break;

	case 24://chorus 1		14516
		progFastBlingBling(14500, 3, 26);
		break;

	case 26://übergang		968
		progStrobo(1000, 28, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 28://chorus 2		15445
		progFastBlingBling(15450, 7, 30);
		break;
		//----------------------------------

	case 30://verse 1a		15484
		progFullColors(15475, 32, 500);
		break;

	case 32://verse 1b		15484
		progCircles(15475, 34, 500);
		break;

	case 34://bridge		15484
		progPalette(13550, 5, 35);	// paletteID -> 0 - 10
		break;

	case 35://übergang		1935
		progStrobo(1925, 36, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progCircles(2125, 30, 500);
		break;

	case 36://chorus 1		14516
		progFastBlingBling(14500, 3, 37);
		break;

	case 37://übergang		968
		progStrobo(1000, 38, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 38://chorus 2		15445
		progFastBlingBling(15450, 7, 40);
		break;
		//----------------------------------

	case 40://BRIDGE		15484
		progPalette(15475, 2, 42);	// paletteID -> 0 - 10
		break;

	case 42://pause		14516
		progMatrixHorizontal(14500, 44);
		//progBlingBlingColoring(14500, 44);
		break;
		//----------------------------------

	case 44://übergang		1935
		progStrobo(1000, 46, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progCircles(2125, 30, 500);
		break;

	case 46://chorus 1		14516
		progFastBlingBling(14500, 5, 48);
		break;

	case 48://übergang		968
		progStrobo(1000, 50, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 50://chorus 2		15445
		progFastBlingBling(14500, 10, 55);
		break;
		//----------------------------------

	case 55://übergang		968
		progStrobo(1000, 60, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;
		//----------------------------------

	case 60://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#8 -> FERTIG: 01.05.2021
void Diamonds() {

	switch (prog) {

	case 0:
		progScrollText("Diamonds by Rihanna", 9600, 75, getRandomColor(), 1);
		break;
		   
	case 1:
		progPalette(19200, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://intro2	8067
		progPalette(19200, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

	case 5:
		progPalette(19200, 7, 10);	// paletteID -> 0 - 10
		break;

	case 10:
		//progMovingLines(16134, 15);
		progCircles(19200, 15, 150);
		break;

	case 15://pre chorus	16134
		progFullColors(28800, 20, 600);
		break;

	case 20:
		progPalette(19200, 8, 25);
		//progStrobo(19200, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25:
		progRandomLines(19200, 30, 150);
		//progCircles(19200, 30, 150);
		break;

	case 30:
		//progMovingLines(16134, 35);
		progPalette(26400, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35:
		progBlack(1200, 40);
		//progMovingLines(1200, 40);
		//progFullColors(16134, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40:
		progStrobo(1200, 45, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45:
		progFullColors(19200, 50, 150);
		//progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50:
		progFastBlingBling(19200, 7, 55);
		break;

	case 55:
		progPalette(12000, 5, 60);	// paletteID -> 0 - 10
		break;

	case 60:
		progBlingBlingColoring(38400, 100, 50);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#9 -> FERTIG: 02.05.2021
void SetFire() {

	switch (prog) {

	case 0://text	115	14348
		progScrollText("Set fire to the rain by Adele", 14348, 75, getRandomColor(), 1);
		break;

	case 1://v1: i let it fall	115	10696
		//progBlingBlingColoring(24202, 5);
		progPalette(10696, 4, 2);
		//progPalette(10696, 8, 2);	// paletteID -> 0 - 10
		break;

	case 2://v1: my hands	115	16696
		progPalette(16696, 8, 5);	// paletteID -> 0 - 10
		//progPalette(16696, 11, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

	case 5: // tremolo bridge	115	14609
		progPalette(14609, 1, 10);
		//progCircles(14609, 10, 500);
		break;

	case 10: // chorus 1	115	16696
		progStern(16696, 525, 15);
		break;

	case 15://v2: as i lay …	115	16696
		progPalette(16696, 11, 20);
		//progCircles(16696, 20, 525);
		//progFullColors(16134, 20, 1050);
		break;

	case 20:// 20	tremolo bridge	115	14609
		progPalette(14609, 1, 25);
		//progStrobo(16696, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://chorus 2 a	115	16696
		progCircles(16696, 30, 525);
		break;

	case 30://chorus 2 b	115	16696
		progStern(16696, 525, 35);
		//progPalette(16696, 11, 35);
		//progPalette(16696, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://sometimes	115	16696
		progFullColors(16696, 40, 525);
		//progCircles(14769, 40, 450);
		break;

	case 40://1 takt Stop	115	2087
		//progBlack(2087, 45);
		progMovingLines(2087, 45);
		//progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://chorus 3 a	115	16696
		progCircles(16696, 50, 250);
		//progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://chorus 3 b	115	16696
		progRandomLines(16696, 55, 250, true);
		//progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://SOLO a	115	16696
		progFullColors(16696, 60, 525);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://SOLO b	115	16696
		progFastBlingBling(16696, 12, 65);
		//progCircles(16696, 50, 250, false);
		//progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus 4 a	115	16696
		progRandomLines(16696, 70, 250, false);
		//progFastBlingBling(4025, 12, 70);
		break;

	case 70://chorus 4 b	115	16696
		progCircles(16696, 75, 250, false);
		//progFastBlingBling(4025, 12, 75);
		break;

	case 75://BLING BLING	115	37565
		progMatrixHorizontal(37565, 80);
		//progFastBlingBling(4025, 12, 80);
		break;

	case 80://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#10 -> ist raus!
void Chandelier() {

	switch (prog) {

	case 0://Text	15630
		progScrollText("Chandelier by Sia", 15630, 75, getRandomColor(), 1);
		break;

	case 1://intro1	8067
		//progBlingBlingColoring(24202, 5);
		progPalette(8067, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://intro2	8067
		progPalette(8067, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

		//case 3://verse	16134
		//	progPalette(8067, 8, 5);	// paletteID -> 0 - 10
		//	//progMatrixScanner(24202, 5);
		//	break;

	case 5:// verse	16134
		progCircles(16134, 10, 500);
		break;

	case 10://reggea	16134
		progMovingLines(16134, 15);
		break;

	case 15://pre chorus	16134
		progFullColors(16134, 20, 500);
		break;

	case 20://chorus	16134
		progStrobo(16134, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://verse 2	16134
		progCircles(16134, 30, 500);
		break;

	case 30://reggea	16134
		//progMovingLines(16134, 35);
		progPalette(16134, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://pre chorus	16134
		progFullColors(16134, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40://chorus	16134
		progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://pause	8067
		progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://gaga	16134
		progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://pre chorus	16134
		progFullColors(16134, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://chorus	12101
		progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus	4034
		progFastBlingBling(4025, 12, 70);
		break;

	case 70://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#11 -> FERTIG: 02.05.2021 +  6.5.21 -> zeitplan perfektioniert!!!!!!
void Titanium() {

	switch (prog) {

	case 0://text	11900
		progScrollText("Titanium by David Guetta", 11900, 75, getRandomColor(), 5);
		break;

	case 5:// intro	9050
		progCircles(9050, 10, 475);
		break;

	case 10://v1: u shout it out	15250
		progPalette(15250, 6, 15);
		//progMovingLines(15238, 15);
		break;

	case 15://v1: im criticised	15225
		progPalette(15225, 3, 20);
		//progFullColors(15238, 20, 500);
		break;

	case 20://im bulletproof	15250
		progMatrixScanner(15250, 25);
		//progStrobo(16134, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://chorus 1	15225
		progFullColors(15225, 30, 475);
		//progCircles(16134, 30, 500);
		break;

	case 30://bridge	15250
		progFastBlingBling(15250, 4, 35);
		//progMovingLines(16134, 35);
		//progPalette(16134, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://v2: cut me down	15225
		progPalette(15225, 9, 40);
		//progFullColors(16134, 40, 475);
		//progCircles(14769, 40, 450);
		break;

	case 40://v2: race your voice	15250
		progPalette(15250, 3, 45);
		//progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://im bulletproof	15225
		progMatrixScanner(15225, 50);
		//progBlack(12923, 50);
		break;

	case 50://chorus 2	15250
		progFullColors(15250, 55, 475);
		//progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://bridge	14750
		progFastBlingBling(14750, 8, 60);
		//progFullColors(16134, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://Pause: STONE HEART ..	15725
		progBlack(15725, 65);
		//progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus 3	30475
		progFastBlingBling(30475, 1, 75, 1, 100, 2000);
		//progFullColors(15238, 70, 250);
		break;

	//case 70://chorus 4	126	15238
	//	progFullColors(15238, 75, 125);
	//	break;

	case 75://bridge triolisch	13325
		progStrobo(13325, 80, 175, 255, 255, 255);
		//progFastBlingBling(13333, 12, 80);
		//progFastBlingBling(13333, 2, 80, 1, 100, 500);
		break;

	case 80://matrix	7625
		progMatrixHorizontal(7625, 85);
		break;

	case 85://BLACK	126	28571
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#12 -> is raus!
void SomeoneYouLoved() {

	switch (prog) {

	case 0://Text	15630
		progScrollText("Someone you loved by Lewis Capaldi", 15630, 75, getRandomColor(), 1);
		break;

	case 1://intro1	8067
		//progBlingBlingColoring(24202, 5);
		progPalette(8067, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://intro2	8067
		progPalette(8067, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

		//case 3://verse	16134
		//	progPalette(8067, 8, 5);	// paletteID -> 0 - 10
		//	//progMatrixScanner(24202, 5);
		//	break;

	case 5:// verse	16134
		progCircles(16134, 10, 500);
		break;

	case 10://reggea	16134
		progMovingLines(16134, 15);
		break;

	case 15://pre chorus	16134
		progFullColors(16134, 20, 500);
		break;

	case 20://chorus	16134
		progStrobo(16134, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://verse 2	16134
		progCircles(16134, 30, 500);
		break;

	case 30://reggea	16134
		//progMovingLines(16134, 35);
		progPalette(16134, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://pre chorus	16134
		progFullColors(16134, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40://chorus	16134
		progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://pause	8067
		progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://gaga	16134
		progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://pre chorus	16134
		progFullColors(16134, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://chorus	12101
		progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus	4034
		progFastBlingBling(4025, 12, 70);
		break;

	case 70://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#13 -> TODO!
void ShouldntStop() {

	switch (prog) {

	case 0://Text	15630
		progScrollText("Shouldn't stop moving by Nerds on Fire", 15630, 75, getRandomColor(), 1);
		break;

	case 1://intro1	8067
		//progBlingBlingColoring(24202, 5);
		progPalette(8067, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://intro2	8067
		progPalette(8067, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

		//case 3://verse	16134
		//	progPalette(8067, 8, 5);	// paletteID -> 0 - 10
		//	//progMatrixScanner(24202, 5);
		//	break;

	case 5:// verse	16134
		progCircles(16134, 10, 500);
		break;

	case 10://reggea	16134
		progMovingLines(16134, 15);
		break;

	case 15://pre chorus	16134
		progFullColors(16134, 20, 500);
		break;

	case 20://chorus	16134
		progStrobo(16134, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://verse 2	16134
		progCircles(16134, 30, 500);
		break;

	case 30://reggea	16134
		//progMovingLines(16134, 35);
		progPalette(16134, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://pre chorus	16134
		progFullColors(16134, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40://chorus	16134
		progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://pause	8067
		progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://gaga	16134
		progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://pre chorus	16134
		progFullColors(16134, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://chorus	12101
		progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus	4034
		progFastBlingBling(4025, 12, 70);
		break;

	case 70://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#14 -> FERTIG: 03.05.2021 +  4.5.21 -> zeitplan perfektioniert!!!!!!
void SunAlwaysShinesOnTV() {

	switch (prog) {

	case 0: // scrolltext	126	16900
		progScrollText("The sun always shines on TV by a-ha", 16900, 75, getRandomColor(), 2);
		break;

	case 2: // BLACK	126	5250
		progBlack(5250, 4);
		//progFullColors(1900, 10, 2000);
		break;

	case 4: // touch	126	950
		progShowText("Touch", 950, 0, 13, getRandomColor(), 6);
		break;

	case 6:// me	126	950
		progShowText("Me", 950, 6, 13, getRandomColor(), 8);
		break;

	case 8: // intro: how can it be	126	13325
		progCircles(13325, 10, 1900, true);
		break;

	case 10: // hold	126	1900
		progShowText("Hold", 1900, 1, 13, getRandomColor(), 12);
		break;

	case 12: // me	126	1925
		progShowText("Me", 1925, 6, 13, getRandomColor(), 14);
		break;

	case 14: // intro: sun alway shines	126	13325
		progCircles(13325, 16, 1900, false);
		break;

	case 16: // Steigerung	126	15225
		progFastBlingBling(15225, 1, 18, 1, 100, 2000);
		break;

	case 18: // vorspiel	126	7625
		progFullColors(7625, 20, 475);
		break;

	case 20: // melody lick	126	15250
		progRandomLines(15250, 22, 475, false);
		break;

	case 22: // verse 1	126	30475
		progBlingBlingColoring(30475, 24, 5000);//3    59,5hz
		break;

	case 24: // touch	126	950
		progShowText("Touch", 950, 0, 13, getRandomColor(), 26);
		break;

	case 26:// me	126	950
		progShowText("Me", 950, 6, 13, getRandomColor(), 28);
		break;

	case 28:// ch1: strobo	126	5250
		progStrobo(5250, 30, 250, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 30:// BLACK	126	2375
		progBlack(2375, 32);
		break;

	case 32:// chorus:stern	126	13325
		progStern(13325, 475, 34);
		break;

	case 34:// strobo fast	126	3800
		progStrobo(3800, 36, 75, 255, 255, 255);
		break;

	case 36:// full colors	126	3825
		progFullColors(3825, 38, 475);
		break;

	case 38: // melody lick	126	7625
		progRandomLines(7625, 40, 475, false);
		break;

	case 40: // verse 2	126	30475
		progBlingBlingColoring(30475, 42, 5000);//3    59,5hz
		break;

	case 42: // touch	126	950
		progShowText("Touch", 950, 0, 13, getRandomColor(), 44);
		break;

	case 44:// me	126	950
		progShowText("Me", 950, 6, 13, getRandomColor(), 46);
		break;

	case 46:// ch1: strobo	126	5225
		progStrobo(5225, 48, 250, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progStrobo(5225, 48, 300, 255, 255, 255);
		break;

	case 48:// BLACK	126	2400
		progBlack(2400, 50);
		break;

	case 50:// chorus:stern	126	13325
		progStern(13325, 475, 52);
		break;

	case 52:// strobo fast	126	3800
		progStrobo(3800, 54, 75, 255, 255, 255);
		break;

	case 54:// full colors	126	3825
		progFullColors(3825, 56, 475);
		break;

	case 56: // pre-Solo	126	11425
		progMatrixScanner(11425, 58);
		break;
		
	case 58:// SOLO	126	13325
		progFastBlingBling(13325, 1, 60, 1, 100, 2000);
		break;

	case 60:// TREMOLO	126	1900
		progStrobo(1900, 62, 50, 255, 255, 255);
		break;

	case 62:// halfTime	126	3825
		progFullColors(3825, 64, 950);
		break;

	case 64:// chorus:stern	126	2850
		progStern(2850, 475, 66);
		break;

	case 66:// TREMOLO	126	950
		progStrobo(950, 68, 50, 255, 255, 255);
		break;

	case 68:// chorus:stern	126	10475
		progStern(10475, 475, 70);
		break;

	case 70:// TREMOLO	126	950
		progStrobo(950, 72, 50, 255, 255, 255);
		break;

	case 72:// 	126	14300
		progStern(14300, 475, 74);
		break;

	case 74:// 	126	950
		progStrobo(950, 76, 50, 255, 255, 255);
		break;

	case 76:// BLACK	126	7625
		progBlack(7500, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop 
		break;
	}
}

//#15 -> FERTIG: 05.05.2021 +  6.5.21 -> zeitplan perfektioniert!!!!!!
void peopleArePeople() {

	switch (prog) {

	case 0: // 0	scrolltext	21500
		progScrollText("People are people by Depeche Mode", 21500, 90, getRandomColor(), 2);
		//progBlack(8000, 2);
		break;

	case 2: // intro	17750	scanner
		progMatrixScanner(17750, 4);
		break;

	case 4: // chorus 1	15250
		progFullColors(15250, 5, 250);
		break;

	case 5: // bam bam FX	875
		progStrobo(875, 6, 125, 255, 255, 255);
		break;

	case 6: // 3er übergang	3125
		progStern(3125, 1000, 7);
		break;

	case 7: // bam bam FX	875
		progStrobo(875, 8, 125, 255, 255, 255);
		break;

	case 8: // 3er übergang	1875
		progStern(1875, 1000, 9);
		break;

	case 9:// verse 1	16000
		progCircles(16000, 10, 500);
		//progStrobo(16000, 10, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 10: // i cant understand…	6000
		progPalette(6000, 7, 12);
		//progMovingLines(6000, 12);
		break;

	case 12: // bam bam FX	500
		progStrobo(500, 14, 125, 255, 255, 255);	// Weisser strobo
		//progFullColors(15000, 30, 2000);
		break;

	case 14: // black	500
		progBlack(500, 16);
		break;

	case 16: // snarewirbel	1000
		progCircles(1000, 18, 50); // auch cool
		//progRandomLines(1000, 18, 50);
		//progStrobo(1000, 18, 50, 255, 255, 255);
		break;

	case 18: // chorus 2	15250
		progFullColors(15250, 20, 250);
		//progCircles(15250, 20, 500);
		break;

	case 20: // bam bam FX	750
		//progStrobo(750, 22, 125, 255, 255, 255);
		progStrobo(750, 22, 125, getRandomColorValue(), 255, getRandomColorValue());
		break;

	case 22: // 3er übergang	6000
		progBlingBlingColoring(28000, 26, 2000);
		//progOutline(6000, 24);
		break;

	case 24: // help me understand	22000
		
		break;

	case 26: // verse 2	16000
		progRandomLines(16000, 28, 500);
		break;

	case 28: // i cant understand…	6000
		progPalette(6000, 8, 30);
		//progMatrixHorizontal(6000, 30);
		//progMovingLines(6000, 30);
		break;

	case 30: // bam bam FX	500
		progStrobo(500, 32, 125, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 32: // black	500
		progBlack(500, 34);
		break;

	case 34: // snarewirbel	1000
		progStrobo(1000, 36, 50, 255, 255, 255);
		break;

	case 36: // chorus 2	16000
		progStern(16000, 500, 38);
		break;

	case 38: // übergang	3250
		progMatrixScanner(3250, 40);
		break;

	case 40: // bam bam FX	750
		progStrobo(750, 42, 125, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 42: // i cant understand…	8000
		//progPalette(3250, 8, 40);
		progMatrixHorizontal(8000, 44);
		//progMovingLines(8000, 44);
		break;

	case 44: // bam bam FX	500
		progStrobo(500, 46, 125, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 46: // i cant understand…	7500
		progMatrixHorizontal(7500, 48);
		//progPalette(7500, 8, 48);
		//progRandomLines(7500, 48, 500, true);
		break;

	case 48: // bam bam FX	500
		progStrobo(500, 50, 125, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 50: // black	500
		progBlack(500, 52);
		break;

	case 52: // snarewirbel	1000
		progCircles(1000, 54, 50); // auch cool
		//progStrobo(1000, 54, 50, 255, 255, 255);
		break;

	case 54: // chorus 3	15500
		progFullColors(15500, 55, 250);
		break;

	case 55: // bam bam FX	500
		progStrobo(500, 56, 100, 255, 255, 255);
		break;

	case 56: // chorus 4	15000
		progFastBlingBling(15000, 1, 58, 1, 100, 1500);
		break;

	case 58: // snarewirbel	1000
		progStrobo(1000, 60, 50, 255, 255, 255);
		break;

	case 60: // BLACK	10000
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#16 -> FERTIG: 6.5.21 -> zeitplan perfektioniert!!!!!!
void enjoyTheSilence() {

	switch (prog) {

	case 0://text	21500
		progScrollText("Enjoy the silence by Depeche Mode", 21500, 90, getRandomColor(), 5);
		break;

	case 5://intro1	11675
		progPalette(11675, 11, 10);	// paletteID -> 0 - 10
		break; 

	case 10:// verse 1a	8500
		progPalette(8500, 3, 15);	// paletteID -> 0 - 10
		break;

	case 15:// all i ever wanted	8500
		progFullColors(8500, 20, 1050);
		break;

	case 20://words are very	7975
		progStrobo(7975, 25, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://STOP	525
		progBlack(525, 30);
		break;

	case 30://drumloop	2125
		progMovingLines(2125, 35);
		break;

	case 35://synth	16975
		progFastBlingBling(16975, 5, 40); //20s -> 3:13
		break;

	case 40://verse 1a	8500
		progPalette(8500, 2, 45);
		break;

	case 45://verse 1b	8500
		progPalette(8500, 3, 50);	// paletteID -> 0 - 10
		break;

	case 50://all i ever wanted	8500
		progFullColors(8500, 55, 1050);
		break;

	case 55://words are very	8500
		progStrobo(8500, 60, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 60://1  2  3  4	2125
		progScrollText("1  2  3  4", 2125, 25, getRandomColor(), 65);
		break;

	case 65: //enjoy the silence	8500
		progFastBlingBling(8500, 2, 70); //20s -> 3:13
		break;
	
	case 70: //enjoy the silence	8475
		progFastBlingBling(8475, 4, 75); //20s -> 3:13
		break;

	case 75: //bass slap part	17000
		progPalette(17000, 9, 80);	// paletteID -> 0 - 11
		break;

	case 80:// all i ever wanted	8500
		progFullColors(8500, 85, 1050);
		break;

	case 85://words are very	8500
		progStrobo(8500, 90, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 90:// all i ever wanted	8475
		progFullColors(8475, 95, 1050);
		break;

	case 95://words are very	8500
		progStrobo(8500, 100, 50, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 100://1  2  3  4	2125
		progScrollText("1  2  3  4", 2125, 25, getRandomColor(), 105);
		break;

	case 105: //enjoy the silence	8500
		progFastBlingBling(8500, 1, 110); //20s -> 3:13
		break;

	case 110: //enjoy the silence	8500
		progFastBlingBling(8500, 2, 115); //20s -> 3:13
		break;

	case 115: //enjoy the silence	8500
		progFastBlingBling(8500, 3, 120); //20s -> 3:13
		break;

	case 120: //enjoy the silence	8500
		progFastBlingBling(8500, 4, 125); //20s -> 3:13
		break;

	case 125: //enjoy the silence	8475
		progFastBlingBling(8475, 5, 130); //20s -> 3:13
		break;

	case 130: //enjoy the silence	8500
		progFastBlingBling(8500, 6, 135); //20s -> 3:13
		break;

	case 135: //BLACK
		progBlack(10000, 200); //20s -> 3:13
		break;

	case 200:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#17 -> TODO!
void sober() {

	switch (prog) {

	case 0: //black	2875
		progBlack(2875, 2);
		break;

	case 2: //  text	9575
		progScrollText("Sober by Pink", 9575, 90, getRandomColor(), 4);
		//progMatrixScanner(17750, 4);
		break;

	case 4: // v1a: i dont wanna be	10200
		//progFullColors(10200, 6, 250); 
		progPalette(10200, 7, 6);
		break;

	case 6: // v1b: i dont wanna be	10225
		progPalette(10225, 3, 8);
		//progStrobo(10225, 8, 125, 255, 255, 255);
		break;

	case 8: // ah…the sun is blinding	10200
		//progStern(10200, 1000, 10);
		progCircles(10200, 10, 650, false);
		break;

	case 10: // oh.. Im finding	9575 
		progRandomLines(9575, 11, 650, false);
		break;

	case 11:// fx 650
		progStrobo(650, 12, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 12: // chorus 1a	10200
		progFullColors(10200, 14, 650);
		//progStern(10200, 650, 14);
		break;

		
		//TODO: hier noch @1877,75 und @1881,75 einen FX einbauen??


	case 14:// chorus 1b	10225
		progStern(10225, 650, 16, 15);
		break;

	case 16: // verse 2a	2550
		progPalette(2550, 7, 18);
		//progMovingLines(6000, 12);
		break;

	case 18: // silence	2550
		progBlack(2550, 20);	// Weisser strobo
		//progFullColors(15000, 30, 2000);
		break;

	case 20: // verse 2a	5100
		progPalette(5100, 3, 22);
		break;

	case 22: // verse 2b	10225
		progPalette(10225, 4, 24);
		break;

	case 24: // ah…the night is calling	10200
		progCircles(10200, 26, 650, false);
		break;

	case 26: // ah …im falling	7675
		progRandomLines(7675, 28, 650, false);
		break;

	case 28: // blaaaaame	1900
		progPalette(1900, 8, 30);
		break;

	case 30: // snarewirbel	650
		progStrobo(650, 32, 50, 255, 255, 255);
		break;

	case 32: // chorus 2a	4475
		//progRandomLines(4475, 34, 500);
		progFullColors(4475, 34, 650);
		break;

	case 34: // fx	625
		progStrobo(625, 36, 75, 255, 255, 255);
		break;

	case 36: // chorus 2a	5100
		progFullColors(5100, 38, 650);
		break;

	case 38: // chorus 2b	4475
		progStern(4475, 650, 40, 15);
		break;

	case 40: // fx	650
		progStrobo(650, 42, 50, 255, 255, 255);
		break;

	case 42: // chorus 2b	3825
		progStern(3825, 650, 44, 15);
		break;

	case 44: // begin slide	625
		progMatrixScanner(625, 46);
		break;

	case 46: // trommelwirbel	650
		progStrobo(650, 48, 50, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 48: // coming down	8925
		progCircles(8925, 50, 150, true);
		break;

	case 50: // fx	1300
		progStrobo(1300, 52, 100, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 52: // coming down	10200
		progRandomLines(10200, 54, 150, true);
		break;

	case 54: // when its good…	20425
		progBlingBlingColoring(20425, 56, 2000);
		//progStrobo(500, 50, 125, getRandomColorValue(), getRandomColorValue(), 255);
		break;

	case 56: // SOLO	9575
		progPalette(9575, 6, 58);
		//progBlack(500, 52);
		break;

	case 58: // trommelwirbel	650
		progStrobo(650, 60, 100, 255, 255, 255);
		break;

	case 60: // chorus 3a	4450
		progFullColors(4450, 62, 325);
		break;

	case 62: // fx	650
		progStrobo(650, 64, 100, 255, 255, 255);
		break;

	case 64: // chorus 3a	5100
		progFullColors(5100, 66, 325);
		break;

	case 66: // chorus 3b	4475
		progFullColors(4475, 68, 325);
		break;

	case 68: // fx	625
		progStrobo(625, 70, 50, 255, 255, 255);
		break;

	case 70: // chorus 3b	3850
		progStern(3850, 650, 72, 15);
		//progFullColors(3850, 72, 325);
		break;

	case 72: // trommelwirbel	1275
		progStrobo(1275, 74, 50, 255, 255, 255);
		break;

	case 74: // coming down	8925
		//progFullColors(8925, 76, 325);
		progCircles(8925, 76, 50, true);
		break;

	case 76: // fx	1275
		progStrobo(1275, 78, 50, 255, 255, 255);
		break;

	case 78: // coming down	10225
		progFastBlingBling(10225, 3, 80, 1, 100, 1500);
		break;

	case 80: // outro strings	12750
		progBlingBlingColoring(28000, 26, 2000);
		//progStrobo(1000, 60, 50, 255, 255, 255);
		break;

	case 82: // BLACK	10000
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}

//#18 -> ok: 5.3.22
void prisoner() {

	switch (prog) {

	case 0: //black	3525
		progBlack(5390, 2);
		break;

	case 2: //  text	15925
		progScrollText("Prisoner by Miley Cyrus ", 14065, 90, getRandomColor(), 4);
		break;

	case 4: // black	2825
		progBlack(2810, 6);
		break;

	case 6: // auftakt	1875
		progMovingLines(1875, 8);
		break;

	case 8: // v1: strung out on a ceiling	7500 
		progRandomLines(7500, 10, 470, false);
		break;

	case 10: // 10	oh i cant control it 	7500
		progStern(7500, 940, 12, 15);
		break;

	case 12:// 12	i try to replace it	7500
		progCircles(7500, 14, 470);
		break;

	case 14:// 14	oh i cant control it 	7500
		progStern(7500, 470, 16, 15);
		break;

	case 16: // 16	u keep making it hard	11250
		progPalette(11250, 7, 18);
		//progMovingLines(6000, 12);
		break;

	case 18: // 18	Chorus 1 a	14050
		progFullColors(14065, 20, 235);
		break;

	case 20: // 20	fx	950
		progStrobo(935, 22, 75, 255, 255, 255);
		break;

	case 22: // 22	Chorus 1 b	14050
		progCircles(14065, 24, 235); // auch cool
		break;

	case 24: // 24	fx	950
		progStrobo(935, 26, 75, 255, 255, 255);
		break;

	case 26: // 26	v2: i tasted heaven	7500
		progRandomLines(7500, 28, 470, false);
		break;

	case 28: // 28	oh i cant control it 	7500
		progStern(7500, 940, 30, 15);
		break;

	case 30: // 30	u keep making it hard	11250
		progPalette(11250, 8, 32);
		break;

	case 32: // 32	STOP / Vocals	3750
		progScrollText("Prisoner", 3750, 75, getRandomColor(), 34);
		break;

	case 34: // 34	Chorus 2 a	10300
		progStern(10315, 470, 36, 15);
		break;

	case 36: // 36	fx	950
		progStrobo(935, 38, 75, 255, 255, 255);
		break;

	case 38: // 38	Chorus 2 b	14050
		progFastBlingBling(14050, 4, 40);
		break;

	case 40: // 40	fx	950
		progStrobo(935, 42, 75, 255, 255, 255);
		break;

	case 42: // 42	Chorus 3 a	14050
		progFastBlingBling(14065, 8, 44);
		break;

	case 44: // 44	fx	950
		progStrobo(935, 46, 75, 255, 255, 255);
		break;

	case 46: // 46	let me go	5625
		progMatrixScanner(5625, 48, 30);
		break;

	case 48: // 50	TEXT: let me go	2575
		progScrollText("Let me go", 5500, 75, getRandomColor(), 50);
		break;

	case 50: // 52	BLACK	10000
		progBlack(10000, 100); 
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}
//==============================================,

void TEMPLATE() {

	switch (prog) {

	case 0://Text	15630
		progScrollText("Shouldn't stop moving by Nerds on Fire", 15630, 75, getRandomColor(), 1);
		break;

	case 1://intro1	8067
		//progBlingBlingColoring(24202, 5);
		progPalette(8067, 3, 2);	// paletteID -> 0 - 10
		break;

	case 2://intro2	8067
		progPalette(8067, 4, 5);	// paletteID -> 0 - 10
		//progMatrixScanner(24202, 5);
		break;

		//case 3://verse	16134
		//	progPalette(8067, 8, 5);	// paletteID -> 0 - 10
		//	//progMatrixScanner(24202, 5);
		//	break;

	case 5:// verse	16134
		progCircles(16134, 10, 500);
		break;

	case 10://reggea	16134
		progMovingLines(16134, 15);
		break;

	case 15://pre chorus	16134
		progFullColors(16134, 20, 500);
		break;

	case 20://chorus	16134
		progStrobo(16134, 25, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 25://verse 2	16134
		progCircles(16134, 30, 500);
		break;

	case 30://reggea	16134
		//progMovingLines(16134, 35);
		progPalette(16134, 4, 35);	// paletteID -> 0 - 10
		break;

	case 35://pre chorus	16134
		progFullColors(16134, 40, 500);
		//progCircles(14769, 40, 450);
		break;

	case 40://chorus	16134
		progStrobo(16134, 45, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		//progMatrixScanner(29538, 40);
		break;

	case 45://pause	8067
		progMatrixScanner(8050, 50);
		//progBlack(12923, 50);
		break;

	case 50://gaga	16134
		progFastBlingBling(16125, 7, 55);
		//progWordArray(wordArrCastle, 6, 325, 1846, getRandomColor(), 55);
		break;

	case 55://pre chorus	16134
		progFullColors(16134, 60, 500);
		//progRandomLines(16134, 56, 450);
		break;

	case 60://chorus	12101
		progStrobo(12100, 65, 75, getRandomColorValue(), getRandomColorValue(), getRandomColorValue());
		break;

	case 65://chorus	4034
		progFastBlingBling(4025, 12, 70);
		break;

	case 70://ende schwarz
		progBlack(10000, 100);
		break;

	case 100:
		FastLED.clear();
		switchToSong(0);	// SongID 0 == DEFAULT loop
		break;
	}
}
//=======================================================

//************* 21.01.22 TODO: WIEDER AKTIVIEREN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// interrupt every 25 ms so that fastLED can process uninterrupted (takes about 18 ms)

#define INCREMENT	5	// 25 // process FastLED-loops only every 25 ms (fast-led takes approx. 18 ms!!)
	//  => !!!! IMMER AUCH IN SETUP DEN CALLBACK AUFRUF ANPASSEN !!!!!

/* void setupInterrupt() {
    TCCR3A = 0;
    TCCR3B = 0x0B;      // WGM32 (CTC), Prescaler: // 0x0C = 256 // 0x0B = 64
    OCR3A = 6250;      // 16M/64(prescaler) * 0,025 sec (=25 ms) = 6250 [10ms = 2.500 / 15ms = 3.750 / 20ms = 5.000]
    TIMSK3 = 0x02;      // enable compare interrupt
}

ISR(TIMER3_COMPA_vect) {
	millisCounterTimer = millisCounterTimer + INCREMENT;	// wird von den progs fürs timing bzw. delay-ersatz verwendet
    millisCounterForSeconds = millisCounterForSeconds + INCREMENT;
    millisCounterForProgChange = millisCounterForProgChange + INCREMENT;

    flag_processFastLED = true;	// process FastLED-loops only every 25 ms (fast-led takes approx. 18 ms!!)
    PORTD ^= 0x40;				// toggle LED every 25 ms

    // test zur messung der timing-praezision
    if (millisCounterForSeconds >= 1000) {
        //actualMillis = millis();
        //diffMillis = actualMillis - lastMillis;
        //lastMillis = actualMillis;
        //Serial.println(millisCounterForSeconds);
        millisCounterForSeconds = 0;
        OneSecondHasPast = true;
		//PORTD ^= 0x40;
    }

	if (millisCounterForProgChange >= nextChangeMillis) switchToPart(nextSongPart);
}  */

void callback() // toggle the LED
{
	millisCounterTimer = millisCounterTimer + INCREMENT;	// wird von den progs fürs timing bzw. delay-ersatz verwendet
    millisCounterForSeconds = millisCounterForSeconds + INCREMENT;
    millisCounterForProgChange = millisCounterForProgChange + INCREMENT;
	millisToReduceCPUSpeed = millisToReduceCPUSpeed + INCREMENT;

    flag_processFastLED = true;	// process FastLED-loops only every 25 ms (fast-led takes approx. 18 ms!!)

    // test zur messung der timing-praezision
    if (millisCounterForSeconds >= 1000) {
        millisCounterForSeconds = 0;
        OneSecondHasPast = true;
    }

	if (millisCounterForProgChange >= nextChangeMillis) switchToPart(nextSongPart);
}

void setup() {
 
 	Serial.begin(9600);

	//--- interrupt-timer fuer callback
	t1.begin(callback, 5ms); // 25 ms => !!!! IMMER AUCH define INCREMENT ANPASSEN !!!!!

    //---- MIDI ----------------
    delay(1000);	// Time for serial port to work?
    //Serial1.begin(31250);	// for midi

	MIDI.begin(10); // Initialize the Midi Library.
	// OMNI sets it to listen to all channels.. MIDI.begin(2) would set it
	// to respond to notes on channel 2 only.
	MIDI.setHandleControlChange(MidiDatenAuswerten); // This command tells the MIDI Library
	// the function you want to call when a Continuous Controller command
	// is received. In this case it's "MyCCFunction".
    //---------------------

	//--- LIPO Safer ----------
	adc_value = analogRead(LIPO_PIN);     
	voltageSmooth = map(adc_value, 0, 440, 0, 90); // 440 entspricht 9,0 Volt
	
	//---- fuer OLED Display --------
	//   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	//   delay(100);
	//   display.clearDisplay();
	//   display.display();
	//------------------------------

	// the audio library needs to be given memory to start working
	//   AudioMemory(12);

	//--- Development LEDs setup -------
	pinMode(LED1_PIN, OUTPUT); 
	pinMode(LED2_PIN, OUTPUT); 
	pinMode(LED3_PIN, OUTPUT); 

	//---- Define matrix width and height. --------
	matrix = new FastLED_NeoMatrix(leds, MATRIX_WIDTH, MATRIX_HEIGHT, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);

	//----- initialize LEDs ---------
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMMATRIX).setCorrection(TypicalLEDStrip);
	//NEOPIXEL	//WS2812B
	matrix->begin();
	matrix->setBrightness(BRIGHTNESS);
	    
    matrix->setTextWrap(false);
    matrix->setRemapFunction(myRemapFn);	// fuer meine spezifische matrix!

	//------ Setup Palette
	currentPalette = RainbowColors_p;
	currentBlending = LINEARBLEND;
	
	//-----------------
	switchToSong(0);  // TODO: set back to 0 !!!!
}
//====================================================


//----- fuer audio in -----------
// float band[8];
// void printNumber(float n) {
//   if (n >= 0.024) {
//     Serial.print(n, 3);
//     Serial.print(" ");
//   } else {
//     Serial.print("   -  "); // don't print "0.00"
//   }
// }
//-----------------------------


void loop() {

	//=== ausserhalb vom fastLED loop ====

	//--- LEDs ggf. wieder ausschalten ---
	if (LED1_on == true) {
		if (millis() - LED1_millis >= 500) {
			digitalWrite(LED1_PIN, LOW);
			LED1_on = false;
		}
	}
	// if (LED2_on == true) {
	// 	if (millis() - LED2_millis >= 1) {
	// 		digitalWrite(LED2_PIN, LOW);
	// 		LED2_on = false;
	// 	}
	// }
	if (LED3_on == true) {
		if (millis() - LED3_millis >= 500) {
			digitalWrite(LED3_PIN, LOW);
			LED3_on = false;
		}
	}

	if (OneSecondHasPast) {
		secondsForVoltage++;	// count seconds for voltage lipo safer 
		OneSecondHasPast = false;
		//--- LED einschalten
		// digitalWrite(LED3_PIN, HIGH);
		// LED3_on = true;
		// LED3_millis = millis();
	}

	//---- check voltage as lipo safer ------
	if (secondsForVoltage >= SECONDSFORVOLTAGE) {
		
		adc_value = analogRead(LIPO_PIN);     
		voltageSmooth = 0.7 * voltageSmooth + 0.3 * map(adc_value, 0, 440, 0, 90); // 440 entspricht 9,0 Volt
											//0.7 * voltageSmooth + 0.3 * .... is used as a smoothing function
 		//  Serial.print("voltage = ");
		//  Serial.println(voltageSmooth);  

		secondsForVoltage = 0;
	}

	//---- start loop only when voltage is high enough
 	if (voltageSmooth > 94) {	// !!! TODO !!!! //only fire LEDs if voltage is > 7,99V

		//checkIncomingMIDI();
		//checkIncomingMIDITEST(); // macht nur einfache ausgabe der midi commands
		MIDI.read(); // Continuously check if Midi data has been received.


		//=== ab hier wird nur alle 5 ms ausgefuehrt ======

		if (flag_processFastLED) {	// LED loop only in certain time-slots to make ms-counter more accurate

			//--- debugging: LED ein
			//digitalWrite(LED2_PIN, HIGH);

			FastLED.setBrightness(BRIGHTNESS); // zur sicherheit for jedem loop neu auf default setzen. ggf. kann einzelner fx das überschreiben

			switch (songID) {
			case 0:
				defaultLoop();
				break;
			case 1:
				LearnToFly();
				break;
			case 2:
				Castle();
				break;
			case 3:
				TooClose();
				break;
			case 4:
				Pokerface();
				break;
			case 5:
				UseSomebody();
				break;
			case 6:
				NoRoots();
				break;
			case 7:
				Firework();
				break;
			case 8:
				Diamonds();
				break;
			case 9:
				SetFire();
				break;
			case 10:
				Chandelier();
				break;
			case 11:
				Titanium();
				break;
			case 12:
				SomeoneYouLoved();
				break;
			case 13:
				ShouldntStop();
				break;
			case 14:
				SunAlwaysShinesOnTV();
				break;
			case 15:
				peopleArePeople();
				break;
			case 16:
				enjoyTheSilence();
				break;
			case 17:
				sober();
				break;
			case 18:
				prisoner();
				break;

			case 20:
				TEMPLATE();
				break;

			default:
				defaultLoop();
				break;
			}
			flag_processFastLED = false;
			
			//-- debugging: LED aus
			//digitalWrite(LED2_PIN, LOW);
		}
	}
	else {	// if voltage is too low let LED 0 blink red
		FastLED.clear();
		FastLED.show();
		delay(500);
		FastLED.setBrightness(BRIGHTNESS);
		leds[0] = CRGB::Red;
		FastLED.show();
		delay(500);
	} 


//---- fuer audio in -------------------------
//   if (fft.available()) {
//     Serial.print("FFT: ");
//     for (int i = 0; i < 8; i++) { // 0-25  -->  DC to 1.25 kHz
//       float n = fft.read(i);
//       //printNumber((1- n)* 1000);
//       band[0] = fft.read(0);      //0  //0
//       band[1] = fft.read(1);      //0  //1
//       band[2] = fft.read(2, 3);   //1  //2
//       band[3] = fft.read(3, 5);   //2  //3
//       band[4] = fft.read(4, 6);   //3  //4
//       band[5] = fft.read(6, 14);  //6  //6
//       band[6] = fft.read(14, 32); //12 //10
//       band[7] = fft.read(32, 45); //23 //18
//       printNumber(band[i]);
//     }
//     Serial.println();
//   }
//---------------------------------------------

//----- fuer oled-display -------------------
// if (flag_update_display == true) {
//   display.clearDisplay(); // clear buffer
//   for (int r = 0; r < 9; r++)
//   {
//     for (int y = 0; y < 8; y++) // fill buffer completely withs chars
//     {
//       display.setTextSize(1);
//       display.setTextColor(1);
//       display.setCursor(0, (y * 8));
//       display.print("012345678901234567890");
//     }
//     display.setTextSize(1); // erase one line at the time
//     display.setTextColor(0);
//     display.setCursor(0, (r * 8));
//     display.print("012345678901234567890");
//     display.display();
//     //delay(200);
//   }
//   flag_update_display = false;
// }
//------------------------------------------

}
//====================================================



