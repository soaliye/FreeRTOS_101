/*
  * FreeRTOS QUEUES
  *
*/

#define LED_PIN 2

static const uint8_t qlen = 6;
static QueueHandle_t tQueue;
static QueueHandle_t bQueue;

static const uint8_t command_buffer_len = 255;
static char *command = nullptr;

static uint32_t t = 100;

void setup() {
  // Setup
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("************ QUEUES EXPERIEMENT ************");

  // Pin Setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Create Queues
  tQueue = xQueueCreate(qlen, sizeof(int));
  bQueue = xQueueCreate(qlen, sizeof(char));

  // Attach Tasks
  xTaskCreatePinnedToCore(serialDriver, "Serial I/O", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(LEDDriver, "LED Driver", 1024, NULL, 1, NULL, 1);

  // Delete this task (setup task)
  vTaskDelete(NULL);
}

void loop() {
  // Delete this task (loop task)
  vTaskDelete(NULL);
}

// TASKS
void serialDriver(void *param){
  static uint32_t delay;
  static char _char;
  static char buffer[command_buffer_len];
  static uint8_t index = 0;
  static bool isValid = false;
  static char validCmd[6] = "delay";

  // Set all chars to 0
  memset(buffer, 0, command_buffer_len);

  for(;;){
    // Receive Message
    //xQueueReceive(bQueue, (void*)&message, 0);
    //Serial.println(message);

    // Get Delay from Serial
    if(Serial.available() > 0 && index < command_buffer_len){
      _char = Serial.read();

      // If not end or space
      if(_char != ' ' && _char != '\n'){
        buffer[index] = _char;
        index++;
      }else{

        // If can take the null char
        if(index + 1 < command_buffer_len){
          index++;
          buffer[index] = '\0';
        }else{
          Serial.print("error@: ");
          Serial.println("Command Exceeded buffer size!");
        }
        
        // Allocate Space
        command = (char*)pvPortMalloc(index * sizeof(char));
        configASSERT(command);
        memcpy(command, buffer, index + 1);

        // Log Command
        Serial.print("command@: ");
        Serial.println(command);

        // Compare to see if the command is "delay"
        isValid = strcmp(command, validCmd);
        Serial.print("valid@: ");
        Serial.println(isValid == 0? "True" : "False");

        // Log Delay
        delay = Serial.parseInt();
        Serial.print("delay@: ");
        Serial.println(delay);
        Serial.read(); // Get rid of the '\n\ character

        // RESET Everything
        memset(buffer, 0, command_buffer_len);
        index = 0;
        delay = 0;
      }
    }

    // Send Delay
    //xQueueSend(tQueue, (void*)&delay, 0);

  }
}

void LEDDriver(void *param){
  static uint8_t count = 0;
  static uint16_t superCount = 0;

  for(;;){
    /*
    // Receive Delay
    if(xQueueReceive(tQueue, (void*)&t, 0) == pdTRUE){
      //
    }

    // Send Message
    if(count == 100){
      message = sprintf("Blinked %d times\n", supercount);
      xQueueSend(bQueue, (void*)&message, 0);
      count = 0;
    }

    // LED Toggle
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(t / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(t / portTICK_PERIOD_MS);
    count++;
    superCount++;
    */
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}




// CODE GARAGE
/*
void serialDriver(void *param){
  static uint32_t delay;
  static char _char;
  static char buffer[command_buffer_len];
  static uint8_t index = 0;
  static bool isValid = false;
  static char validCmd[6] = "delay";

  // Set all chars to 0
  memset(buffer, 0, command_buffer_len);

  for(;;){
    // Receive Message
    //xQueueReceive(bQueue, (void*)&message, 0);
    //Serial.println(message);

    // Get Delay from Serial
    if(Serial.available() > 0 && index < command_buffer_len){
      _char = Serial.read();

      // If not end or space
      if(_char != ' ' && _char != '\n'){
        buffer[index] = _char;
        index++;
      }else{

        // If can take the null char
        if(index + 1 < command_buffer_len){
          index++;
          buffer[index] = '\0';
        }else{
          Serial.print("error@: ");
          Serial.println("Command Exceeded buffer size!");
        }
        
        // Allocate Space
        command = (char*)pvPortMalloc(index * sizeof(char));
        configASSERT(command);
        memcpy(command, buffer, index + 1);

        // Log Command
        Serial.print("command@: ");
        Serial.println(command);

        // Compare to see if the command is "delay"
        isValid = strcmp(command, validCmd);
        Serial.print("valid@: ");
        Serial.println(isValid == 0? "True" : "False");

        // Log Delay
        delay = Serial.parseInt();
        Serial.print("delay@: ");
        Serial.println(delay);
        Serial.read(); // Get rid of the '\n\ character

        // RESET Everything
        memset(buffer, 0, command_buffer_len);
        index = 0;
        delay = 0;
      }
    }

    // Send Delay
    //xQueueSend(tQueue, (void*)&delay, 0);

  }
}
*/