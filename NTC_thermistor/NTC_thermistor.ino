/*


*/
#define INTERVAL_MS 50 // Print interval
#define ZERO_CELSIUS 273.15 // In kelvin
#define ROOM_TEMP 298.15 // In kelvin
#define PAIR_RESISTOR 20000.0 // Pair resistor pull up
#define THERMISTER_R0 100000.0
#define THERMISTER_BETA 4400.0

void setup() {
  Serial.begin(115200);

}
void loop() {
  static unsigned long timer = millis();
  static float filtered_value = thermistor_read(analogRead(A0));
  if(millis() - timer >= INTERVAL_MS){
    timer += INTERVAL_MS;
    float new_value = thermistor_read(analogRead(A0));
    float alpha = 0.1;
    filtered_value = (1.0-alpha)*filtered_value + alpha*new_value;// More smooth result
    float upper_temp = thermistor_read(float(analogRead(A0))+0.5);
    float lower_temp = thermistor_read(float(analogRead(A0))-0.5);
    float accuracy = abs(upper_temp - lower_temp);
    Serial.print("raw\t");
    Serial.print(new_value);
    Serial.print("\tfilter\t");
    Serial.print(filtered_value);
    Serial.print("\t error +-");
    Serial.println(accuracy);
    
    
  }
  
}

float thermistor_read(float raw_sensor_val){
    //function take 440us to compute
    float voltage = raw_sensor_val*5.0/1024.0;
    float current = (5.0-voltage)/PAIR_RESISTOR;
    float thermistor_resistance = voltage/current;
    float temp = 1.0/(1.0/ROOM_TEMP + (1.0/THERMISTER_BETA)*log(thermistor_resistance/THERMISTER_R0));// calculate temp in kelvin
    return temp-ZERO_CELSIUS; // Convert to celsius
}

