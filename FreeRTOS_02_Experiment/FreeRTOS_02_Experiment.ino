//-------------------| 
//  * EXPERIMENT
//  * First Task gets data from serial up to '\n'
//  * Second Task prints data and frees memory
//-----------------------------------------------------------

bool messageReady = 0;
static const uint8_t bufferSize = 255;
static char *messagePtr = nullptr;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("************** DALLOC EXPERIMENT ***************");

  xTaskCreatePinnedToCore(readSerial, "Read Message", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(printSerial, "Display Message", 1024, NULL, 1, NULL, 1);

  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelete(NULL);
}

// Tasks

void readSerial(void *param){
  char char_;
  uint8_t index = 0;
  char buffer[bufferSize];

  // Empty the buffer
  memset(buffer, 0, bufferSize);

  for(;;){


    if(Serial.available() > 0){
      char_ = Serial.read();

      if(index < bufferSize - 1){
        buffer[index] = char_;
        index++;
      }

      if(char_ == '\n'){
        buffer[index - 1] = '\0';

        if(!messageReady){
          messagePtr = (char*)pvPortMalloc(index * sizeof(char));
          configASSERT(messagePtr);

          memcpy(messagePtr, buffer, index);

          messageReady = 1;
        }

        memset(buffer, 0, bufferSize);
        index = 0;
      }
    }
  }
}

void printSerial(void *param){
  for(;;){
    if(messageReady){
      // Display
      Serial.print("@output: ");
      Serial.println(messagePtr);

      // Free
      vPortFree(messagePtr);
      messagePtr = nullptr;

      // Reset
      messageReady = 0;
    }
  }
}