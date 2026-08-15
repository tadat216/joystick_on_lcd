#include <Arduino.h>
#include <LibPrintf.h>

#define UP 2
#define DWN 3
#define LET 4
#define RHT 5
#define MID 6

struct JoystickStatus{
  int x, y, mid;

  bool operator == (const JoystickStatus& other) const {
    return x == other.x && y == other.y && mid == other.mid;
  }

  bool operator != (const JoystickStatus& other) const {
    return x != other.x || y != other.y || mid != other.mid;
  }
};

JoystickStatus cur_st;
JoystickStatus prv_st;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DWN, INPUT_PULLUP);
  pinMode(LET, INPUT_PULLUP);
  pinMode(RHT, INPUT_PULLUP);
  pinMode(MID, INPUT_PULLUP);
}

void updateJoystickStatus(){
  cur_st.y = !digitalRead(UP) ? 1 : (!digitalRead(DWN) ? -1 : 0); 
  cur_st.x = !digitalRead(RHT) ? 1 : (!digitalRead(LET) ? -1 : 0); 
  cur_st.mid =  !digitalRead(MID) ? 1 : 0;
}

void loop() {
  updateJoystickStatus();
  if(cur_st != prv_st){
    printf("x: %d, y: %d, mid: %d\n", cur_st.x, cur_st.y, cur_st.mid);
    prv_st = cur_st;
  }
  delay(50);
}
