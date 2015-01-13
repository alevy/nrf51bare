#include <stdint.h>

#define GPIO_BASE 0x50000000
#define GPIO_CFG 0x700
#define GPIO_SET 0x504
#define RED_LED 21
#define GREEN_LED 22
#define BLUE_LED 23

extern uint32_t __stack_top;
extern uint32_t __etext;
extern uint32_t __bss_start;
extern uint32_t __bss_end;
extern uint32_t __data_start;
extern uint32_t __data_end;

volatile uint32_t* led_pin_cfg =
  (uint32_t*)(GPIO_BASE + GPIO_CFG);

inline void gpio_set_output(int pin) {
  led_pin_cfg[pin] = 3;
}

volatile uint32_t* led_pin_set =
  (uint32_t*)(GPIO_BASE + GPIO_SET);

inline void gpio_toggle(int pin) {
  *led_pin_set ^= 1 << pin;
}

void init() {
  {
    // Relocate data to RAM
    uint32_t *src = &__etext;
    uint32_t *dest = &__data_start;

    while (dest != &__data_end) {
      *dest++ = *src++;
    }
  }

  {
    // Zero-out uninitialized data
    uint32_t *dest = &__bss_start;
    while (dest != &__bss_end) {
      *dest = 0;
    }
  }

  gpio_set_output(RED_LED);
  gpio_toggle(RED_LED);
  gpio_set_output(GREEN_LED);
  gpio_toggle(GREEN_LED);
  gpio_set_output(BLUE_LED);

  while(1) {
    int i;
    for (i = 0; i < 1000000; ++i) {
      asm volatile("nop");
    }
    gpio_toggle(BLUE_LED);
    gpio_toggle(RED_LED);

    for (i = 0; i < 1000000; ++i) {
      asm volatile("nop");
    }
    gpio_toggle(RED_LED);
    gpio_toggle(GREEN_LED);

    for (i = 0; i < 1000000; ++i) {
      asm volatile("nop");
    }
    gpio_toggle(GREEN_LED);
    gpio_toggle(BLUE_LED);
  }
}

void dummy_handler() {
  while(1)
    ;
}

__attribute__ ((section(".vectors"))) void* interrupt_handlers[] = {

  &__stack_top,
  init,
  dummy_handler,
  dummy_handler,
  0,                           /* Reserved */
  0,                           /* Reserved */
  0,                           /* Reserved */
  0,                           /* Reserved */
  0,                           /* Reserved */
  0,                           /* Reserved */
  0,                           /* Reserved */
  dummy_handler,
  0,                           /* Reserved */
  0,                           /* Reserved */
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  0,		 /*Reserved */
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  dummy_handler,
  0,		 /*Reserved */
  0,		 /*Reserved */
  0,		 /*Reserved */
  0,		 /*Reserved */
  0,		 /*Reserved */
  0		 /*Reserved */
};

