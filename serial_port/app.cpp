/*
* @author       : huang li long <huanglilongwk@outlook.com>
* @time         : 2016/07/14
* @brief        : read and write data from and to serial port
*/

#include "app.h"

APP::APP(Serial_Port *serial_Port_)
{
    reading_status = 0;
	writing_status = 0; 

    read_tid  = 0;
	write_tid = 0;

    _thread_shoudle_exit = false;

    serial_port = serial_Port_;
}

APP::~APP(){}

// start up, create two thread for reading and writing 
void APP::start()
{
    int result;

    // check serial port's status
    if (serial_port->status != 1) // serial port status --> opened ? 
	{
		fprintf(stderr,"ERROR: serial port not open\n");
		throw 1;
	}

    // create a read thread 
    printf("Start a read thread! \n");
	result = pthread_create( &read_tid, NULL, &start_app_read_thread, this);
	if(result) throw result;

    // create a write thread 
    printf("Start a write thread! \n");
	result = pthread_create(&write_tid, NULL, &start_app_write_thread, this);
	if(result) throw result;

    return;
}

// serial port read and write function
void APP::read_message()
{
    bool success;   // receive success flag
    char data =0;
    success = serial_port->read_message(&data);
    /* do something with received data */
}

int APP::write_message(char *buf, int len)
{
    int re_len = serial_port->write_message(buf, len);

    return re_len;
}

void APP::stop()
{
    // closs thread
    printf("Close thread! \n");

    // signal exit
    _thread_shoudle_exit = true;

	// wait for exit
	pthread_join(read_tid,  NULL);
	pthread_join(write_tid, NULL);

}

void APP::start_read_thread()
{
    if(reading_status != 0)
	{
		fprintf(stderr,"read thread already running\n");
		return;
	}
	else
	{
		read_thread();
		return;
	}
}

void APP::start_write_thread(void)
{
    if ( not writing_status == false )
	{
		fprintf(stderr,"write thread already running\n");
		return;
	}

	else
	{
		write_thread();
		return;
	}
}

void APP::handle_quit(int sig)
{
	try 
    {
		stop();
	}
	catch (int error) 
    {
		fprintf(stderr,"Warning, could not stop app\n");
	}
}

void APP::read_thread()
{
	reading_status = true;
	while(!_thread_shoudle_exit)
	{
		read_message();
		usleep(10000); // Read batches at 100Hz
	}

	reading_status = false;

	return;
}

void APP::write_thread()
{
	// signal startup
    writing_status = true;

    // add you data here!
    char buf[20];
    for(char i=0; i<20; i++)
    {
        buf[i] = i;
    }
    while(!_thread_shoudle_exit)
	{
		usleep(250000);   // Stream at 4Hz
		write_message(buf, 20);
	}

    // signal end
	writing_status = false;

	return;
}

// ------------------------------------------------------------------------------
//  Pthread Starter Helper Functions
// ------------------------------------------------------------------------------

void *start_app_read_thread(void *args)
{
	// create an app object for reading
	APP *app_read = (APP *)args;

	// run the object's read thread
	app_read->start_read_thread();

	// done!
	return NULL;
}

void *start_app_write_thread(void *args)
{
	// create an app object for writing
	APP *app_write = (APP *)args;

	// run the object's read thread
	app_write->start_write_thread();

	// done!
	return NULL;
}