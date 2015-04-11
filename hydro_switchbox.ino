//#define DEBUG

#include "schedule.h"
#include "timestamp.h"

//this is what time it is when the chip turns on
unsigned long start_time = time_to_milli( 20, 27 );

int next_event_idx = -1;
unsigned long next_event_millis = 0;
int day = 0;
unsigned long next_day_millis = ONEDAY;
unsigned long milli_offset = 0;

//DO keep these sorted.  Otherwise nothing good will happen.
Event schedule[] = {
    {time_to_milli(6, 0), LIGHTPIN, LOW},

    {time_to_milli(6, 10), PUMPPIN, LOW},
    {time_to_milli(6, 40), PUMPPIN, HIGH},


    {time_to_milli(10, 0), FANPIN, LOW},
    {time_to_milli(14, 0), FANPIN, HIGH},

    {time_to_milli(14, 10), PUMPPIN, LOW},
    {time_to_milli(14, 40), PUMPPIN, HIGH},

    {time_to_milli(16, 0), FANPIN, LOW},
    {time_to_milli(19, 0), FANPIN, HIGH},

    {time_to_milli(19, 10), PUMPPIN, LOW},
    {time_to_milli(19, 40), PUMPPIN, HIGH},

    //{time_to_milli(18, 0), LIGHTPIN, HIGH},
    {time_to_milli(23, 0), LIGHTPIN, HIGH},


    //TEST SCHEDULE
    //{time_to_milli(0, 0), LIGHTPIN, LOW},
    //{time_to_milli(3, 0), LIGHTPIN, HIGH},
    //{time_to_milli(4, 0), LIGHTPIN, LOW},
    //{time_to_milli(5, 0), LIGHTPIN, HIGH},
    //{time_to_milli(10, 0), LIGHTPIN, LOW},
    //{time_to_milli(15, 0), LIGHTPIN, HIGH},

    SCHED_SENTINEL
};

void setup() {
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    //pinMode(3, OUTPUT);
    digitalWrite( 0, HIGH );
    digitalWrite( 1, HIGH );
    digitalWrite( 2, HIGH );
    //digitalWrite( 3, HIGH );

    start_time = dayOffsetFromString( __TIME__ );

    //unsigned long start_off = millis() + start_time;
    milli_offset = start_time;
    readyNextEvent();

    //skip events until the next one is in the future.
    while ( milli_offset > next_event_millis )
        readyNextEvent();

    //start the event that should be currently running
    startCurrentEvents( next_event_idx );
}

void loop() {
    unsigned long now = safe_millis( millis() );

    if ( now > next_day_millis ) {
        day++;
        next_day_millis += ONEDAY;
    }

    if ( now > next_event_millis ) {
        digitalWrite( schedule[ next_event_idx ].pin, schedule[ next_event_idx ].value );
        readyNextEvent();
    }

    if ( 0 == ( now % TIME_SAVE_INTERVAL ) )
    {
        updateTimestamp( now );
    }
}

//run the events for each pin that should be running if the next event is idx
void startCurrentEvents( int idx ) {
    int pins[4] = {HIGH, HIGH, HIGH, HIGH};

    int i, started = 0;
    for ( i = idx; i != idx || !started; i++ )
    {
        started = true;
        if ( is_sentinel( schedule[ i ] ) )
        {
            i = -1;
            continue;
        }

        pins[ schedule[i].pin ] = schedule[i].value;
    }

    for ( i = 0; i < 4; i++ )
    {
        if ( pins[i] > -1 )
            digitalWrite( i, pins[i] );
    }
}

void readyNextEvent() {
    next_event_idx++;
    int event_day = day;
    if ( is_sentinel( schedule[ next_event_idx ] ) ) {
        next_event_idx = 0;
        event_day++;
    }

    unsigned long prev = next_event_millis;
    next_event_millis = ONEDAY*event_day + schedule[ next_event_idx ].time;

    //blinkN( 1, next_event_idx + 1 );
    if ( prev > next_event_millis ) {
        //overflow
        //milli_offset = next_event_millis + (((unsigned long)-1) - prev);
        //next_event_millis += milli_offset;
        //blinkN( 2, 10 );
    }
}

void blinkN( int pin, int n ) {
    while ( n > 0 ) {
        digitalWrite( pin, LOW );
        delay( 100 );
        digitalWrite( pin, HIGH );
        delay( 300 );
        n--;
    }
}

