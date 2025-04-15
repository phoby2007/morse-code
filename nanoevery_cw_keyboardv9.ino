#include <LiquidCrystal.h>

//variable
const char* jc[] = {
  "a", "i", "u", "e", "o", "ka", "ki", "ku", "ke", "ko", "sa", "si", "shi", "su", "se", "so", "ta", "ti", "chi", "tu",      //20
  "tsu", "te", "to", "na", "ni", "nu", "ne", "no", "ha", "hi", "hu", "fu", "he", "ho", "ma", "mi", "mu", "me", "mo", "ya",  //20
  "yu", "yo", "ra", "ri", "ru", "re", "ro", "wa", "wo", "nn", "ga", "gi", "gu", "ge", "go", "za", "zi", "ji", "zu", "ze",   //20
  "zo", "da", "di", "du", "de", "do", "ba", "bi", "bu", "be",                                                               //10
  "bo", "pa", "pi", "pu", "pe", "po", "kya", "kyu", "kyo", "sya",                                                           //10
  "syu", "syo", "tya", "tyu", "tyo", "nya", "nyu", "nyo", "hya", "hyu",                                                     //10
  "hyo", "mya", "myu", "myo", "rya", "ryu", "ryo", "fa", "fi", "fe",                                                        //10
  "fo", "gya", "gyu", "gyo", "zya", "ja", "zyu", "ju", "zyo", "jo",                                                         //10
  "dya", "dyu", "dyo", "bya", "byu", "byo", "pya", "pyu", "pyo", "1",                                                       //10
  "2", "3", "4", "5", "6", "7", "8", "9", "0", "(", ")", "-", ",", "."                                                      //14
};                                                                                                                          //japanese_code
const char* ljc[] = {
  "\xb1", "\xb2", "\xb3", "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc", "\xbc", "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc1", "\xc2",
  "\xc2", "\xc3", "\xc4", "\xc5", "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcc", "\xcd", "\xce", "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4",
  "\xd5", "\xd6", "\xd7", "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xa6", "\xdd", "\xb6\xde", "\xb7\xde", "\xb8\xde", "\xb9\xde", "\xba\xde", "\xbb\xde", "\xbc\xde", "\xbc\xde", "\xbd\xde", "\xbe\xde",
  "\xbf\xde", "\xc0\xde", "\xc1\xde", "\xc2\xde", "\xc3\xde", "\xc4\xde", "\xca\xde", "\xcb\xde", "\xcc\xde", "\xcd\xde",
  "\xce\xde", "\xca\xdf", "\xcb\xdf", "\xcc\xdf", "\xcd\xdf", "\xce\xdf", "\xb7\xd4", "\xb7\xd5", "\xb7\xd6", "\xbc\xd4",
  "\xbc\xd5", "\xbc\xd6", "\xc1\xd4", "\xc1\xd5", "\xc1\xd6", "\xc6\xd4", "\xc6\xd5", "\xc6\xd6", "\xcb\xd4", "\xcb\xd5",
  "\xcb\xd6", "\xd0\xd4", "\xd0\xd5", "\xd0\xd6", "\xd8\xd4", "\xd8\xd5", "\xd8\xd6", "\xcc\xb1", "\xcc\xb2", "\xcc\xb4",
  "\xcc\xb5", "\xb7\xde\xd4", "\xb7\xde\xd5", "\xb7\xde\xd6", "\xbc\xde\xd4", "\xbc\xde\xd4", "\xbc\xde\xd5", "\xbc\xde\xd5", "\xbc\xde\xd6", "\xbc\xde\xd6",
  "\xc1\xde\xd4", "\xc1\xde\xd5", "\xc1\xde\xd6", "\xcb\xde\xd4", "\xcb\xde\xd5", "\xcb\xde\xd6", "\xcb\xdf\xd4", "\xcb\xdf\xd5", "\xcb\xdf\xd6", "1",
  "2", "3", "4", "5", "6", "7", "8", "9", "0", "(", ")", "-", ",", "]"
};  //lcd_japanese_display
const int mjc[221] = {
  11211, 21, 221, 12111, 21222, 2122, 12122, 2221, 1211, 1111, 12121, 11212, 11212, 11121, 21112, 1112, 12, 2212, 2212, 2112,
  2112, 21211, 22122, 212, 1212, 2222, 1121, 2211, 1222, 11221, 1122, 1122, 2, 122, 1221, 22121, 1, 12221, 12212, 211,
  12211, 11, 222, 112, 12112, 111, 2121, 121, 2111, 21212, 2122, 22, 12122, 22, 2221, 22, 1211, 22, 1111, 22, 12121, 22, 11212, 22, 11212, 22, 11121, 22, 21112, 22,
  1112, 22, 12, 22, 2212, 22, 2112, 22, 21211, 22, 22122, 22, 1222, 22, 11221, 22, 1122, 22, 2, 22,
  122, 22, 1222, 22112, 11221, 22112, 1122, 22112, 2, 22112, 122, 22112, 12122, 211, 12122, 12211, 12122, 11, 11212, 211,
  11212, 12211, 11212, 11, 2212, 211, 2212, 12211, 2212, 11, 21212, 211, 1212, 12211, 1212, 11, 11221, 211, 11221, 12211,
  11221, 11, 22121, 211, 22121, 12211, 22121, 11, 112, 211, 112, 12211, 112, 11, 1122, 11211, 1122, 21, 1122, 12111,
  1122, 21222, 12122, 22, 211, 12122, 22, 12211, 12122, 22, 11, 11212, 22, 211, 11212, 22, 211, 11212, 22, 12211, 11212, 22, 12211, 11212, 22, 11, 11212, 22, 11,
  2212, 22, 211, 2212, 22, 12211, 2212, 22, 11, 11221, 22, 211, 11221, 22, 12211, 11221, 22, 11, 11221, 22112, 211, 11221, 22112, 12211, 11221, 22112, 11, 21111,
  22111, 22211, 22221, 22222, 12222, 11222, 11122, 11112, 11111, 121121, 212212, 21121, 212121, 212122
};  //morse_japanese_code 1:ツー 2:トン
const int cnc[134] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};  //charcter_number_count

//func
void setup() {
  //initialize
  Serial.begin(115200);  //debug
}

void loop() {
}
