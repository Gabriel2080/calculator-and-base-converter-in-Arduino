#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 4, 3, 2, A4, A5);


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','=','/'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
int LCDRow = 0;


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int MAX_DEC_LENGTH = 3;
const int MAX_BIN_LENGTH = 3; 

enum { DECIMAL_CALCULUS,
 BINARY_CALCULUS,
 DECIMAL_TO_BINARY,
 BINARY_TO_DECIMAL} mode = DECIMAL_CALCULUS;

char key;

long first = 0;
long second = 0;
double total = 0;
char operacao;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(LCDRow, 0);
}
  
void loop(){
  Serial.print("Entrando no loop ");
  Serial.print("[");
  Serial.print(mode);
  Serial.print("] ");
  
  switch(mode){
   case DECIMAL_CALCULUS: decimal_calculator(); break;
   case BINARY_CALCULUS: binary_calculator(); break;
   case DECIMAL_TO_BINARY: decimal_to_binary(); break;
   case BINARY_TO_DECIMAL: binary_to_decimal(); break;
  } 
}

void decimal_calculator(){
  char stop = 'C';
  while (stop == 'C'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calculadora dec");
    decimal_expression(stop);
  }
  change_mode(); 
}

void binary_calculator(){
  char stop = 'C';
  while (stop == 'C'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calculadora bin");
    binary_expression(stop);
  }
  change_mode(); 
}

void decimal_to_binary(){
  char stop = 'C';
  while (stop == 'C'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dec to bin");
    dec_to_bin_expression(stop);
  }
  change_mode();
}

void binary_to_decimal(){
  char stop = 'C';
  while (stop == 'C'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("bin to dec");
    bin_to_dec_expression(stop);
  }
  change_mode();
}

void decimal_expression(char& stop){
  first = 000;
  second = 0;
  total = 0;
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < MAX_DEC_LENGTH; i++) {
    key = keypad.getKey();
    lcd.setCursor(0, 1);
    while (!(key >= '0'&& key <= '9') && 
           (key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C') && !(key == '=' && i == 0)){
      key = keypad.getKey();
       
    }
    
    if(key == '=' || key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C'){
      break;
    }else {
      first = first * 10 + (key - '0');
      lcd.print(first);
    }
    
  }
  
  if(key == '='){
    stop = 1;
  }

  if(key != '=' && key != 'C'){
    lcd.setCursor(3, 1);
    while ((key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C')){
      key = keypad.getKey();
    }
    lcd.print(key);

    if((key != '+') || (key != '-') || (key != '/') || (key != '*')){operacao = key;}

    for (int i = 0; i < MAX_DEC_LENGTH; i++) {
      key = keypad.getKey();
      lcd.setCursor(4, 1);
      while (!(key >= '0'&& key <= '9') && 
             (key != 'C') && (key != '=')){
        key = keypad.getKey();
        if(key == '='){break;}
        else if(key == 'C'){break;}
      }
      if(key == '='){break;}
      else if(key == 'C'){break;}
      second = second * 10 + (key - '0');
      lcd.print(second);
    }

    if(operacao == '+') {
      lcd.setCursor(7,1);
      lcd.print('=');
      total = first + second;
      lcd.print(total);
    }

    else if(operacao == '-') {
      lcd.setCursor(7,1);
      lcd.print('=');
      total = first - second;
      lcd.print(total);
    }

    else if(operacao == '*') {
      lcd.setCursor(7,1);
      lcd.print('=');
      total = first * second;
      lcd.print(total);
    }

    else if(operacao == '/') {
      lcd.setCursor(7,1);
      lcd.print('=');
      if(second == 0){
        lcd.setCursor(0,1);
        lcd.print("Can not / by 0");
      }else{
        total = first / second;
        lcd.print(total);
      }
    }
    while (key != 'C'){
        key = keypad.getKey();
    }
  }
}

void binary_expression(char& stop){
  first = 0;
  second = 0;
  total = 0;
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < MAX_DEC_LENGTH; i++) {
    key = keypad.getKey();
    lcd.setCursor(0, 1);
    while (!(key >= '0'&& key <= '1') && 
           (key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C') && !(key == '=' && i == 0)){
      key = keypad.getKey();
       
    }
    
    if(key == '=' || key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C'){
      break;
    }else {
      first = first << 1;
      first = bitWrite(first, 0, (key - '0'));
      lcd.print(bitWrite(first, 0, (key - '0')),BIN);
      Serial.println(first);
    }
    
  }
  
  if(key == '='){
    stop = 1;
  }

  if(key != '=' && key != 'C'){
    lcd.setCursor(3, 1);
    while ((key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C')){
      key = keypad.getKey();
    }
    lcd.print(key);

    if((key != '+') || (key != '-') || (key != '/') || (key != '*')){operacao = key;}

    for (int i = 0; i < MAX_DEC_LENGTH; i++) {
      key = keypad.getKey();
      lcd.setCursor(4, 1);
      while (!(key >= '0'&& key <= '1') && 
             (key != 'C') && (key != '=')){
        key = keypad.getKey();
        if(key == '='){break;}
        else if(key == 'C'){break;}
      }
      if(key == '='){break;}
      else if(key == 'C'){break;}
      second = second << 1;
      second = bitWrite(second, 0, (key - '0'));
      lcd.print(bitWrite(second, 0, (key - '0')),BIN);
      Serial.println(second);
    }

    if(operacao == '+') {
      lcd.setCursor(7,1);
      lcd.print('=');
      int total = first + second;
      Serial.println(total);
      Serial.println(total, BIN);
      
      lcd.print(total, BIN);
    }

    else if(operacao == '-') {
      lcd.setCursor(7,1);
      lcd.print('=');
      int total = first - second;
      Serial.println(total);
      Serial.println(total, BIN);
      
      lcd.print(total, BIN);
    }

    else if(operacao == '*') {
      lcd.setCursor(7,1);
      lcd.print('=');
      int total = first * second;
      Serial.println(total);
      Serial.println(total, BIN);
      
      lcd.print(total, BIN);
    }

    else if(operacao == '/') {
      lcd.setCursor(7,1);
      lcd.print('=');
      if(second == 0){
        lcd.setCursor(0,1);
        lcd.print("Can not / by 0");
      }else{
        int total = first / second;
        Serial.println(total);
        Serial.println(total, BIN);

        lcd.print(total, BIN);
      }
    }
    while (key != 'C'){
        key = keypad.getKey();
    }
  }
}

void dec_to_bin_expression(char& stop){
  first = 0;

  lcd.setCursor(0, 1);
  for (int i = 0; i < MAX_DEC_LENGTH; i++) {
    key = keypad.getKey();
    lcd.setCursor(0, 1);
    while (!(key >= '0'&& key <= '9') && 
           (key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C') && !(key == '=' && i == 0)){
      key = keypad.getKey();

    }

    if(key == '=' || key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C'){
      break;
    }else {
      first = first * 10 + (key - '0');
      lcd.print(first);
    }
  }

  if(key == '='){
    stop = 1;
  }
  if(key != '='){
    lcd.print('=');
  }
  
  while (!(key == '=' || key == '+' || key == '-' || key == '*' || key == '/' || key == 'C')){
    key = keypad.getKey();
    if(key == '+'){break;}
    else if(key == 'C'){break;}
  }
  if((key == '=' || key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C')){
    lcd.print('>');
    lcd.print(first,BIN);
  }
  
  while (key != '=' && key != 'C'){
        key = keypad.getKey();
    }
}

void bin_to_dec_expression(char& stop){
  first = 0;

  lcd.setCursor(0, 1);
  for (int i = 0; i < MAX_DEC_LENGTH; i++) {
    key = keypad.getKey();
    lcd.setCursor(0, 1);
    while (!(key >= '0'&& key <= '1') && 
           (key != '+') && (key != '-') && 
           (key != '/') && (key != '*') && 
           (key != 'C') && !(key == '=' && i == 0)){
      key = keypad.getKey();

    }

    if(key == '=' || key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C'){
      break;
    }else {
      first = first << 1;
      first = bitWrite(first, 0, (key - '0'));
      lcd.print(bitWrite(first, 0, (key - '0')),BIN);
      Serial.println(first);
    }
  }

  if(key == '='){
    stop = 1;
  }
  if(key != '='){
    lcd.print('=');
  }

  while (!(key == '=' || key == '+' || key == '-' || key == '*' || key == '/' || key == 'C')){
    key = keypad.getKey();
    if(key == '+'){break;}
    else if(key == 'C'){break;}
  }
  if((key == '+'
       || key == '-' || key == '*'
       || key == '/' || key == 'C')){
    lcd.print('>');
    lcd.print(first);
  }
  
  while (key != '=' && key != 'C'){
        key = keypad.getKey();
    }
}

void change_mode(){
  switch(mode)
  {
    case DECIMAL_CALCULUS: mode = BINARY_CALCULUS; break;
    case BINARY_CALCULUS: mode = DECIMAL_TO_BINARY; break;
    case DECIMAL_TO_BINARY: mode = BINARY_TO_DECIMAL; break;
    case BINARY_TO_DECIMAL: mode = DECIMAL_CALCULUS; break;
  } 
}