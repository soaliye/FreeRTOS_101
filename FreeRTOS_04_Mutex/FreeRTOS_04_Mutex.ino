/*------------------- MUTEX
  * 
  *
  *
*/

// FreeRTOS Generalizes Mutexes and Semaphores are just Semaphores
static SemaphoreHandle_t mutex;

// Shared resource
static uint8_t counter;

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("**************** MUTEX DEMO - RACE CONDITION **************");

  // Create Mutex
  mutex = xSemaphoreCreateMutex();

  // Attach Tasks
  xTaskCreatePinnedToCore(incrementValue, "Thread1", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(incrementValue, "Thread2", 1024, NULL, 1, NULL, 1);

  // Delete Setup and Loop task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}


// TASKS
void incrementValue(void *param){
  uint8_t loc;

  for(;;){

    // If we can take the mutex
    if(xSemaphoreTake(mutex, 0) == pdTRUE){
      // Increment
      loc = counter;

      if(loc < 254){
        loc++;
      }

      // Waste time
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      counter = loc;

      // Print shared resource
      Serial.print("resource@");
      Serial.print(pcTaskGetName(NULL));
      Serial.print(": ");
      Serial.println(counter);

      // Give back the mutex
      xSemaphoreGive(mutex);
      taskYIELD();
    }
    
    
  }
}