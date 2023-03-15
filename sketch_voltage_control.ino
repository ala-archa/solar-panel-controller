
const int analog_pin = A0;    // Naming analog input pin
const char mosfet_pin = 12;   // Transistor control pin  
const char led_pin = 13;      // Indication pin

unsigned int  uart_delay = 0;      // In ms
unsigned long power_on_delay = 1000ul * 1;  // In ms

void setup() {
  Serial.begin(9600);         // Initiating Serial communication
  // Setting output control pins
  pinMode(mosfet_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);

  digitalWrite(mosfet_pin, LOW);
}

void loop() {
  unsigned int adc_input = analogRead (analog_pin); // Analog Values from 0 to 1023
  // reference voltage 5 ~4.98V
  // adc resolution 1024
  // divider values 1k, 3.3k sum ~4.3R
  // block input diode 0.6-0.7V 
  float adc_voltage = adc_input * 4.98/1024 * (994 + 3283)/994 ; // voltage in volts
  float input_voltage = adc_voltage + 0.69; // voltage + 0.69V drop on input diode

  if (input_voltage > 11.0 && power_on_delay == 0)
  {
    // power on system
    digitalWrite(mosfet_pin, HIGH);
    digitalWrite(led_pin, HIGH);
  }
  else if (input_voltage < 8.0)
  {
    // power off system
    digitalWrite(mosfet_pin, LOW);
    digitalWrite(led_pin, LOW);

    power_on_delay = 1000ul * 60 * 5;    // Set delay for power on to 5 minutes. 
  }

  if (uart_delay == 0) // Every ms send data to usb uart interface
  {
    Serial.print("Input ");
    Serial.print(input_voltage);
    Serial.print(" V, power delay ");
    Serial.print(power_on_delay/1000);
    Serial.println(" s");
    uart_delay = 1000;
  }

  // Time 
  if (power_on_delay > 0) // Decrease power delay if present
  {
    power_on_delay -= 1;
  }  

  if (uart_delay > 0) // Decrease uart delay if present
  {
    uart_delay -= 1;
  }

  delay(1);
}
