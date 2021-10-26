#include <TroykaOLED.h>
// создаём объект для работы с дисплеем
// и передаём I²C адрес дисплея
TroykaOLED myOLED(0x3C);

//Зададим пины кнопок
const byte LEFT_BUTTON = 7;
const byte RIGHT_BUTTON = 6;
//Initializing the button states as 1 (HIGH)
volatile byte left_state = HIGH;
volatile byte right_state = HIGH;


//Setting up menu items
byte highlight, last_menu_item;
char *menu[] = {"View", "Generate", "Compare"};

void checkInterface()
{
  if (highlight < last_menu_item) 
    {
      highlight++;
    }
  else
    {
      highlight = 0;
    } 
  }

void drawInterface(){
  myOLED.clearDisplay();
  myOLED.drawCircle(3, 32, 2);
  myOLED.drawLine(122, 32, 127, 32);
  myOLED.drawLine(122, 32, 124, 34);
  myOLED.drawLine(124, 34, 127, 32);
  for (int i =0; i<3;i++)
  {
    if (highlight == i) myOLED.invertText(true);
    myOLED.print(menu[i], 8, i*10);
    myOLED.invertText(false);
    }
  }

void rightButtonISR(){
  right_state = digitalRead(RIGHT_BUTTON);
  }

void leftButtonISR(){
  left_state = digitalRead(LEFT_BUTTON);
  }

void setup() {
  //Built-int?
  pinMode(LED_BUILTIN, OUTPUT);
  // инициализируем дисплей
  myOLED.begin();
  // выбираем шрифт 6×8
  myOLED.setFont(font6x8);
  myOLED.invertText(true);
  myOLED.print("sergey@lourie.info", 0, 0);
  myOLED.invertText(false);
  delay(1000);
  myOLED.clearDisplay();
  //Кнопки
  Serial.begin(9600);
  pinMode(RIGHT_BUTTON, INPUT);
  attachInterrupt(RIGHT_BUTTON, rightButtonISR, FALLING);
  pinMode(LEFT_BUTTON, INPUT);
  attachInterrupt(LEFT_BUTTON, leftButtonISR, FALLING);
  highlight = 0;
  last_menu_item = round(sizeof(menu)/4);
  //I don't know why, but Genuino was giving me quadruple the size of the menu
  delay(1000);
  drawInterface();
}



void loop() {
  if (right_state != HIGH)
  {
    checkInterface();
    drawInterface();
    myOLED.print(String(right_state), 40, 54);
    myOLED.print(String(highlight), 60, 54);
    myOLED.print(String(last_menu_item), 80, 54);
    right_state = HIGH;
    }
  if (left_state != HIGH)
  {
    myOLED.print(String(left_state), 12, 54);
    left_state = HIGH;
    }
}
