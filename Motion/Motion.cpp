
#include "SpotPWM.h"
#include "SpotScreen.h"

// servo angels in us: 1000=-45deg, 1500=0deg, 2000=+45deg

volatile int adc[16] = { 0 };
volatile int currentAdc = 8;

int gCurrentServo = 0;

class SpServo {
public:
    SpServo(uint8_t ix) : pwmIx{ix} {}
    void UpdatePWM() {
        // send a command only if something changed
        bool mustUpdate = false;
        if (isOn!=wasOn) {
            wasOn = isOn;
            mustUpdate = true;
        }
        if (isOn) {
            if (isAt!=wasAt) {
                wasAt = isAt;
                mustUpdate = true;
            }
            if (mustUpdate) {
                setServoUSec(pwmIx, isAt);
                Serial.print("Servo set: ");
                Serial.print(pwmIx);
                Serial.print(", ");
                Serial.println(isAt);
            }
        } else {
            if (mustUpdate) {
                pwm.setPWM(pwmIx, 0, 0);
                Serial.print("Servo off: ");
                Serial.println(pwmIx);
            }
        }
    }
    void SetPower(bool on) { isOn = on; }
    void SetPosition(int pos) { isAt = pos; }
    int GetPosition() { return isAt; }
    bool isOn = false;
    bool wasOn = true;
    int isAt = 1500;
    int wasAt = 1499;
    uint8_t pwmIx = 255;
};

SpServo gServoList[] {
    // teh numbers correspond to the PWM output channel in hardware
    SpServo( 3), SpServo( 2), SpServo( 1),     // front right leg
    SpServo( 7), SpServo( 6), SpServo( 5),     // front left leg
    SpServo(11), SpServo(10), SpServo( 9),     // hind right leg
    SpServo(15), SpServo(14), SpServo(13),     // hind left leg
};
int gNServoList = sizeof(gServoList) / sizeof(gServoList[0]);


void updateServos()
{
    for (int i=0; i<gNServoList; i++)
        gServoList[i].UpdatePWM();
}


void setup(void)
{
    digitalWrite(22, 1); // 0 enables PWM, 1 disables it
    pinMode(22, OUTPUT);

#if 0
    tone(45, 1396, 100);
    delay(120);
    tone(45, 1568, 100);
    delay(120);
    tone(45, 1760, 100);
    delay(120);
    tone(45, 1975, 100);
#else
    tone(45, 2000, 10);
    delay(60);
    tone(45, 2000, 10);
#endif

    analogReference(INTERNAL2V56);
    adc[10] = analogRead(A10);
    //ADMUX = bit (REFS0) | (adcPin & 0x07);

    Serial.begin(9600);
    Serial1.begin(115200);
    Serial1.print("\nboot\n");
    ComposeTrimScreen();
    setupScreen();

    setupPWM();
    delay(10);
    // pwm.setPWM(0, 4096, 0); // Port all on

    updateServos();
    pwm.setPWM(0, 0, 2048); // just some constant output that we can read back in through A12/INT23 to verify that the PWM module is still running
    digitalWrite(22, 0); // 0 enables PWM, 1 disables it

    pinMode(LED_BUILTIN, OUTPUT);

#if 0
    // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    //OCR1A = 31250;            // compare match register 16MHz/256/2Hz
    OCR1A = 15625;            // compare match register 16MHz/256/4Hz
    //OCR1A = 1250;             // compare match register 16MHz/256/50Hz
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
    interrupts();             // enable all interrupts
#endif
}

#if 0
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
    // 50Hz interrupt (emergency poer cut, etc.)
    //digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);   // toggle LED pin

    // ADMUX = bit (REFS0) | (adcPin & 0x07);  // channel 8 = 0b100000, etc.

    currentAdc = 8;
    bitSet(ADCSRB, MUX5); // A8 to 15
    ADMUX = (ADMUX & 0b11100000) - 8 + currentAdc;
    bitSet(ADCSRA, ADEN); // ENable ADC
    bitSet(ADCSRA, ADIE); // Interrupt Enable ADC
    bitSet(ADCSRA, ADSC); // Start Conversion ADC

    //adc10 = analogRead(A10);
}


ISR (ADC_vect)
{
    byte low, high;

    // read the current value
    low = ADCL;
    high = ADCH;
    adc[currentAdc] = (high<<8)|low;

    if (currentAdc < 11) {
        // now kick off the next AD conversion
        currentAdc++;
        ADMUX = (ADMUX & 0b11100000) - 8 + currentAdc;
        //bitSet(ADCSRA, ADEN); // ENable ADC
        //bitSet(ADCSRA, ADIE); // Interrupt Enable ADC
        bitSet(ADCSRA, ADSC); // Start Conversion ADC
    } else {
        digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);   // toggle LED pin
    }
}  // end of ADC_vect
#endif


char serBuf[64];
int serBufN = 0;
int serBufMax = sizeof(serBuf)-1;
unsigned long analogWriteTimer = 0;

int sPos = 1500;

void loop(void)
{
#if 1
    while (Serial1.available()) {
        int s = Serial1.read();
        //Serial.write(s);
        if (s=='\n') {
            serBuf[serBufN] = 0;
            //Serial.println(serBuf);
            if (serBuf[0]=='s') { // slider
                int m = serBuf[1]-'a';
                int v = atoi(serBuf+2);
                gServoList[m].SetPosition(v);
            } else if (serBuf[0]=='C') { // Set the current motor
                int m = serBuf[1]-'a';
                gCurrentServo = m;
                Serial1.print('s');
                Serial1.print((char)('a'+m));
                Serial1.print(gServoList[m].GetPosition());
                Serial1.print('\n');
            } else if (serBuf[0]=='M') { // Motor Power (Mxy; x = aIndex, y=0 or 1)
                int m = serBuf[1]-'a';
                bool on = (serBuf[2]=='1');
                gServoList[m].SetPower(on);
            } else if (strcmp(serBuf, "beep")==0) { // activate nothing
                tone(45, 2000, 50);
                Serial.println("->beep");
            } else if (strcmp(serBuf, "a0")==0) { // activate nothing
                Serial1.println("*T(off)*");
                Serial.println("->a0");
            } else if (strcmp(serBuf, "afra")==0) { // activate front right ankle
                Serial1.println("*TL F Ankle*");
                Serial.println("->afra");
            }
            serBufN = 0;
        } else if (serBufN<serBufMax) {
            serBuf[serBufN++] = s;
        } else {
            // cmd buffer overflow
        }
    }
    auto now = millis();
    if (now>analogWriteTimer) {
        analogWriteTimer = now + 20;
        updateServos();

        /*
         for (int i=8; i<12; i++) {
         Serial.print(adc[i]);
         Serial.print(" ");
         }
         Serial.println("");
         */
    }
    //delay(1);
#endif
#if 0
    int d;
    for (d=0; d<200; d+=3) {
        setServoUSec( 0, 1500-2*d);
        setServoUSec( 1, 1500+2*d);
        setServoUSec( 2, 1500-2*d);
        setServoUSec( 3, 1500+2*d);
        setServoUSec( 4, 1500+d);
        setServoUSec( 5, 1500-d);
        setServoUSec( 6, 1500+d);
        setServoUSec( 7, 1500-d);
        delay(20);
    }
    for (d=200; d>0; d-=3) {
        setServoUSec( 0, 1500-2*d);
        setServoUSec( 1, 1500+2*d);
        setServoUSec( 2, 1500-2*d);
        setServoUSec( 3, 1500+2*d);
        setServoUSec( 4, 1500+d);
        setServoUSec( 5, 1500-d);
        setServoUSec( 6, 1500+d);
        setServoUSec( 7, 1500-d);
        delay(20);
    }
    delay(1000);
#endif
#if 0
    for (int i=0; i<12; i++) {
        setServoUSec( i, 1600);
        delay(500);
        setServoUSec( i, 1500);
        delay(500);
    }
#endif
} 


void swrite(const char *txt)
{
//    int n = strlen(txt)+2;
//    delay(n*8);
    Serial1.println(txt);
}

//
// http://keuwl.com/apps/bluetoothelectronics/userguide/remote_code.html
//
void ComposeTrimScreen()
{
    swrite("*.kwl");
    swrite("clear_panel();");
    swrite("set_grid_size(16, 8);");
    swrite("set_panel_notes(\"Robowerk Spot\", \"Trim\", , );");

    /*
     // -- go back to the main panel
     Serial1.write("add_button(14, 0, 4, \"back;\", );\n");
     // -- reload this panel
     Serial1.write("add_button(15, 0, 7, \"reload;\", );\n");
     */
    // -- motor power for all motors
    swrite("add_switch(14, 6, 4, \"pwr1;\", \"pwr0;\", 0, 0);");

    /*
     // -- motors leg front right
     Serial1.write("add_button(0, 5,  7, \"fr0a;\", );\n");
     Serial1.write("add_button(0, 6, 12, \"fr1a;\", );\n");
     Serial1.write("add_button(0, 7, 12, \"fr2a;\", );\n");
     delay(100);
     */
    swrite("add_slider(3,1,7,1000,2000,1500,\"s\",\";\",0);");

    swrite("run();");
    swrite("*");
}

