
#define MILLIS 1000L
#define time_to_milli(h,m) ((unsigned long)(h*60 + m)*60*MILLIS)
#define LIGHTPIN 0
#define SCHED_SENTINEL {(unsigned long)-1, -1, -1}
#define is_sentinel(x) (x.time == (unsigned long)-1)
#define ONEDAY (MILLIS * 60 * 60 * 24)
#define ONEWEEK (ONEDAY * 7)
#define safe_millis(x) (milli_offset + x)

typedef Event struct {
    unsigned long time;
    int pin;
    int value;
}

//this is what time it is when the chip turns on
unsigned long start_clock[2] = {12, 0};

int next_event_idx = -1;
unsigned long int next_event_millis = 0;
int day = 0;
unsigned long next_day_millis = ONEDAY;
unsigned long milli_offset = 0;

//DO keep these sorted.  Otherwise nothing good will happen.
Event schedule[] = {
    //{time_to_milli(6, 0), 0, LOW},
    {time_to_milli(18, 0), LIGHTPIN, HIGH},
    {time_to_milli(23, 0), LIGHTPIN, LOW},
    SCHED_SENTINEL
};

void setup() {                
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite( 0, HIGH );
    digitalWrite( 1, HIGH );
    digitalWrite( 2, HIGH );
    digitalWrite( 3, HIGH );

    unsigned int start_off = millis() + time_to_milli(start_clock[0], start_clock[1]);
    readyNextEvent();

    //skip events until the next one is in the future.
    while ( start_off > next_event_millis )
        readyNextEvent();
}

void loop() {
    unsigned now = safe_millis( millis() );
    if ( now > next_event_millis ) {
        digitalWrite( schedule[ next_event_idx ].pin, schedule[ next_event_idx ].value );
        readyNextEvent();
    }

    if ( now > next_day_millis ) {
        day++;
        next_day_millis += ONEDAY;
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
    next_event_millis = ONEDAY*event_day + schedule[ nxt_event_idx ].time;

    if ( prev > next_event_millis ) {
        //overflow
        milli_offset = next_event_millis + (((unsigned long)-1) - prev);
        next_event_millis += milli_offset;
    }
}
