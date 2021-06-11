/*
   DEBER 5.4: FUNCIONES RTOS
   OBJETIVO: REALIZAR UN SEMAFORO EMPLEANDO FUNCIONES DE RTOS
   NOMBRE: RAFAEL FARINANGO
*/

#include <Arduino_FreeRTOS.h> // librería de RTOS

//leds para simular el semaforo
#define ROJO 10
#define AMARI 9
#define VERDE 8

//objeto del gestor tareas
typedef int TaskProfiler;

//definir las tareas
TaskProfiler rojoLEDProfiler;
TaskProfiler amariLEDProfiler;
TaskProfiler verdeLEDProfiler;

TaskHandle_t red_Handle, yellow_Handle, green_Handle; // manejar la prioridad

//contadores para verificar tiempos del semaforo
int contR = 0;
int contA = 0;
int contV = 0;

void setup() {
  Serial.begin(9600);

  xTaskCreate(redLedControllerTask, "Red LED Task", 100, NULL, 1, &red_Handle); //rutina,nombre,memoria,NULL,prioridad,Manejo de prioridad
  xTaskCreate(yellowLedControllerTask, "Yellow LED Task", 100, NULL, 1, &yellow_Handle); //rutina,nombre,memoria,NULL,prioridad,NULL
  xTaskCreate(greenLedControllerTask, "Green LED Task", 100, NULL, 1, &green_Handle);
}


void redLedControllerTask(void *pvParameters) { //tarea 1
  pinMode(ROJO, OUTPUT);    //establece pin rojo como salida
  while (1) {               //realiza un bucle infinito
    contR++;                //contador de segundos
    Serial.println(contR);
    if ((contR > 0) && (contR <= 10)) { //si contadorR esta entre 0 - 10 seg ejecuta:
      digitalWrite(ROJO, HIGH);         //enciende led rojo
      digitalWrite(VERDE, LOW);         //apaga led verde
      vTaskSuspend(yellow_Handle);      //suspende tarea yellow
      vTaskSuspend(green_Handle);       //suspende tarea green
    }
    else if ((contR > 11) && (contR <= 12)) {     //si contadorR esta entre 11 - 12 seg ejecuta:
      vTaskResume(yellow_Handle);                 //reanuda tarea yellow
      //vTaskSuspend(green_Handle);
    }
    else if ((contR > 13) && (contR <= 22)) {     //si contadorR esta entre 13 - 22 seg ejecuta:
      vTaskResume(green_Handle);                  //reanuda tarea green
      vTaskSuspend(yellow_Handle);                //suspende tarea yellow
    }
    else if ((contR > 23) && (contR <= 24)) {     //si contadorR esta entre 23 - 24 seg ejecuta:
      vTaskResume(yellow_Handle);                 //reanuda tarea yellow
      //vTaskSuspend(green_Handle);
    }
    else if (contR == 25) {                       //si contadorR es igual a 25 seg ejecuta:
      contR = 0;                                  //contadorR es igual a cero
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);        //ejecuta un delay cada 1 seg
  }
}


void yellowLedControllerTask(void *pvParameters) { //tarea 2
  pinMode(AMARI, OUTPUT);               //establece pin rojo como salida
  while (1) {                           //realiza un bucle infinito
    //Serial.println("Proceso led amarillo");
    //amariLEDProfiler++;
    //Serial.print("YELLOW TASK:");
    //Serial.println(amariLEDProfiler);

    contA++;                            //contador que sirve solo de guia no cumple ninguna funcion en el semaforo
    Serial.println(contA);
    digitalWrite(AMARI, digitalRead(AMARI) ^ 1);    //hace titilar el led amarillo
    digitalWrite(ROJO, LOW);                        //apaga led rojo
    digitalWrite(VERDE, LOW);                       //apaga led verde

    vTaskDelay(200 / portTICK_PERIOD_MS);           //establece velocidad a 200 ms
  }
}


void greenLedControllerTask(void *pvParameters) { //tarea 3
  pinMode(VERDE, OUTPUT);                     //establece pin rojo como salida
  while (1) {                                 //realiza un bucle infinito
    contV++;                                  //contador que sirve solo de guia no cumple ninguna funcion en el semaforo
    Serial.println(contV);
    digitalWrite(VERDE, HIGH);                //enciende led verde
    digitalWrite(ROJO, LOW);                  //apaga led rojo
    digitalWrite(AMARI, LOW);                 //apaga led amarillo

    vTaskDelay(1000 / portTICK_PERIOD_MS);    //establece velocidad a 1 seg
  }
}


void loop() {
  // put your main code here, to run repeatedly:
}
