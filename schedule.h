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
  #define duration_to_event(h,m,duration,pin) {time_to_milli(h,m), pin, LOW}, {time_to_milli(h,m)+duration, pin, HIGH}
  #define ONESECOND 1000L
  #define ONEMINUTE (ONESECOND * 60)
  #define ONEHOUR (ONEMINUTE * 60)
  #define ONEDAY (ONEHOUR * 24)
  #define TIME_SAVE_INTERVAL 60000L
#endif

#define PLUG0 0
#define PLUG1 1
#define PLUG2 2
#define PLUG3 3
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

#define swapEvents(a, b, tmp) tmp = a; a = b; b = tmp;
#define eventBefore(a, b)  (a.time < b.time)

int _partitionSchedule(Event *sched, int lo, int hi) {

  int pivotIndex = lo;
  Event pivotValue = sched[pivotIndex];
  Event tmp;

  swapEvents(sched[pivotIndex], sched[hi], tmp);
  int storeIndex = lo;

  int i;
  for (i = lo; i < hi; i++) {
    if (eventBefore(sched[i], pivotValue)) {
      swapEvents(sched[i], sched[storeIndex], tmp);
      storeIndex++;
    }
  }

  swapEvents(sched[storeIndex], sched[hi], tmp);
  return storeIndex;
}

void _sortSchedule(Event *sched, int lo, int hi) {
  if (lo < hi) {
    int p = _partitionSchedule(sched, lo, hi);
    _sortSchedule(sched, lo, p - 1);
    _sortSchedule(sched, p + 1, hi);
  }
}

void sortSchedule(Event *sched) {
  int hi = 0;
  while (!is_sentinel(sched[hi]))
    hi++;

  _sortSchedule(sched, 0, hi-1);
}
