
#define LED_PIN 2
uint32_t blinkingDelay = 100;
uint32_t newVal;

TaskHandle_t LEDHandle = NULL;
TaskHandle_t InputHandle = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***************** I/O EXPERIMENT ******************");

  pinMode(LED_PIN, OUTPUT);

  xTaskCreatePinnedToCore(toggleLED, "Toggle LED", 1024, NULL, 1, &LEDHandle, 1);
  xTaskCreatePinnedToCore(serialInput, "Serial Input", 1024, NULL, 2, &InputHandle, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

}


// Tasks
void toggleLED(void *param){
  for(;;){
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(blinkingDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(blinkingDelay / portTICK_PERIOD_MS);
  }
}

void serialInput(void *param){
  for(;;){
    if(Serial.available() > 0){
      newVal = Serial.parseInt();
      blinkingDelay = newVal > 0? newVal : blinkingDelay;
      Serial.print("New Blinking Delay: ");
      Serial.println(blinkingDelay);
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}