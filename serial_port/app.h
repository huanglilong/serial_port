/*
* @author       : huang li long <huanglilongwk@outlook.com>
* @time         : 2016/07/14
* @brief        : read and write data from and to serial port
*/
#ifndef __APP_H_
#define __APP_H_

#include "serial_port.h"
#include <signal.h>

void* start_app_read_thread(void *args);
void* start_app_write_thread(void *args);

class APP
{

public:

    APP();
    APP(Serial_Port *serial_port);
    ~APP();

    void read_message(void);
    int  write_message(char *buf, int len);

    void start();
    void stop();

    void start_read_thread(void);
    void start_write_thread(void);

    void handle_quit(int sig);

    // mutex signal
    char reading_status;
	char writing_status;

    // thread status 
    bool _thread_shoudle_exit;

private:

    Serial_Port *serial_port;
    
    pthread_t read_tid;
    pthread_t write_tid;

    void read_thread(void);
    void write_thread(void);

};

#endif

