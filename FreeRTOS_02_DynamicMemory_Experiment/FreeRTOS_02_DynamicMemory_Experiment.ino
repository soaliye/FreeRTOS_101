/*
  *-----------------| 
  * DYNAMIC MEMORY ALLOCATION EXPERIMENT
  * 
  *
  *-------------------------------
*/

TaskHandle_t staticArrayHandle = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("**************** DALLOC EXPERIMENT ****************");

  xTaskCreatePinnedToCore(staticArray, "Static Array Allocation", 1500, NULL, 1, &staticArrayHandle, 1);

  // Delete setup Task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Delete loop Task
  vTaskDelete(NULL);
}

// TASKS

/*
  * Creates an array of 100 integers, populating the array
  * Every sizeof(int) = 4 bytes
  * for the total array, we need 4 x 100 = 400 bytes
  *
  * The task is given 1 kbytes of stack = 1024 bytes
  * As the minimum is 768 bytes, it means that 768 bytes are going to be used
  * for OVERHEAD: the TCB, Initial Stack Frame and function call overhead.
  *
  * leaving only 256 bytes for the actual content while 400 bytes are needed
  *
  * Plus Dynamic Allocation
*/
void staticArray(void *param){
  for(;;){
    int n = 1;
    int array[100];

    for(int i = 0; i < 100; i++){
      array[i] = n + i;
    }
    Serial.println(array[76]);

    // Display Stack Size Data
    Serial.print("@output: Free Stack Size (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Display Heap Size Data
    Serial.print("@output: Free Heap Size Before Malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Allocating memory for 1024 integers
    int *pointer = (int*)pvPortMalloc(1024 * sizeof(int));
    
    if(pointer == NULL){
      Serial.println("@output: Not Enough Heap!");
    }else{
      for(int i = 0; i < 1024; i++){
        pointer[i] = 7;
      }
    }

    Serial.print("@output: Free Heap Size After Malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free Memory
    vPortFree(pointer);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}