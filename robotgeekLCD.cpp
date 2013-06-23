#include "robotgeekLCD.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include <Wire.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// robotgeekLCD constructor is called).

//Rs-p0
//R/W-p2
//E-p2
//Backlight-p3
//DB4-p4
//DB5-p5
//DB6-p6
//DB7-p7


//the LCD will always be IIC, so we'll strip out alot of the LiquidCrystal Library



robotgeekLCD::robotgeekLCD(uint8_t address)
{
  init(0x27);
  
}



//always four bit mode
void robotgeekLCD::init(uint8_t address)
{

  Wire.begin(); // join i2c bus (address optional for master)
	
  _rs_pin =0;
  _rw_pin = 1;
  _enable_pin = 2;
  
  _data_pins[0] = 0;
  _data_pins[1] = 0;
  _data_pins[2] = 0;
  _data_pins[3] = 0; 
  _data_pins[4] = 4;
  _data_pins[5] = 5;
  _data_pins[6] = 6;
  _data_pins[7] = 7; 

//always 4 bit mode
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;



	//always 16/x
  begin(address);  
}

void robotgeekLCD::begin(uint8_t address) {


  _numlines = 2;
  _currline = 0;


	int tadd = address;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delayMicroseconds(50000); 
  // Now we pull both RS and R/W low to begin commands
  
  _iicLastSent = B00000000;
  Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
  Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
  Wire.endTransmission();    // stop transmitting



    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02); 
    
  
  

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

/********** high level commands, for the user! */
void robotgeekLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void robotgeekLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void robotgeekLCD::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row >= _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void robotgeekLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void robotgeekLCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}


// Turn the display on/off (quickly)
void robotgeekLCD::nooBacklight() {



		_iicLastSent = _iicLastSent & (~B00001000);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		delay(1000);
		
		
		_iicLastSent = _iicLastSent | B00001000;
		
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		
		delay(1000);
		
			
		_iicLastSent = _iicLastSent & (~B00001000);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		delay(1000);
		
		
		_iicLastSent = _iicLastSent | B00001000;
		
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		
		delay(1000);
		
			
		_iicLastSent = _iicLastSent & (~B00001000);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		delay(1000);
		
		
		_iicLastSent = _iicLastSent | B00001000;
		
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		
		delay(1000);
		
				
			
		
		
}

// Turn the display on/off (quickly)
void robotgeekLCD::noBacklight() {



		_iicLastSent = _iicLastSent & (~B00001000);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting


				
			
		
		
}

void robotgeekLCD::backlight() {
		_iicLastSent = _iicLastSent | B00001000;
		
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
		
		
}



// Turns the underline cursor on/off
void robotgeekLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void robotgeekLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void robotgeekLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void robotgeekLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void robotgeekLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void robotgeekLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void robotgeekLCD::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void robotgeekLCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void robotgeekLCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void robotgeekLCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void robotgeekLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void robotgeekLCD::command(uint8_t value) {
  send(value, LOW);
}

inline size_t robotgeekLCD::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void robotgeekLCD::send(uint8_t value, uint8_t mode) {

	//int tempValue = mode;//rspin high
	//tempValue = tempValue &(~(1<<2))//rw_pin low
	


	if(mode == 1)
	{
		_iicLastSent = _iicLastSent | B00000001;
		_iicLastSent = _iicLastSent & (~B00000010);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
	}
	else
	{
	
		_iicLastSent = _iicLastSent & (~B00000011);
		Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
		Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
		Wire.endTransmission();    // stop transmitting
	
	
	}



  //digitalWrite(_rs_pin, mode);

  // if there is a RW pin indicated, set it low to Write
 /* if (_rw_pin != 255) { 
    digitalWrite(_rw_pin, LOW);
  }
  
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } 
  else {
  */  
    write4bits(value>>4);
    write4bits(value);
  //}
}

void robotgeekLCD::pulseEnable(void) {


  
  _iicLastSent = _iicLastSent & (~B00000100);//mask last send with inverse enable bit to turn it off 
  Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
  Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
  Wire.endTransmission();    // stop transmitting

  delayMicroseconds(1);   // enable pulse must be >450ns 
  
  _iicLastSent = _iicLastSent | B00000100;//mask last send with enable bit to turn it on 
  Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
  Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
  Wire.endTransmission();    // stop transmitting

  delayMicroseconds(1);    // enable pulse must be >450ns
  
  _iicLastSent = _iicLastSent & (~B00000100);//mask last send with inverse enable bit to turn it off 
  Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
  Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
  Wire.endTransmission();    // stop transmitting

  delayMicroseconds(100);    // commands need > 37us to settle



}

void robotgeekLCD::write4bits(uint8_t value) {

	int tempValue = value << 4;//shift the value up 8 bits
	_iicLastSent = _iicLastSent | (tempValue);//mask last sent value with value bits 
	Wire.beginTransmission(0x27); // transmit to device 0x27, lcd
	Wire.write(_iicLastSent);              // sends one byte, sets RS,R/W, and enable low  
	Wire.endTransmission();    // stop transmitting
	pulseEnable();
}


