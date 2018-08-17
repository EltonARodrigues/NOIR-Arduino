long unsigned int init_time = millis();
long unsigned int range_collect_time = 1000;

void setup(){
    Serial.begin(9600);
    Serial.println("INIT");
    Serial.print("&init_time:");
    Serial.print(&init_time);
    Serial.print("\t&range_collect_time: ");
    Serial.print(&range_collect_time);
}

bool has_pass_fifthen_seconds(long unsigned int &init_time, long unsigned int &range_collect_time){
  if ((millis()-init_time) > range_collect_time){
    Serial.println("PASSOU SEC");
    init_time = millis();
    return true;
  }
  Serial.println("FALSE");
  return false;
}

void loop(){
    has_pass_fifthen_seconds(init_time, range_collect_time);
    delay(500);
}