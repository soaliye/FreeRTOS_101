
char myString[] = "FreeRTOS";

static TaskHandle_t printCharsHandle = NULL;
static TaskHandle_t printStarHandle = NULL;

void setup() {
  // put your setup code here, to run once:
  // Setup
  Serial.begin(300); // Very slow, to see it in slow-motion
  delay(1000);
  Serial.println();
  Serial.println("-------------- SCHEDULER EXPERIMENT ---------------");

  // Setup Task Data
  Serial.print("Core: ");
  Serial.print(xPortGetCoreID());
  Serial.print(" ++/++  Priority: ");
  Serial.println(uxTaskPriorityGet(NULL));

  // Attach Tasks
  xTaskCreatePinnedToCore(printCharacters, "Print Chars of a String", 1024, NULL, 1, &printCharsHandle, 1);
  xTaskCreatePinnedToCore(printAsterisks, "Print Askterisks", 1024, NULL, 2, &printStarHandle, 1);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Toggle the High Priority Task
  for(int i = 0; i < 3; i++){
    vTaskSuspend(printStarHandle);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(printStarHandle);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  if(printCharsHandle != NULL){
    vTaskDelete(printCharsHandle);
    printCharsHandle = NULL;
  }
}


// TASKS
void printCharacters(void *param){
  int len = strlen(myString);

  for(;;){
    for(int i = 0; i < len; i++){
      Serial.print(myString[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void printAsterisks(void *param){
  for(;;){
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}