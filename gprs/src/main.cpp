#include <mbed.h>
#include <time.h>
#include <stdlib.h>
#include <DHT.h>
#include <comunication.h>

int main()
{       
        
    srand(time(NULL));
    pc.baud(115200);  
    esp.baud(115200);   
    //pc.printf("\r\nInicializando!!!");
    //wait(4);
    //GPRSconfig();
    float var = 1.0;
    COMM comm(GPRS);

    while(1){
        //float abc = 1.0;
        DHT sensor(PTB0, DHT22);
        do{
            sensor.readData();
            wait(1);
            printf("%d \r\n", (int) sensor.ReadTemperature(CELCIUS));
            printf("%d \r\n", (int) sensor.ReadHumidity());
        } while((int) sensor.ReadTemperature(CELCIUS) == 0 || (int) sensor.ReadTemperature(CELCIUS) > 40);
        wait(2);    
        
        pc.printf("Send to Ubidots:\r\n");
        comm.send(sensor.ReadTemperature(CELCIUS), sensor.ReadHumidity());
    }
    
}