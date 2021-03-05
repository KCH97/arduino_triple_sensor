#include <SoftwareSerial.h>

struct TFminiPlus
{
    int distance;
    int strength;
    boolean receiveComplete;
};

TFminiPlus TFminiPlusOne = {0, 0, false};
TFminiPlus TFminiPlusTwo = {0, 0, false};
TFminiPlus TFminiPlusThree = {0, 0, false};

//SoftwareSerial port(TX, RX);
SoftwareSerial portOne(2, 2);
SoftwareSerial portTwo(3, 3);
SoftwareSerial portThree(4, 4);

void getTFminiPlusData(SoftwareSerial *port, TFminiPlus *TFminiPlus)
{
    port->listen();
    while (TFminiPlus->receiveComplete == false)
    {
        static char i = 0;
        char j = 0;
        int checksum = 0;
        static int rx[9];
        if (port->available())
        {

            rx[i] = port->read();

            if (rx[0] != 0x59)
            {

                i = 0;
            }
            else if (i == 1 && rx[1] != 0x59)
            {

                i = 0;
            }
            else if (i == 8)
            {

                for (j = 0; j < 8; j++)
                {

                    checksum += rx[j];
                }

                if (rx[8] == (checksum % 256))
                {

                    TFminiPlus->distance = rx[2] + rx[3] * 256;

                    TFminiPlus->strength = rx[4] + rx[5] * 256;

                    TFminiPlus->receiveComplete = true;
                }

                i = 0;
            }
            else
            {

                i++;
            }
        }
    }
}

void setup()
{

    Serial.begin(115200);

    portOne.begin(115200);

    portTwo.begin(115200);

    portThree.begin(115200);
}

void loop()
{

    static unsigned long lastTime = millis();

    static unsigned int count = 0;

    static unsigned int frequency = 0;

    getTFminiPlusData(&portOne, &TFminiPlusOne);

    getTFminiPlusData(&portTwo, &TFminiPlusTwo);

    getTFminiPlusData(&portThree, &TFminiPlusThree);

    if (TFminiPlusOne.receiveComplete == true && TFminiPlusTwo.receiveComplete == true && TFminiPlusThree.receiveComplete == true)
    {

        //    if(TFminiPlusOne.receiveComplete == true && TFminiPlusTwo.receiveComplete == true) {

        ++count;

        if (millis() - lastTime > 999)
        {

            lastTime = millis();

            frequency = count;

            count = 0;
        }

        Serial.print(TFminiPlusOne.distance);

        Serial.print("cm\t");

        Serial.print("strength1: ");

        Serial.print(TFminiPlusOne.strength);

        Serial.print("\t");

        Serial.print(TFminiPlusTwo.distance);

        Serial.print("cm\t");

        Serial.print("strength2: ");

        Serial.print(TFminiPlusTwo.strength);

        Serial.print("\t");

        Serial.print(TFminiPlusThree.distance);

        Serial.print("cm\t");

        Serial.print("strength3: ");

        Serial.print(TFminiPlusThree.strength);

        Serial.print("\t");
        Serial.print(frequency); //40~70Hz, It maybe higher if we don't print other thing.
        Serial.println("Hz");
        TFminiPlusOne.receiveComplete = false;
        TFminiPlusTwo.receiveComplete = false;
        TFminiPlusThree.receiveComplete = false;
    }
}