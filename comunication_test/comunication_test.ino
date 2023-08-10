//program na ozkouseni seriove komunikace mezi raspberry pi a esp 32 
void setup(){
Serial.begin(115200);//zacne komunikace 
while(!Serial){}//pocka nez je seriova komunikace pripravena mozna to ani nemusi byt  
}

void loop(){
Serial.println("message");
delay(1000); 
}