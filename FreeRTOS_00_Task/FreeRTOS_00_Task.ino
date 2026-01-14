// Force the use of only one core
// aap_cpu is an already defined variable
//---------------------------------------
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


#define LED_PIN 2

TaskHandle_t toggleLEDHandle = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  xTaskCreatePinnedToCore(toggleLED, "LED Toglle", 1024, NULL, 1, &toggleLEDHandle, app_cpu);
  //vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}

// TASKS
void toggleLED(void *param){
  for(;;){
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}