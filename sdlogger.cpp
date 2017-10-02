#include <sdlogger.h>

String sdlogger::parser(String data){/// parser
 static String parse_keywords[9] = { "mkdir", "name", "log", "read", "set date", "set time", "open", "close"};
 String returns;// data string to be passsed
int p, parsed = -1;

 for ( int i = 0; i < 9; i++){
  uint8_t p = data.indexOf(parse_keywords[i]);
  if(p > -1 && p == 0){ 
    parsed = i;
    data = data.substring(data.indexOf(parse_keywords[i]) + parse_keywords[parsed].length() + 1);
    break;
  }
 }

 

 switch(parsed){
  default:
  //returns = " no valid keyword found ";
  break;
  case 0:
  returns = makedir(data);
  break;
  case 1:
  returns = path;
  break;
  case 2:
  returns = logger(data);
  break;
  case 3:
  returns = reader(data);
  break;
  case 4:
  returns = setdate(data);
  break;
  case 5:
  returns = settime(data);
  break;
 }

  return returns;
}




bool sdlogger::begin(uint8_t pin){ // begin sdcard and create log path
	SDSS = pin;
	bool status = true;
	if(!SD.begin(SDSS)){
		Serial.println("No SD card available");
		status = false;
		}else{/// make log file
		makedir("/" +directory);
		makefile(path);
		if(debug){Serial.println("SD Ini done");}
		
		}
	return status;
}

String sdlogger::makedir(String data){ // make directory data = directory

if( !SD.exists(data)){// make directory if not available
  if( SD.mkdir(data) ){data =  " Created directory: "+data;}
}else{ data = "directory exists"; }

  return data;
}

String sdlogger::makefile( String data){// make file data = path;

   if( !SD.exists(data)){// make directory if not available
  myFile = SD.open(data, FILE_WRITE);
  myFile.close();
  data = "Created file: "+data;
 }else{ data = "file exists "; }

 return data;
}

String sdlogger::logger(String data){
	if( timeStamp ){// add time stamp if desired
		data = data + timef();
	}
     myFile = SD.open(path, FILE_WRITE);
	 myFile.println(data);
	 myFile.close();

	 return "wrote to: "+path;
}

String sdlogger::reader(String data){// read file on path "data"

myFile = SD.open(path, FILE_READ);
if (myFile) {
    Serial.println(data);
    while (myFile.available()) {
     Serial.write(myFile.read());
    }
}
myFile.close();

return "done reading file path: "+path;
}


String sdlogger::settime(String data){// set time
	String p;
	
	p = data.substring(0, data.indexOf(time_delimiter));
	hours = p.toInt();
	
	data = data.substring(data.indexOf(time_delimiter) + 1);
	p = data.substring(0, data.indexOf(time_delimiter));
	minutes = p.toInt();
	
	data = data.substring(data.indexOf(time_delimiter) + 1);
	p = data.substring(0, data.indexOf(time_delimiter));
	seconds = p.toInt();
	return "time set: "+ String(hours)+":"+String(minutes)+":"+String(seconds);
}



String sdlogger::timef(){// time updater

  while( micros() - time_timer > 1000000){
	  time_timer = micros() - ( micros() - time_timer - 1000000);
	
    if( seconds < 59 ){
      seconds++;
    }else{
      seconds = 0;

      if( minutes < 59){
        minutes++;
      }else{
        minutes = 0;
        if( hours < 24){
          hours++;
        }else{
          hours = 0;
		  datef();
		  
        }
        
      }

    
    
	 }

	 time = String(hours)+":"+String(minutes)+":"+String(seconds);
	 time_stamp = " "+date +" "+ time;
	 if(debug){Serial.println(time_stamp);}
	 }
	 return time_stamp;
}


String sdlogger::setdate(String data){// set date format " yy-mm-dd
	String p;

	p = data.substring(0, data.indexOf(date_delimiter));
	year = p.toInt();
	
	data = data.substring(data.indexOf(date_delimiter) + 1);
	p = data.substring(0, data.indexOf(date_delimiter));
	month = p.toInt();

	
	data = data.substring(data.indexOf(date_delimiter) + 1);
	p = data.substring(0, data.indexOf(date_delimiter));
	day = p.toInt();
	
	date = String(year)+date_delimiter+String(month)+date_delimiter+day;
	return "date set: "+ String(year)+"-"+String(month)+"-"+String(day);
}

void sdlogger::datef(){
	
	static uint8_t max_days[13] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if( day < max_days[month] ){
		day++;
	}else{
		
		day = 1;
		
		if( month < 12 ){
			month++;
		}else{
			month = 1;
			year++;
		}
		
		
	}
	date = String(year)+date_delimiter+String(month)+date_delimiter+String(day);
		
}

	