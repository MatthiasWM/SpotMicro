
#include "SpotPWM.h"
#include "SpotScreen.h"
#include "TMonitor.h"
#include "TServo.h"

// servo angels in us: 1000=-45deg, 1500=0deg, 2000=+45deg

volatile int adc[16] = { 0 };
volatile int currentAdc = 8;

TMonitor Monitor;


void updateServos()
{
    for (int i=0; i<NServo; i++)
        Servo[i].UpdatePWM();
}


void setup(void)
{
    digitalWrite(22, 1); // 0 enables PWM, 1 disables it
    pinMode(22, OUTPUT);

    tone(45, 2000, 10);
    delay(60);
    tone(45, 2000, 10);

    analogReference(INTERNAL2V56);
    adc[10] = analogRead(A10);
    //ADMUX = bit (REFS0) | (adcPin & 0x07);

    Serial.begin(9600);
    Monitor.Setup();
    ComposeTrimScreen();
    setupScreen();

    TServo::Setup();

    updateServos();
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
    while (Serial1.available())
        Monitor.OnDataIn();
    auto now = millis();
    if (now>analogWriteTimer) {
        analogWriteTimer = now + 20;
        updateServos();
    }
}

