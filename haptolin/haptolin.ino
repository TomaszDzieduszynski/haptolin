// haptolin is a native-midi arduino-based microcontroller compatible "plug'n'play" with most of the synthesizers
// use a genuine arduino nano or a compatible board
// haptolin code by bzqp
// check the full project on Youtube, Arduino project hub and Thingiverse!
// 2019-2020

void setup() {
Serial.begin(31250);
//Serial.begin(9600);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(2, INPUT_PULLUP);
pinMode(3, INPUT_PULLUP);

attachInterrupt(0, enkoder2, CHANGE);
attachInterrupt(1, enkoder3, CHANGE);
}
int enk = 0;
int pot1;
int pot2;
int transpozycja = 0;

double dbl;
double inter;
int nuta;
int nutastara;
int bend;
int bendstary;
int dynamika;
int dynamikastara;
int aftertouch;
int aftertouchstary;

void loop() {

if(enk==2){
  delay(150);
  analogWrite(11, 0);
  analogWrite(9, 0);
  enk = 0;
  attachInterrupt(0, enkoder2, CHANGE);
  attachInterrupt(1, enkoder3, CHANGE);
}



nutastara=nuta;
dynamikastara=dynamika;
bendstary=bend;
aftertouchstary=aftertouch;

pot1=map(analogRead(A7),0,1023,1023,0);
pot2=map(analogRead(A6),0,1023,1023,0);

if(pot2<=64){
  dynamika=0;
  aftertouch=0;
}
else if ((pot2>=65)&&(pot2<=500)) {
 dynamika=double((-cos(double((double(map(pot2,65,500,0,436))/double(436)*double(PI))))/double(2)+double(0.5))*double(127));
 aftertouch=0;
}
else {
  dynamika=127;
  aftertouch=map(pot2,501,750,0,127);
}

bend=64+(pot1%32);
dbl=(double)pot1/(double)32;
inter=floor(dbl);
nuta=(35+inter+transpozycja);

if(!(aftertouch==aftertouchstary)){
  //176 Control Change
   Serial.write(176);
   Serial.write(15);
   Serial.write(aftertouch);
   //delay(100);
}

if(!(dynamika==dynamikastara)){
  //176 Control Change
   Serial.write(176);
   Serial.write(14);
   Serial.write(dynamika);
   //delay(100);
}



if(!(nuta==nutastara))
{
   //144 Note On
   Serial.write(144);  //144+channel
   Serial.write(nuta);  //pitch
   Serial.write(127);
   //delay(100);


   //128 Note Off
   Serial.write(128);  //128+channel
   Serial.write(nutastara);  //pitch
   Serial.write(127); //velocity
//delay(100);
}


if(!(bend==bendstary)){
  //224 Pitch Bend
   Serial.write(224);  //224+channel
   Serial.write(0);  //LSB
   Serial.write(int(bend));  //MSB
   //delay(100);
}


delay(30);

//LED RGB
double moc = double(pot2)/1023;
analogWrite(9, (pot1/4*0.65)*double(moc));            //RED
analogWrite(10, ((255-pot1/4)*0.8+51)*double(moc));   //GREEN
analogWrite(11, 0);                                   //BLUE



}

void enkoder2(){
if (!digitalRead(2)==digitalRead(3)){
  detachInterrupt(digitalPinToInterrupt(3));
  detachInterrupt(digitalPinToInterrupt(2));
  enk = 2;
  transpozycja++;
  analogWrite(11, 255);
}

}
void enkoder3(){

if (!digitalRead(2)==digitalRead(3)){
  detachInterrupt(digitalPinToInterrupt(2));
  detachInterrupt(digitalPinToInterrupt(3));
  enk = 2;
  transpozycja--;
  analogWrite(9, 255);
}


}
