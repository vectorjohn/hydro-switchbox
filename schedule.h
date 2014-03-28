#ifdef DEBUG
  #define MILLIS 1L
  #define time_to_milli(h,m) ((unsigned long)(h*1000L))
  #define ONEDAY (24000L)
#else
  #define MILLIS 1000L
  #define time_to_milli(h,m) ((unsigned long)(h*60 + m)*60*MILLIS)
  #define ONEDAY (MILLIS * 60 * 60 * 24)
#endif

#define LIGHTPIN 0
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
