/*
 * Двухтональный генератор на базе ардуино нано (С) UA6EM
 * V-1.01 06.01.2019 (без использования таймеров)
 * V-1.02 07.01.2019 (Используя таймер 1 формируем частоту 2250 Гц на Pin10)
 * Prescaler: 1; MaxValue: 3555; Frequency: 2249.72Hz; Diff: 0.28Hz
 * http://arduino.ru/forum/programmirovanie/etyudy-dlya-nachinayushchikh-blink-i-bez-delay-i-bez-millis#comment-163979
 * 
 */

unsigned long freq_225;
unsigned long freq_1800;
unsigned long freq_2250;
byte flag_225 = 0;
byte delitel = 0;

enum Prescalers {
   PRESCALER_1 = 1, PRESCALER_8 = 2, PRESCALER_64 = 3, PRESCALER_256 = 4, PRESCALER_1024 = 5
};


// функция установки параметров таймера для выбранной частоты (в скобках номер таймера из калькулятора)
//     (1) Prescaler: 1; MaxValue: 4443; Frequency: 1800.18Hz; Diff: 0.18Hz
//     (2) Prescaler: 32; MaxValue: 138; Frequency: 1798.56Hz; Diff: 1.44Hz
//     (1) Prescaler: 1; MaxValue: 3555; Frequency: 2249.72Hz; Diff: 0.28Hz
//     (2) Prescaler: 32; MaxValue: 110; Frequency: 2252.25Hz; Diff: 2.25Hz
//     (1) Prescaler: 1; MaxValue: 35555; Frequency: 225.00Hz; Diff: 0.00Hz
// (0,1,2) Prescaler: 256; MaxValue: 138; Frequency: 224.82Hz; Diff: 0.18Hz

void set_Timer1(){   //Freq=2250Hz
   uint8_t prescaler = PRESCALER_1;
   uint16_t topValue = 3555;
   pinMode(10, OUTPUT);
   TCCR1A = 0x10;              // Инвертирование пина 10 по сравнению
   TCCR1B = 0x08 | prescaler;  // Установить СТС режим и делитель частоты
   OCR1A = topValue;           // установить TOP равным topValue

   // set up timer with prescaler = 64 and CTC mode
   // TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
   // initialize counter
   // TCNT1 = 0;
   // initialize compare value
   // OCR1A = 24999;
}

/*
void set_Timer2(){   //Freq=1800Hz
   uint8_t prescaler2 = 32;
   uint16_t topValue2 = 138;
   pinMode(11, OUTPUT);
   TCCR2A = 0x10;               // Инвертирование пина 11 по сравнению        ??? Уточнить
   TCCR2B = 0x08 | prescaler2;  // Установить СТС режим и делитель частоты    ??? Уточнить
   OCR2A = topValue2;           // установить TOP равным topValue             ??? Уточнить
   
   // set up timer with prescaler = 256
   // TCCR2 |= (1 << CS22)|(1 << CS21);
   // initialize counter
   // TCNT2 = 0;
   // set up timer with prescaler = 1024
   // TCCR2 |= (1 << CS22)|(1 << CS21)|(1 << CS20);
   // Делители 0-0(таймер остановлен); 1-1;2-8;3-32;4-64;5-128;6-256;7-1024

   // Ещё код для таймера 2 
   // TCCR2A = (1<<WGM21);    
   // TCCR2B = (1<<CS20);    
   // OCR2A = 0x80; 
   // TIMSK2 = (1<<OCIE2A); 
   
}
*/


void start_Timer1(){
  TCCR1A = 0x10;              // Инвертирование пина 10 по сравнению
}

// Процедура для организации синхронизирующей частоты 225Гц от таймера частоты 2250
/*
void sync_Timer(){    if (digitalRead(10)==HIGH && flag_225 == 0 ){
    digitalWrite(13,HIGH);
        } else {
    digitalWrite(13,LOW);}
}
*/

// Процедура для организации синхронизирующей частоты 225Гц от частоты 2250hz (Pin9)
void sync_Timer(){ 
  if(delitel == 10){
    digitalWrite(13, !digitalRead(13));
    delitel = 0;
  }
}

void setup() {
  // Запустим таймер для частоты 2250 (Выход Pin 10)
  set_Timer1();
  start_Timer1();
  
  pinMode(7, OUTPUT);  // Freq 225Hz
  pinMode(8, OUTPUT);  // Freq 1800Hz
  pinMode(9, OUTPUT);  // Freq 2250Hz
  pinMode(13, OUTPUT); // Freq 225Hz Via SYNC Freq 2250Hz
  
  digitalWrite(13,HIGH);
  digitalWrite(9,HIGH);
  freq_2250 = micros();
  digitalWrite(8,HIGH);
  freq_1800 = micros();
  digitalWrite(7,HIGH);
  freq_225 = micros();
}

void loop() {
  if(micros() - freq_2250 >= 222){
  digitalWrite(9,!digitalRead(9));
  delitel++;
  freq_2250 = micros();
  }
  sync_Timer(); 
   
  if(micros() - freq_1800 >= 277){
  digitalWrite(8,!digitalRead(8));
  freq_1800 = micros();
  }

  if(micros() - freq_225 >= 2222){
  digitalWrite(7,!digitalRead(7));
  freq_225 = micros();
  }
}
/* END */
