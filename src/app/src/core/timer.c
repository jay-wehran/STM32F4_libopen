/*  pwm on the timer peripheral */

#include "app/inc/core/timer.h"
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

// 168_000_000
// freq = system_freq / ((prescalar - 1) * (arr - 1))

#define PRESCALAR (168)
#define ARR_VALUE (1000)



void timer_setup(void){
    rcc_periph_clock_enable(RCC_TIM2);

    // High level timer configuration
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    // setup PWM mode
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);

    // Enable PWM output
    timer_enable_counter(TIM2);
    timer_enable_oc_output(TIM2, TIM_OC1);

    // setup frequency and resolution 
    timer_set_prescaler(TIM2, PRESCALAR - 1);
    timer_set_period(TIM2, ARR_VALUE - 1);


}

void timer_pwm_set_duty_cycle(float duty_cycle){
    // duty cycle = (ccr / arr) * 100
    // duty cycle / 100 = ccr / arr
    // crr = arr * (duty cycle / 100)

    const float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);
    timer_set_oc_value(TIM2, TIM_OC1, (uint32_t)raw_value);
}




// Since there is no direct timer hardwrae for PG13, here is sample program if it did 

/*
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "core/system.h"
#include "core/timer.h"

#define LED_PORT        (GPIOG)
#define LED_PIN         (GPIO13)


// set cf for gpio, enable gpio PG13
static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOG);
    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);   
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN);
}


int main(void) {

    system_setup();
    gpio_setup();
    timer_setup();


    uint64_t start_time = system_get_ticks();
    float duty_cycle = 0.0f;

    timer_pwm_set_duty_cycle(duty_cycle);

    while (1) {
        if(system_get_ticks() - start_time >= 10){
            duty_cycle += 1.0f;
            if(duty_cycle > 100.0f) {
                duty_cycle = 0.0f;
            }
            timer_pwm_set_duty_cycle(duty_cycle);

            start_time = system_get_ticks();
        }
        // now able to do useful work without nop
    }

    // Never return
    return 0;
}



*/