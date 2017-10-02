
#include <SPI.h>
#include <SD.h>
#include <arduino.h>
#define _SDLOGGER_H


class sdlogger{
	public:
	
	bool begin(uint8_t pin);//
	String parser(String data);// parser
	String makedir(String data);// directory maker data = directory 
	String makefile(String data);//file maker  data = directory + file name
	String reader(String data);//reader
	String logger(String data);//logger
	String timef();// time updater
	void datef();// date updater
	String settime(String date);//  date yy-mm-dd
	String setdate(String data);// time  hh:mm:ss
	
	
	
	String directory = "LOG";// default log directory
	String file = date;// file name
	String fileextention = ".txt";// file extention
	String path = directory +"/"+ file + fileextention;// file path
	
	
	String date = "2017-09-29";//date, makes log file name "yy/mm/dd"
	String time = "12:0:0";
	String time_stamp = date +" "+ time;//time stamp for log;
	unsigned long time_timer;// timef timer
	
	uint16_t year = 2017, month = 9, day = 28;
	uint8_t seconds, minutes = 5, hours = 21;
	
	int SDSS = 4; // sdcard chip select
	
	bool timeStamp = true;// wether or not to add time and date stamp
	bool debug = true;// debug mode prints details
	String date_delimiter = "-";
	String time_delimiter = ":";

	File myFile;
	
	private:
	//

};
