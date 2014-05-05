#ifdef DEBUG
  #define MILLIS 1L
  #define time_to_milli(h,m) ((unsigned long)(h*1000L))
  #define ONESECOND 1000L
  #define ONEMINUTE (ONESECOND * 60)
  #define ONEHOUR (ONEMINUTE * 60)
  #define ONEDAY (24000L)
  #define TIME_SAVE_INTERVAL 5000L
#else
  #define MILLIS 1000L
  #define time_to_milli(h,m) ((unsigned long)(h*60 + m)*60*MILLIS)
  #define ONESECOND 1000L
  #define ONEMINUTE (ONESECOND * 60)
  #define ONEHOUR (ONEMINUTE * 60)
  #define ONEDAY (ONEHOUR * 24)
  #define TIME_SAVE_INTERVAL 60000L
#endif

#define LIGHTPIN 0
#define FANPIN 1
#define SCHED_SENTINEL {(unsigned long)-1, -1, -1}
#define is_sentinel(x) (x.time == (unsigned long)-1)
#define ONEWEEK (ONEDAY * 7)
#define safe_millis(x) (milli_offset + x)

typedef struct {
    unsigned long time;
    int pin;
    int value;
} Event;

int numEvents( Event sched[] ) {
  int n = 0;
  while ( !is_sentinel( sched[ n ] ) )
    n++;
  return n;
}

unsigned long dayOffsetFromString( char* ts ) {
    return 10L * ONEHOUR * (ts[0] - '0') + ONEHOUR * (ts[1] - '0')
        + 10L * ONEMINUTE * (ts[3] - '0') + ONEMINUTE * (ts[4] - '0')
        + 10L * ONESECOND * (ts[6] - '0') + ONESECOND * (ts[7] - '0');
}
