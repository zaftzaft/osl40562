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

int main() {

  int i, n;

  if(!bcm2835_init()) {
    return 1;
  }

  for(i = 0;i < 4;i++) {
    bcm2835_gpio_fsel(digit[i], BCM2835_GPIO_FSEL_OUTP);
  }

  for(i = 0;i < 8;i++) {
    bcm2835_gpio_fsel(pos[i], BCM2835_GPIO_FSEL_OUTP);
  }

  for(i = 0;i < 8;i++) {
    bcm2835_gpio_write(pos[i], LOW);
  }

  bcm2835_close();

  return 0;
}
