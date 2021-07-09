#include "FastLED.h"
#include <TroykaOLED.h>
#include <Keyboard.h>
//#include <math.h>


// создаём объект для работы с дисплеем
// и передаём I²C адрес дисплея
TroykaOLED myOLED(0x3C);

#define RGB_LED_COUNT 4
#define LED_TYPE WS2812B
// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 2
#define COLOR_ORDER GRB

//Зададим пины кнопок
#define RIGHT_BUTTON A0
#define LEFT_BUTTON A1

// This variable is to control the Uno Slot built-in RGB LED strip
CRGB strip[RGB_LED_COUNT];

//Setting up menu items
int left_vol, right_vol;
byte highlight, last_menu_item;
char *menu[] = {"View", "Generate", "Compare"};

//Setting up interrupts constants for analog reads
const byte adcPin = 0;
volatile int adcReading;
volatile boolean adcDone;
boolean adcStarted;



void setup() {
  // инициализируем дисплей
  myOLED.begin();
  // выбираем шрифт 6×8
  myOLED.setFont(font6x8);
  myOLED.invertText(true);
  myOLED.print("sergey@lourie.info", 0, 0);
  myOLED.invertText(false);
  delay(1000);
  myOLED.clearDisplay();
  // Добавляем ленту.
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(strip, RGB_LED_COUNT);
  //Кнопки
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  highlight = 0;
  last_menu_item = round(sizeof(menu)/2);
  //I don't know why, but Arduino was giving me twice the size of the menu

  //initializing interrupts for ADC
  Serial.begin (115200);
  ADCSRA =  bit (ADEN);                      // turn ADC on
  ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);  // Prescaler of 128
  ADMUX  =  bit (REFS0) | (adcPin & 0x07);    // AVcc and select input port
}

// ADC complete ISR for later interrupt callback
ISR (ADC_vect)
  {
  adcReading = ADC;
  adcDone = true;  
  }  // end of ADC_vect

void loop() {
  for (int i =0; i<3;i++)
  {
    if (highlight == i) myOLED.invertText(true);
    myOLED.print(menu[i], 8, i*10);
    myOLED.invertText(false);
    }
  myOLED.drawCircle(3, 32, 2);
  myOLED.drawLine(122, 32, 127, 32);
  myOLED.drawLine(122, 32, 124, 34);
  myOLED.drawLine(124, 34, 127, 32);

  //from Nick Gammon's site
  // if last reading finished, process it
  if (adcDone)
    {
    adcStarted = false;

    // do something with the reading, for example, print it
    myOLED.print(adcReading, 40, 54);
    delay (500);

    adcDone = false;
    }
    
  // if we aren't taking a reading, start a new one
  if (!adcStarted)
    {
    adcStarted = true;
    // start the conversion
    ADCSRA |= bit (ADSC) | bit (ADIE);
    }    
  left_vol = analogRead(LEFT_BUTTON);
  right_vol = analogRead(RIGHT_BUTTON);
  //TODO  - rewrite this into function
  if (right_vol == 0) {
    highlight++;
    if (highlight == last_menu_item) highlight = 0;
    myOLED.clearDisplay();
    myOLED.print(highlight, 6, 54);
    myOLED.print(last_menu_item, 116, 54 );
    }
}
