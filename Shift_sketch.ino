
#include "IRremote.h"//подключаем библиотеку
IRrecv irrecv(3); // указываем вывод, к которому подключен приемник

const int data_pin = 5;
const int sh_pin = 7;
const int st_pin = 6;

int digit = 0;
/*
 * нумерация сегментов в соответствии с выходами регистра
 * +--5--+
 * |     |
 * 6     4
 * |     |
   +--7--+
 * |     |
 * 1     3
 * +--2--+
 * 
 * 
 */
byte digits[10] = {
  // 1 соответствует зажигаемому сегменту
  0b11111100, // 0
  0b00110000, // 1
  0b11011010, // 2
  0b01111010, // 3
  0b00110110, // 4
  0b01101110, // 5
  0b11101110, // 6
  0b00111000, // 7
  0b11111110, // 8
  0b01111110, // 9
};

//----вызов класса decode_results из подключеной библиотеки----
//
decode_results results;

/*http://robotclass.ru/tutorials/arduino-shift-register/
   Для заполнения регистра мы добавили специальную функцию shift.
   Функция в цикле опускает уровень на SH_CP, затем устанавливает нужный бит на выводе DS, и в конце поднимает SH_CP.
   Наконец, после завершения цикла, мы делаем импульс на ST_CP для передачи битов на выходы регистра.
   Всё как во временной диаграмме, которую мы разобрали ранее. В цикле мы должны перебрать все биты числа v.
   Сначала взять первый бит, передать его в регистр. Потом второй, потом третий, и т.д. Как это сделать?
   Разберем вот такую конструкцию:
   v & (1 << i)
  переменная v — это восемь бит, которые мы хотим записать в регистр.
  Операция 1<<i сдвигает единицу на i позиций влево. К примеру, 1<<3 соответствует числу 0000 1000 в бинарном виде.
  1<<4 даст уже  0001 0000, и так далее. Назовем результат этой операции трафаретом. Операция & — это бинарная операция «И».
  С помощью неё мы как бы накладываем трафарет на число v. Допустим, число v = 0111 0110.
  На третьем шаге цикла трафарет будет равен 0000 0100. Теперь наложим трафарет и получим:
  0111 0110 & 0000 0100 = 0000 0100 = истина
  Тут следует уточнить, что в языке C++, как и во многих других языках,
  любое отличное от нуля число интерпретируется как «истина» или логическая единица.
  digitalWrite интерпретирует эту логическую единицу как высокий уровень сигнала, что нам и требуется!
  На следующей итерации трафарет уже будет равен 0000 1000, наложим теперь его на v:
  0111 0110 & 0000 1000 = 0000 0000 = ложь
  В общем, мы двигаем единичку в трафарете, и каждый раз накладываем полученный трафарет на число.
*/
void shift(byte digitBit) {
  for (int i = 1; i <= 8; i++) {
    digitalWrite(sh_pin, LOW);//опускаем уровень sh_cp
    digitalWrite(data_pin, digitBit & (1 << i ));
    digitalWrite(sh_pin, HIGH);//поднимаем уровень sh_cp
  }
  digitalWrite(st_pin, HIGH); //подаем импульс на ST_CP для передачи битов на выходы регистра
  digitalWrite(st_pin, LOW);//возвращаем начальное состояние
}

void setup() {
  Serial.begin(9600); //запускаем и выставляем скорость COM порта, для отладки
  pinMode(data_pin, OUTPUT);
  pinMode(sh_pin, OUTPUT);
  pinMode(st_pin, OUTPUT);
  irrecv.enableIRIn(); // запускаем прием
  digit = 0;
  shift( digits[digit] );
}

void loop() {

  if (irrecv.decode( &results )) { // если данные пришли

    switch (results.value) {


      case 0xFF30CF://кнопка 1
        digit = 1;
        shift( digits[digit] );
        break;
      case 0xFF18E7://кнопка 2
        digit = 2;
        shift( digits[digit] );
        break;
      case 0xFF7A85://кнопка 3
        digit = 3;
        shift( digits[digit] );
        break;
      case 0xFF10EF://кнопка 4
        digit = 4;
        shift( digits[digit] );
        break;
      case 0xFF38C7://кнопка 5
        digit = 5;
        shift( digits[digit] );
        break;
      case 0xFF5AA5://кнопка 6
        digit = 6;
        shift( digits[digit] );
        break;
      case 0xFF42BD://кнопка 7
        digit = 7;
        shift( digits[digit] );
        break;
      case 0xFF4AB5://кнопка 8
        digit = 8;
        shift( digits[digit] );
        break;
      case 0xFF52AD://кнопка 9
        digit = 9;
        shift( digits[digit] );
        break;
      case 0xFF6897://кнопка 0
        digit = 0;
        shift( digits[digit] );
        break;
    }
    //------Отладка------
    //----коды кнопок----
    // Serial.print("Код кнопки: ");
    //Serial.println( results.value, HEX );
    //----Конец отладки-----

    irrecv.resume(); // принимаем следующую команду
  }
}
