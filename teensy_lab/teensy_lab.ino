/* For CAN */
#include <FlexCAN.h>
FlexCAN CANbus0(250000);
static CAN_message_t rx_msg;

/* For OLED display */
#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 18
#define SCL_PIN 19
Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN);

/* On-board LED */
int on_board_led_pin = 13;
int led_state = LOW;

/* RGB LED pins */
int led_pin_red = 37;
int led_pin_green = 36;
int led_pin_blue = 38;

/* TX2's IP address. Receiving from CAN */
unsigned short ip_addr[4] = {0,0,0,0};

/* For blink_LED */
IntervalTimer myTimer_blink_led;
int blinking_period = 500000;  // toggle every 0.5 sec

/* For CAN_check */
IntervalTimer myTimer_CAN_check;
int CAN_check_period = 20000; // 50 Hz


void blink_LED() {
  //-----------------
  // Your code: Toggle the on-board LED. Use 'digitalWrite' function 
  //-----------------
  if (led_state == LOW) led_state = HIGH;
  else led_state = LOW;
  digitalWrite(on_board_led_pin, led_state);
}


void CAN_check() {
  int ret = Can0.available();  
  if (ret > 0) {
    //-----------------
    // Your code: read CAN message from Can0. 
    // Store the IP address of TX2 to the global variable 'ip_addr'.
    // The CAN identifier for the IP message is 0x2
    //-----------------
    Can0.read(rx_msg);
    if (rx_msg.id = 0x2) {
      for (int i = 0; i < 4; i++)
        ip_addr[i] = rx_msg.buf[i];
    }
  }
}

void loop() {
  display.clear();
  display.setCursor(0,0);

  display.print("IP: ");
  display.print(ip_addr[0]);
  display.print(".");
  display.print(ip_addr[1]);
  display.print(".");
  display.print(ip_addr[2]);
  display.print(".");
  display.println(ip_addr[3]);
  display.println();
  
  display.update();
  
  delay(100);  
}

void set_rgb_led(int red, int green, int blue) {
  analogWrite(led_pin_red, red);
  analogWrite(led_pin_green, green);
  analogWrite(led_pin_blue, blue);  
}

void setup() {
  //-----------------
  // Your code
  // 1. Set the PWM resolution to 16 bits
  // 2. Set the PWM frequency of the RGB LED pins to 100 Hz
  //-----------------
  
   
  pinMode(led_pin_red, OUTPUT);
  pinMode(led_pin_green, OUTPUT);
  pinMode(led_pin_blue, OUTPUT);  

  set_rgb_led(65535, 0, 0); //Full Red

  /* on-board LED: high/low */
  pinMode(on_board_led_pin,OUTPUT); 
  digitalWrite(on_board_led_pin,HIGH); 

  /* Initialize CAN */
  Can0.begin();

  /* Initialize OLED display */
  display.initialize();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //-----------------
  // Your code: make 'CAN_check' periodic (priority = 1) 
  //----------------- 
  myTimer_CAN_check.priority(1);
  myTimer_CAN_check.begin(CAN_check, CAN_check_period);

  //-----------------
  // Your code: make 'blink_LED' periodic (priority = 200) 
  //----------------- 
  myTimer_blink_led.priority(200);
  myTimer_blink_led.begin(blink_LED, blinking_period);
}
