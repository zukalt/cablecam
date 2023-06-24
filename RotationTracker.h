#ifndef RotationTracker_h
#define RotationTracker_h


#define PIN_STATES_LENGTH 6

#define P000 0B00000000
#define P001 0B00000001
#define P011 0B00000011
#define P111 0B00000111
#define P110 0B00000110
#define P100 0B00000100

#define P010 0B00000010
#define P101 0B00000101


#define P1_MASK P001
#define P2_MASK P010
#define P3_MASK P100





class RotationTracker {

public:
  const int p1, p2, p3;
  volatile byte error = 0;

  volatile byte last = 0;
  volatile byte entry = 0;

  volatile int pos = 0;


public:

  RotationTracker(int p1, int p2, int p3)
    : p1(p1), p2(p2), p3(p3) {
  }

  int sensorError() {
    return this->error;
  }

  void attach() {
    // nothing
  }

  void reset() {
    this->last = 0;
    this->entry = 0;
    this->pos = 0;
    this->error = 0;
  }

  void onISREvent(unsigned long time) {
    char pins = 0;
    pins += !bitcheck(PINB, this->p1) ? P1_MASK : 0;
    pins += !bitcheck(PINB, this->p2) ? P2_MASK : 0;
    pins += !bitcheck(PINB, this->p3) ? P3_MASK : 0;
    if (this->last != pins) {
      this->onPinChange(pins);
      this->last = pins;
      // if (this->hasErrorState(pins)) {
      //   this->error = 1;
      // }
    }
  }

  int position() {
    return this->pos;
  }

protected:
  void onPinChange(char pins) {
    if (pins == P111) {
      return;
    }

    if (this->entry) {
      if ((pins == P100) && (this->entry == P011)) {
        this->pos++;
        this->entry = 0;
      } else if ((pins == P001) && (this->entry == P110)) {
        this->pos--;
        this->entry = 0;
      }
    } else {
      if (pins == P011 || pins == P110) {
        this->entry = pins;
      }
    }
  }

  bool hasErrorState(char pins) {
    return (
      (this->last == P000) && (pins & P010)
      || (this->last == P111) && (pins & P101));
  }
};

#endif RotationTracker_h
