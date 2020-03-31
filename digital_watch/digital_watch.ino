// EMBEDDED SYSTEMS PROJECT - DIGITAL WATCH

/********************************* INITIALIZATION *********************************/
// initialize libraries
#include <LiquidCrystal.h>
#include <IRremote.h> 
// initialize pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buttonPin = 10;
int button_state = 0;
const int ledPin = 9;
IRrecv irrecv(7);
decode_results results;  
// store watch date and time
volatile int sec = 0;
volatile int minute = 0;
volatile int hour = 0;
volatile int day = 1;
volatile int month = 1;
volatile int year = 20;
// store stopwatch time
volatile int total_sec = 0;
volatile int total_minute = 0;
volatile int total_hour = 0;
volatile bool total_count = false;
volatile int lap_sec = 0;
volatile int lap_minute = 0;
volatile int lap_hour = 0;
// store timer time
volatile int timer_sec = 0;
volatile int timer_minute = 0;
volatile int timer_hour = 0;
volatile bool timer_count = false;
// set default mode
int mode = 11;

/********************************* REMOTE FUNCTIONS *********************************/
// get input from remote
int readRemoteInput() {
  while(1) {
    if (irrecv.decode(&results)) { // returns 0 if no data ready, 1 if data ready.
      irrecv.resume();
      if (results.value == 0XFF6897) return 0;
      if (results.value == 0xFF30CF) return 1;
      if (results.value == 0xFF18E7) return 2;
      if (results.value == 0xFF7A85) return 3;
      if (results.value == 0xFF10EF) return 4;
      if (results.value == 0xFF38C7) return 5;
      if (results.value == 0xFF5AA5) return 6;
      if (results.value == 0xFF42BD) return 7;
      if (results.value == 0xFF4AB5) return 8;
      if (results.value == 0xFF52AD) return 9;
    }
  }
}
void readData(String displayText, volatile int &value) {
  while(1){
    int digit1, digit2 = 0;
    clearLCD();
  
    writeLCD(displayText, 0);
#ifdef NO_REMOTE
  delay(1000);
#endif
    digit1 = readRemoteInput();
    writeLCD(String(digit1), 1);
  
#ifdef NO_REMOTE
  delay(1000);
#endif
    digit2 = readRemoteInput();
    value = digit1*10 + digit2;
    writeLCD(String(value), 1);
    if (lockInput()){
      break;
    }
  }
}
// gives user two seconds to input value
bool lockInput(void)
{
  // current time in milliseconds
  long start_time = millis();
  while((millis() - start_time) < 2000){
    button_state = digitalRead(buttonPin);
    if (button_state == HIGH){
      return true;
    }
  }
  return false;
}


/************************************ LCD FUNCTIONS *********************************/
// write text to the lcd
void writeLCD(String text, int cursor) {
  lcd.setCursor(0,cursor);
  lcd.print(text);
}
// clear text off lcd
void clearLCD() {
  lcd.clear();
}


/************************************ MODE LOGIC ************************************/
// MODE 0  logic - display date and time
void displayDateTime() {
  clearLCD();
  writeLCD(String(hour) + ":" + String(minute) + ":" + String(sec), 0);
  writeLCD(String(day) + "-" + String(month) + "-" + String(year), 1);
}
// MODE 1 logic - set date and time
void setDateTime() {
  clearLCD();
  writeLCD("Mode 1", 0);
  writeLCD("Set date/time", 1);
  delay(2000);
  readData("Enter hours:", hour); // enter hours
  readData("Enter min:", minute); // enter min
  readData("Enter sec:", sec); // enter sec
  readData("Enter year:", year); // enter year
  readData("Enter month:", month); // enter month
  readData("Enter day:", day); // enter day
}
// MODE 3 logic - timer
void timer() {
  clearLCD();
  writeLCD("Mode 3", 0);
  writeLCD("Countdown Timer", 1);
  delay(2000);
  readData("Enter hours:", timer_hour); // enter hours
  readData("Enter min:", timer_minute); // enter min
  readData("Enter sec:", timer_sec); // enter sec
  timer_count = true; // start counting down
  while(1){
    clearLCD();
    writeLCD("Press button", 1);
    writeLCD("to start", 2);
    button_state = digitalRead(buttonPin);
    if (button_state == HIGH){
      break;
    }
    delay(50);
  }
  
  while(1){
    // output current time
    clearLCD();
    writeLCD("Time: " + String(timer_hour) + ":" + String(timer_minute) + ":" + String(timer_sec), 0);
    
    // check if done
    if (timer_sec == 0 && timer_minute == 0 && timer_hour == 0) {
      clearLCD();
      writeLCD("DONE", 0);
      digitalWrite(ledPin,HIGH);
      timer_count = false;
      break;
    }
    // start and stop timer with button
    button_state = digitalRead(buttonPin);
    if (button_state == HIGH){
      timer_count = !timer_count;
    }
    
    delay(50);
  }
  
  total_count = false; // stop counting
}

// mode 2 logic - stopwatch
void stopWatch() {
  clearLCD();
  writeLCD("Mode 2", 0);
  writeLCD("Stopwatch", 1);
  delay(2000);
  // reset value and starting counting
  resetStopWatch();
  total_count = true;
  while(1){    
    // display current times
    clearLCD();
    writeLCD("Stop Watch", 0);
    writeLCD("Lap: " + String(lap_hour) + ":" + String(lap_minute) + ":" + String(lap_sec), 1);
    // get input from user
    mode = readRemoteInput();
    if (mode == 0){
      resetStopWatch();
      break;
    }
    else if (mode == 1) {
      recordLapTime();
      mode = 3;
    }
    else if (mode == 2) {
      resetLapTime();
      mode = 3;
    }
    delay(50);
  }
  
  total_count = false; // stop counting
}


void resetStopWatch() {
  // stopwatch time
  total_sec = 0;
  total_minute = 0;
  total_hour = 0;
  lap_sec = 0;
  lap_minute = 0;
  lap_hour = 0;
}
void recordLapTime() {
  lap_sec = total_sec;
  lap_minute = total_minute;
  lap_hour = total_hour;
}
void resetLapTime(){
  total_sec = 0;
  total_minute = 0;
  total_hour = 0;
}

/*************************************** ISR ****************************************/
// runs every second and used to keep track of time
ISR(TIMER1_COMPA_vect){ // This function runs once every time timer compare matches
  
  // handle watch date and time
  sec += 1;
  if (sec == 60){
    sec = 0;
    minute += 1;
    if (minute == 60){
      minute = 0;
      hour += 1;
      if (hour == 24) {
        hour = 0;
        day += 1;
        if (day == 31) {
          day = 1;
          month += 1;
          if (month == 13) {
            month = 0;
            year += 1;
          }
        }
      }
    }
  }
  // hande stopwatch time
  if (total_count){
    total_sec += 1;
    if (total_sec == 60){
      total_sec = 0;
      total_minute +=1;
      if (total_minute == 60) {
        total_minute = 0;
        total_hour +=1;
      }
    }
  }
  // handle timer time
  if (timer_count){
    timer_sec -= 1;
    if (total_sec == 0){
      total_sec = 59;
      total_minute -=1;
      if (total_minute == 0) {
        total_minute = 59;
        total_hour -=1;
      }
    }
  }
}


void setup() {    
    // set lcd contrast
    analogWrite(6,75);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2); // 16x2 LCD, etc.
    // initialize the pushbutton pins as an inputs:
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    // start ir remote
    irrecv.enableIRIn();
    
    /* Configure interrupt on Timer1 */
    cli(); // disable global interrupts before configuring interrupt
    TCCR1A = 0; // set TCCR1A register to 00000000
    TCCR1B = 0; // set TCCR1B register to 00000000
    OCR1A = 15624; // set compare match register
    /* Notice that (15624+1)*1/16000000*1024 ~ 1 second */
    TCCR1B |= (1 << WGM12); // Set WGM12 bit to 1, turns on CTC mode:
    TCCR1B |= (1 << CS12)|(1 << CS10); // Set CS10 and CS12 bits to 1
    /* Notice that When CS12=1, CS11=0 and CS12=1, pre-scaler is 1024 */
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt:
    sei(); // enable global interrupts after configured interrupt
}
void loop() {
  // get mode from user input
  mode = readRemoteInput();
  digitalWrite(ledPin,LOW);
  // mode 0 - display date and time in mode 1
  if (mode == 0) {
    displayDateTime();
  }
  // mode 1 - set date and time 
  else if (mode == 1) {
    setDateTime();
  }
  // mode 2 - stopWatch
  else if (mode == 2) {
    stopWatch();
  }
  // mode 3 - timer
  else if (mode == 3) {
    timer();
  }
  delay(100);  
}
