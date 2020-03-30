#include <mbed.h>
#include <comunication.h>
	
COMM::COMM(type_comm_t comm){
    this->comm = comm;
    initGPRS();
}

bool COMM::send(float temperature, float humidity){
    bool aux = false;

    this->temperature = temperature;
    this->humidity = humidity;

    switch (this->comm)
    {
    case GPRS:
        aux = this->sendGPRS();    
        break;
    case WIFI:
        aux = this->sendWIFI();    
        break;
    
    default:
        break;
    }

    return aux;
}

bool COMM::initGPRS(){
    wait(1);
    do{
        strcpy(cmdbuff,"AT+CFUN=1\r\n");
        SendCMD();
        timeout=1;
        getreply();
        pc.printf("Diferente: %d\r\n", strlen(replybuff));
    }while(strlen(replybuff) != 2);

    do{    
        strcpy(cmdbuff, "AT+CIPMUX=0\r\n");
        SendCMD();
        timeout=5;
        getreply();    
        pc.printf("at+cipmux=0 : %s\r\n", replybuff);
        pc.printf("Diferente: %d\r\n", strlen(replybuff));

        if(strlen(replybuff) == 9)
            return false;
    }while(strlen(replybuff) != 6);
   
    do{ 
        strcpy(cmdbuff, "AT+CSTT=\"tim.br\",\"tim\",\"tim\"\r\n");
        SendCMD();
        timeout=5;
        getreply();
        wait(1);
        pc.printf("AT+CSTT=\"tim.br\",\"tim\",\"tim\" : %s\r\n", replybuff);
        pc.printf("Diferente: %d\r\n", strlen(replybuff));

        if(strlen(replybuff) == 9)
            return false;
    }while(strlen(replybuff) != 6);    

    wait(3);

    do{
        strcpy(cmdbuff, "AT+CIICR\r\n");
        SendCMD();
        timeout=15;
        getreply();
        wait(3);
        pc.printf("AT+CIICR : %s\r\n", replybuff);

        pc.printf("Diferente: %d\r\n", strlen(replybuff));

        if(strlen(replybuff) == 9)
            return false;
    } while(strlen(replybuff) != 1);
    
    do{
        strcpy(cmdbuff, "AT+CIFSR\r\n");
        SendCMD();
        timeout=15;
        getreply();
        wait(1);
        pc.printf("AT+CIFSR : %s\r\n", replybuff);
        pc.printf("### %d ###\n\r", strlen(replybuff));

        if(strlen(replybuff) == 9)
            return false;
    }while(strlen(replybuff) < 12);

    return true;
}

bool COMM::initWIFI(){
    timeout=1000;getcount=600;
    wait(1);
    strcpy(cmdbuff,"AT\r\n");
    SendCMD();
    timeout=1;
    getreply();
    wait(1);

    pc.printf("\nGet Connection Status\r\n");
    strcpy(cmdbuff, "AT+CWJAP?\r\n");
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(replybuff);
    
    wait(1);

    return true;
}

bool COMM::sendGPRS(){
    char var[2048];
    char cmd[2048];
    char aux[2048];
    char test[2048];
    pc.printf("\nStart TCP\r\n\n");    
        strcpy(test, "AT+CIPSTART=\"TCP\",\"things.ubidots.com\",80\r\n");
        esp.printf("%s", test);
        timeout=10;
        getreply();
        wait(3);    

    strcpy(var, "{\"temperature\":\"");
    sprintf(aux, "%d", (int) this->temperature);
    strcat(var, aux);
    sprintf(aux, "%d", (int) this->humidity);
    strcat(var, "\",\"moisture\":\"");
    strcat(var, aux);
    strcat(var, "\"}");
    int tamanho = strlen(var);
    sprintf(aux, "%d", tamanho);

    strcpy(cmd,"POST /api/v1.6/devices/TestTCCGPRS HTTP/1.1\r\n");
    strcat(cmd,"Host: things.ubidots.com\r\n");
    strcat(cmd,"User-Agent: ESP8266/1.0\r\n");
    strcat(cmd,"X-Auth-Token: BBFF-ILWiaIPmfKINNKhPorTRuFdw0CuL1b\r\n");
    strcat(cmd,"Connection: close\r\n");
    strcat(cmd,"Content-Type: application/json\r\n");
    strcat(cmd,"Content-Length: ");
    strcat(cmd, aux);
    strcat(cmd, "\r\n\r\n");
    strcat(cmd, var);
    strcat(cmd, "\r\n");

    sprintf(aux, "%d", strlen(cmd));
    pc.printf("%s\n\r", aux);

    strcpy(cmdbuff,"AT+CIPSEND=");
    strcat(cmdbuff, aux);
    strcat(cmdbuff, "\r\n");
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(replybuff);
    
    // send POST
    pc.printf("\r\n%s\r\n", cmd);
    strcpy(cmdbuff, cmd);
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(replybuff);

    wait(5);
    return true;
}

bool COMM::sendWIFI(){
    char test[255];
    pc.printf("\nStart TCP\r\n\n");
    strcpy(test, "AT+CIPSTART=\"TCP\",\"things.ubidots.com\",80\r\n");
    esp.printf("%s", test);
    timeout=10;
    getreply();
    pc.printf(replybuff);

    wait(1);
    char var[255];
    char cmd[500];
    char aux[100];
    strcpy(var, "{\"temperature\":\"");
    sprintf(aux, "%d", (int) this->temperature);
    strcat(var, aux);
    sprintf(aux, "%d", (int) this->humidity);
    strcat(var, "\",\"moisture\":\"");
    strcat(var, aux);
    strcat(var, "\"}");
    int tamanho = strlen(var);
    sprintf(aux, "%d", tamanho);

    strcpy(cmd,"POST /api/v1.6/devices/TestTCC HTTP/1.1\r\n");
    strcat(cmd,"Host: things.ubidots.com\r\n");
    strcat(cmd,"User-Agent: ESP8266/1.0\r\n");
    strcat(cmd,"X-Auth-Token: BBFF-ILWiaIPmfKINNKhPorTRuFdw0CuL1b\r\n");
    strcat(cmd,"Connection: close\r\n");
    strcat(cmd,"Content-Type: application/json\r\n");
    strcat(cmd,"Content-Length: ");
    strcat(cmd, aux);
    strcat(cmd, "\r\n\r\n");
    strcat(cmd, var);
    strcat(cmd, "\r\n");

    sprintf(aux, "%d", strlen(cmd));

    strcpy(cmdbuff,"AT+CIPSEND=");
    strcat(cmdbuff, aux);
    strcat(cmdbuff, "\r\n");
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(replybuff);
    
    wait(0.5);
    
    // send POST
    strcpy(cmdbuff, cmd);
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(replybuff);
    return true;
}

void COMM::getreply()
{    
    memset(replybuff, '\0', sizeof(replybuff));
    t1.reset(); t1.start();replycount=0;
    while(t1.read_ms()< timeout && replycount < getcount) {
        if(esp.readable()) {
            replybuff[replycount] = esp.getc();replycount++;
            }
        }
    t1.stop();      
}

void COMM::SendCMD()
{
    esp.printf("%s", cmdbuff);      
}  
