/*
 * comunication.h
 *
 *  Created on: 30 de set de 2019
 *      Author: robertcabral
 */

#ifndef COMM_H_
#define COMM_H_

static Serial pc(USBTX, USBRX);
static Serial esp(PTE0, PTE1);



typedef enum mode_direction_t
{
	GPRS,
    LORA,
    WIFI
} type_comm_t;

class COMM {
public:
	COMM(type_comm_t comm);
	bool send(float temperature, float humidity);
private:
    char replybuff[2048];
    char cmdbuff[2048];

    Timer t1;
    int  count,ended,timeout,getcount,replycount;
	type_comm_t comm;
    float temperature;
    float humidity;
    bool initGPRS();
    bool initLORA();
    bool initWIFI();
    bool sendGPRS();
    bool sendLORA();
    bool sendWIFI();
    void getreply();
    void SendCMD();
};

#endif /* COMM_H_ */
