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

float getValue(int *d[4]) {
  FILE *fp;
  float val;
  char str[32];
  int x;

  if((fp = fopen("/tmp/node_exporter/tmp102.prom", "r")) == NULL) {
    fprintf(stderr, "read err\n");
    return 0.0;
  }

  fscanf(fp, "%s %f", str, &val);
  fclose(fp);


  val = val * 100;
  x = (int)val;
  d[3] = x % 10;
  d[2] = (x / 10) % 10;
  d[1] = (x / 100) % 10;
  d[0] = (x / 1000) % 10;

  printf("read\n");

  return val;
}

int main() {
  int i, x;
  float val;
  int d[4] = {0, 0, 0, 0};
  struct timeval basetime;
  struct timeval nowtime;

  getValue(d);

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
    if(nowtime.tv_sec - basetime.tv_sec > 10) {
      getValue(d);
      gettimeofday(&basetime, NULL);
    }

    sw(0);
    show(d[0], 0);
    bcm2835_delay(3);

    sw(1);
    show(d[1], 1);
    bcm2835_delay(3);

    sw(2);
    show(d[2], 0);
    bcm2835_delay(3);

    sw(3);
    show(d[3], 0);
    bcm2835_delay(3);
  }

  bcm2835_close();

  return 0;
}
