
#include "SmartServoBus.hpp"
#include "robotka.h"
#include <Arduino.h>
using namespace lx16a;
static SmartServoBus servoBus;
// otestuje funkcnost ramene
bool done = false;
void arm_test() {

    servoBus.set(0, 90_deg, 200.f, 1.f);
    printf("servo moved to:%f\n", rkServosGetPosition(1));
    delay(1500);
}
// nastavi pozici ramene a klepet na chytani kostek
void arm_pickmode() {
    rkServosSetPosition(1, -90);
    printf("arm in pick mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, 30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}
// nastavi pozici ramene na jizdu a zavre klepeta
void arm_drivemode() {
    servoBus.set(0, -30_deg, 100.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
    rkServosSetPosition(1, -90);
    printf("arm in drive mode, angle:%f\n", rkServosGetPosition(1));
}
// nastavi pozici ramene na vylozeni kostky a otevre klepeta
void arm_deliverymode() {
    rkServosSetPosition(1, 90);
    printf("arm in delivery mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, -30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}
///////////////////////
float g_US = 1;
const int speed = 100;
const float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm
const float wheel_diameter = 70; //
byte state = 1;

//jizda rovne o zadanou vzdalenost
void forward(int mm) {
    rkMotorsDrive(mm * ticksToMm, mm * ticksToMm, speed);
}

//zatoceni na miste o zadany pocet stupnu
// + doprava - doleva
void turn(int degrees) {
    rkMotorsDrive(3.141 * wheel_diameter * degrees / 360 * ticksToMm, -3.141 * wheel_diameter * degrees / 360 * ticksToMm, speed);
}

void donedone() {
    done = true;
    printf("ahoj ");
}
//oblouk
// true=doprava, false=doleva
void curve(int radius, int degrees, bool right, int done) {
    int done1 = 0;
    int sR = radius / radius * 40;
    int sL = (radius + wheel_diameter) / radius * 40;
    printf("sR: %i, sL: %i\n", sR, sL);
    if (right) {
        rkMotorsDriveRightAsync((ticksToMm * radius * PI * degrees) / 180, sR);
        rkMotorsDriveLeftAsync((ticksToMm * (radius + wheel_diameter) * PI * degrees) / 180, sL);
        printf("stav: %i", done1);
        printf("done");
    } else {
        rkMotorsDriveLeftAsync((ticksToMm * radius * PI * degrees) / 180, sR);
        rkMotorsDriveRightAsync((ticksToMm * (radius + wheel_diameter) * PI * degrees) / 180, sL);
        printf("stav: %i", done1);
    }

    //done = false ;
}

//oblouk dozadu
void curve_back(int radius, bool right) {
    int sR = radius / radius * 40;
    int sL = (radius + wheel_diameter) / radius * 40;
    printf("sR: %i, sL: %i\n", sR, sL);
    if (right) {
        rkMotorsDriveRightAsync(-ticksToMm * radius * PI, sR);
        rkMotorsDriveLeftAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    } else {
        rkMotorsDriveLeftAsync(-ticksToMm * radius * PI, sR);
        rkMotorsDriveRightAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
}

// robot couva dokud nenarazi na zadni tlacitko
void back_button() {
    rkMotorsSetSpeed(-100, -100);
    while (true) {
        if (rkButtonIsPressed(BTN_RIGHT)) {
            break;
        }
    }
    delay(500);
    rkMotorsSetSpeed(0, 0);
}

// otocka robota ode zdi o 90 stupnu doleva
void turn_by_wall() {
    rkServosSetPosition(3, -90);
    delay(1000);
    rkServosSetPosition(3, 90);
    delay(1000);
    rkServosSetPosition(3, -90);
    delay(1000);
    rkServosSetPosition(3, 90);
    delay(1000);
    back_button();
}
//robot jede na cervene pole
void go_to_red() {
    //sevre klepeta
    arm_drivemode();
    back_button();
    forward(150);
    turn(-90);
    forward(-450); //doladit
    turn(90);
    back_button();
    arm_deliverymode();
    //klepeta pustit
}

void go_to_green() {
    //sevre klepeta
    arm_drivemode();
    back_button();
    arm_deliverymode();
    //klepeta pustit
}

void go_to_blue() {
    //sevre klepeta
    arm_drivemode();
    back_button();
    forward(150);
    turn(90);
    forward(-450); //doladit
    turn(-90);
    back_button();
    arm_deliverymode();
    //klepeta pustit
}
///////////////////////
void encodery() {
    Serial.printf("L: %f, R: %f", rkMotorsGetPositionLeft(), rkMotorsGetPositionRight());
}
void Skoback() {
    forward(1000);
    turn(90);
    forward(450);
    turn(90);
    forward(500);
    turn(-90);
    forward(400);
    turn(-90);
    forward(850);
}
void Sko() {
    forward(850);
    turn(90);
    forward(450);
    turn(90);
    forward(500);
    turn(-90);
    forward(500);
    turn(-90);
    forward(1000);
}
void update_sensors() {
    g_US = rkUltraMeasure(1);
    printf(" g_US: %f \n", g_US);
    // std::cout << " " << std::endl;
}

//////////////////
void setup() {
    rkConfig cfg;
    rkSetup(cfg);
    servoBus.begin(1, UART_NUM_1, GPIO_NUM_27);
    servoBus.setAutoStop(0, true);
    Serial.begin(115200); //zacne komunikace
    //curve(150, 180, true, 1);
    Serial.println("start");
}

void loop() {
    int number = Serial.parseInt();
    Serial.println("dd");
    //
    if (number == 420) {
        Sko();
        Serial.println("69");
        printf("0");
    }

    //zatoceni custom
    if ((number >= -360 && number <= 360) &&(number!= 0)){
        turn(number);
        Serial.println("69");
        Serial.println("1");
    }

    //custom jizda rovne
    if (number >= 1000 && number <= 50000) {
        forward(number / 100);
        Serial.println("69");
        Serial.println("2");
    }

    //otocka v poli
    if (number == 690) {
        forward(450);
        turn(-90);
        Serial.println("69");
        Serial.println("3");
        //todo opravit backbutton
    }

    //test oblouku
    //if (number == 1){
    //arm_drivemode();
    //back_button();
    //forward(500);
    //curve(150, 180, true);
    //forward(50);
    //curve(150, 150, false);
    //forward(1500);
    //Serial.println("69");
    //}
}