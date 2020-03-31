// EMBEDDED SYSTEMS PROJECT - REACTION TIMER

#include <avr/wdt.h> 
#include <LiquidCrystal.h>

// lcd
const int Contrast=75;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// led and buttons
const int ledPin = 9;
const int buttonPin = 10; 
int button_state = 0;

// states of machine
enum states { startup,
  led_timer,
  reaction_timer,
  timeout,
};

// state of machine
// volatile because changes in ISR
volatile states current_state;

long start_time;
long led_start_time; 
long reaction_time;

void startup_f(void) {
  // get current time since program started in micro seconds 
  start_time = micros();
  // led starts between 2 ~ 4 seconds (in microsecs)
  led_start_time = random(2000000, 4000000);

  reaction_time = 0;
  current_state = led_timer; 
}

void led_timer_f(void) {
  if ((micros() + start_time) >= led_start_time) {
    start_time = micros();
    current_state = reaction_timer; 
  }
}

void reaction_timer_f(void) { 
  digitalWrite(ledPin, HIGH); 
  if (button_state == HIGH) {
    reaction_time = micros() - start_time; 
    digitalWrite(ledPin, LOW); 
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("reaction time="); 
    lcd.setCursor(0, 1); 
    lcd.print(reaction_time);
    delay(2000); lcd.clear(); 
    current_state = startup;
  } 
}

// timeout! Tell the user and reset in 8 seconds
void timeout_f(void) {
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("TIME OUT!"); 
  lcd.setCursor(0, 1); 
  lcd.print("Resetting"); 
  delay(100);
}

void setup() {
  //disable watchdog timer
  wdt_disable();
  
  // lcd setup
  analogWrite(6,Contrast); 
  lcd.begin(16, 2);
  
  // led and button setup
  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT);
  
  current_state = startup;
  
  // setup watchdog timer
  // Keep old prescaler setting to prevent unintentional time-out 
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  
  // enable watchdog timer to 8 second timeout
  wdt_enable(WDTO_8S);
  // set watchdog to trigger an ISR when timeout
  WDTCSR |= (1 << WDIE); 
}

ISR (WDT_vect) { 
  current_state = timeout;
}

void loop() {
  button_state = digitalRead(buttonPin);
  if (current_state == startup) { 
    startup_f();
    wdt_reset();
  }
  else if (current_state == led_timer) {
    led_timer_f();
    wdt_reset();
  }
  else if (current_state == reaction_timer) { 
    reaction_timer_f();
  }
  else if (current_state == timeout) {
    timeout_f();
  }
}