#include <hidboot.h>
#include <usbhub.h>
#include <string.h>
#include <LiquidCrystal.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <SoftwareSerial.h>

//define
#define LINE 2
#define ALL 1
#define NOT 0
#define transpin 6
#define max_wpm 41

//class
class KbdRptParser : public KeyboardReportParser {
  void PrintKey(uint8_t mod, uint8_t key);

protected:
  void OnControlKeysChanged(uint8_t before, uint8_t after);

  void OnKeyDown(uint8_t mod, uint8_t key);
  void OnKeyPressed(uint8_t key);
};

//instance
USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);
KbdRptParser Prs;
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);
SoftwareSerial urat(7, 8);

//variable
char prskey = '\0';
const char ec[42] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '/', '?', ',', '.', '@' };  //english_code
const int mec[42] = {
  21, 1222, 1212, 122, 2, 2212, 112, 2222, 22, 2111,
  121, 2122, 11, 12, 111, 2112, 1121, 212, 222, 1,
  221, 2221, 211, 1221, 1211, 1122, 21111, 22111, 22211, 22221,
  22222, 12222, 11222, 11122, 11112, 11111, 122221, 12212, 221122, 112211, 212121, 211212
};  //morse_english_code トン:0,ツー:1

int codeindex[100] = {};
int charcounter = 0, wpm = 20, trans_check = 0, code_length = 0;
bool shift_change_flag = false, mode_sw = false, every_check = false, prskey_flag = false, initialize_flag = true, jpmode_change_flag = false, engmode_change_flag = true;
String code = "", code_tmp = "";

//func
void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  uint8_t c = OemToAscii(mod, key);
  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    if (shift_change_flag) shift_change_flag = false;
    else shift_change_flag = true;
  }
}

void KbdRptParser::OnKeyPressed(uint8_t key) {
  if (!initialize_flag) {
    prskey = key;
    prskey_flag = true;
  }
}

void display(int line, int number, String code, int clear_check) {
  if (clear_check == 1) lcd.clear();
  else if (clear_check == 2) {
    lcd.setCursor(number, line);
    lcd.print("                ");
  }
  lcd.setCursor(number, line);
  lcd.print(code);
}

void trans() {
  int lag = 1200 / wpm;
  String morse;
  for (int i = 0; i < code_length; i++) {
    morse = (String)mec[codeindex[i]];
    for (int l = 0; l < morse.length(); l++) {
      if (morse.charAt(l) == '1') {
        digitalWrite(transpin, HIGH);
        delay(lag * 3);
        digitalWrite(transpin, LOW);
      } else {
        digitalWrite(transpin, HIGH);
        delay(lag);
        digitalWrite(transpin, LOW);
      }
      if (l != morse.length() - 1) delay(lag);
    }
    if (i != code_length - 1) delay(lag * 3);
  }
}

void setup() {
  //initialize
  pinMode(transpin, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  Serial.begin(115200);  //debug
  HidKeyboard.SetReportParser(0, &Prs);

  if (Usb.Init() == -1) display(0, 0, "BAD CONNECTION", ALL);
  else display(0, 0, "KPL2025", ALL);
  initialize_flag = false;
}

void loop() {
  //read POT
  wpm = (int)map(analogRead(A0), 0, 1023, 1, max_wpm);
  //change_mode
  if (digitalRead(A0) == 0) {
    mode_sw = true;
    if (jpmode_change_flag == false) {
      display(0, 0, "", ALL);

      engmode_change_flag = false;
      jpmode_change_flag = true;
    } else {
      display(1, 6, "\xc6\xce\xdd pwm:", NOT);
      if (wpm < 10) display(1, 15, "  ", NOT);
      display(1, 14, (String)wpm, NOT);
    }
  } else {
    mode_sw = false;
    if (engmode_change_flag == false) {
      display(0, 0, "", ALL);

      jpmode_change_flag = false;
      engmode_change_flag = true;
    } else {
      display(1, 6, "eng pwm:", NOT);
      if (wpm < 10) display(1, 15, "  ", NOT);
      display(1, 14, (String)wpm, NOT);
    }
  }

  //mode check & operate
  if (mode_sw) {
    code = "";
    //every check
    if (!every_check) {
      int receive;
      urat.write(true);
      delay(100);
      receive = urat.read();
      if (receive == -1) display(0, 0, "INSERT NANOEVERY", false);
      else every_check = true;
    }
    if(prskey_flag == true){
      
    }
  } else {
    code_length = code.length();
    if (prskey_flag == true) {
      //enter
      if (prskey == 10 || prskey == 13) {
        if (trans_check == 0) {
          //final check
          trans_check = 1;
          initialize_flag = true;
          display(0, 3, "TRANSCHECK", LINE);
          initialize_flag = false;
        } else if (trans_check == 1) {
          //transport
          trans();
          trans_check = 0;
          code = "";
        }
      } else {
        trans_check = 0;
        //delete
        if (prskey == 42 && !shift_change_flag) {
          if (code != "") code.remove(code_length, 1);
          code_tmp = code;
          if (code_tmp.length() > 16) code_tmp.remove(0, code_tmp.length() - 16);
          display(0, 0, code_tmp, LINE);
          //delete all
        } else if (prskey == 42 && shift_change_flag) {
          code = "";
          code_tmp = code;
          if (code_tmp.length() > 16) code_tmp.remove(0, code_tmp.length() - 16);
          display(0, 0, code_tmp, LINE);
        } else {
          //add new char
          if (prskey == '[') prskey = '@';
          if (code_length == 100) {
            initialize_flag = true;
            display(1, 0, "full_code", LINE);
            delay(1000);
            initialize_flag = false;
          } else {
            code.concat(prskey);
            int keyindex;
            for (int i = 0; i < sizeof(ec) / sizeof(ec[0]); i++) {
              if (prskey == ec[i]) {
                keyindex = i;
                codeindex[code_length - 1] = keyindex;
                break;
              }
            }
            code_tmp = code;
            if (code_tmp.length() > 16) code_tmp.remove(0, code_tmp.length() - 16);
            display(0, 0, code_tmp, LINE);
          }
        }
      }
      prskey_flag = false;
    }
  }
}
