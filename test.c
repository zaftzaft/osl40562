#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <bcm2835.h>

#define DIGIT_NUM 4

int digit[] = {21, 18, 17, 15};
int pos[] = {20, 19, 16, 14, 13, 12, 11, 10};

int map[11][8] = {
  {1,1,1,0,1,0,1,1},
  {0,0,1,0,1,0,0,0},
  {1,0,1,1,0,0,1,1},
  {1,0,1,1,1,0,1,0},
  {0,1,1,1,1,0,0,0},
  {1,1,0,1,1,0,1,0},
  {1,1,0,1,1,0,1,1},
  {1,1,1,0,1,0,0,0},
  {1,1,1,1,1,0,1,1},
  {1,1,1,1,1,0,1,0},
  {0,0,0,0,0,1,0,0}
};

int sw(int n) {
  for(int i = 0;i < DIGIT_NUM;i++) {
    bcm2835_gpio_write(digit[i], i == n ? LOW : HIGH);
  }
}


int show(int n, int dot) {
  int i;
  for(i = 0;i < 8;i++) {
    bcm2835_gpio_write(pos[i], map[n][i] ? HIGH : LOW);
  }

  if(dot) {
    bcm2835_gpio_write(pos[5], HIGH);
  }
}

int main() {
  int i, x;
  float val;
  int d[4] = {0, 0, 0, 0};
  struct timeval basetime;
  struct timeval nowtime;
  int counter = 0;
  int dot = 0;

  //getValue(d);

  if(!bcm2835_init()) {
    return 1;
  }

  for(i = 0;i < 4;i++) {
    bcm2835_gpio_fsel(digit[i], BCM2835_GPIO_FSEL_OUTP);
  }

  for(i = 0;i < 8;i++) {
    bcm2835_gpio_fsel(pos[i], BCM2835_GPIO_FSEL_OUTP);
  }

  gettimeofday(&basetime, NULL);

  while(1) {
    gettimeofday(&nowtime, NULL);
    if(nowtime.tv_sec - basetime.tv_sec > 1) {
      dot = 1 ^ dot;
      gettimeofday(&basetime, NULL);
    }

    d[3] = counter % 10;
    d[2] = (counter / 10) % 10;
    d[1] = (counter / 100) % 10;
    d[0] = (counter / 1000) % 10;

    sw(0);
    show(d[0], dot);
    bcm2835_delay(2);

    sw(1);
    show(d[1], dot);
    bcm2835_delay(2);

    sw(2);
    show(d[2], dot);
    bcm2835_delay(2);

    sw(3);
    show(d[3], dot);
    bcm2835_delay(2);

    if(++counter > 9999) {
      counter = 0;
    }
  }

  bcm2835_close();

  return 0;
}
