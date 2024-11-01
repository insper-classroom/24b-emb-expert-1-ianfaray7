#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define AUDIO_PIN 28      
#define ADC_PIN 26        
#define BUTTON_PIN 15     


void setup_adc_and_pwm() {
    
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0); 


    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);
    
    pwm_set_gpio_level(AUDIO_PIN, 0);
}

int main() {
    stdio_init_all();
    setup_adc_and_pwm();
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); 
    
    uint16_t adc_value;
    while (1) {
        if (!gpio_get(BUTTON_PIN)) {  
           
            adc_value = adc_read();
            
        
            pwm_set_gpio_level(AUDIO_PIN, adc_value);
        } else {
            pwm_set_gpio_level(AUDIO_PIN, 0);
        }
        sleep_us(125);
    }
}
