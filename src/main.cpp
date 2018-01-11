#include <Arduino.h>
#include <EEPROM.h>

// Number of milliseconds before button press is written to memory
#define MIN_BUTTON_PRESS 10

#define BUTTON_PIN 12

#define LIGHT_LED true
#define LED_PIN 3

#define BUZZER_PIN 6

/* Memory structure:
 * EEPROM[0], byte: number of total entries
 * EEPROM[2..n], sizeof(ButtonPress): given entries
 */


unsigned long prev_millis; // Time since down_press
bool was_pressed; // Was it pressed at all?

unsigned short data_length; // Current length of data
unsigned short curr_pos; // Current position in EEPROM

typedef struct {
    unsigned long time_started; // Starting time of button press
    unsigned long millis_count; // Length of button press
} ButtonPress;

void setup() {
    Serial.begin(9600);

    // Read in the current position of the data
    EEPROM.get(0, data_length);
    curr_pos = (unsigned short)(sizeof(data_length) + data_length*sizeof(ButtonPress));

    Serial.println("begin_serial"); // Send the initial signal

    // Initialize the button
    was_pressed = false;
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

// Clears the current memory on the EEPROM
void clearmem() {
    for(long p=0; p<EEPROM.length(); p++) {
        EEPROM.write(p, 0);
    }

    data_length = 0;
    curr_pos = sizeof(data_length);

    Serial.println("done");
}

void lightLED(bool on) {
    if(!LIGHT_LED) {
        digitalWrite(LED_PIN, 0);
     }
     digitalWrite(LED_PIN, on);
}
void soundBuzzer(bool on) {
    digitalWrite(BUZZER_PIN, on);
}

// Prints the memory contents to be parsed by the application
void printmem() {
    // Get amount of data, print it to console
    Serial.println(data_length);
    Serial.flush();

    unsigned short new_pos = sizeof(data_length);

    // For each data point, print it directly to console
    for(; new_pos < curr_pos; new_pos += sizeof(ButtonPress)) {
        ButtonPress curr_button;
        EEPROM.get(new_pos, curr_button);
        Serial.println(curr_button.time_started);
        Serial.println(curr_button.millis_count);
        Serial.flush();
    }
}

void savepress(unsigned long curr_millis) {
    unsigned long time_down = curr_millis - prev_millis;
    if (time_down < MIN_BUTTON_PRESS) {
        return;
    }

    ButtonPress button_press = {
        .time_started = prev_millis, 
        .millis_count = time_down
    };
    
    EEPROM.put(curr_pos, button_press);
    curr_pos += sizeof(ButtonPress);
    data_length += 1;
    EEPROM.put(0, data_length);
}


void loop() {
    if(Serial.available()) {
        char command = Serial.read();
        if(command == 'c') {
            clearmem();
        }
        else if(command == 'r') {
            printmem();
        }
    }
    
    bool curr_press = digitalRead(BUTTON_PIN);
    unsigned long curr_time = millis();

    lightLED(!curr_press);
    soundBuzzer(!curr_press);

    if (!curr_press) {
        if (!was_pressed) {
            was_pressed = true;
            prev_millis = curr_time;
        }
    } else {
        if (was_pressed) {
            was_pressed = false;
            savepress(curr_time);
        }
    }

}
