#include "stm32f7xx_hal.h"

#define LED_RED_PIN     GPIO_PIN_0
#define LED_YELLOW_PIN  GPIO_PIN_1
#define LED_GREEN_PIN   GPIO_PIN_2
#define BUTTON_PIN      GPIO_PIN_3
#define LED_PORT        GPIOB
#define BUTTON_PORT     GPIOB

void SystemClock_Config(void);
void GPIO_Init(void);
void delay_seconds(uint32_t seconds);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        // RED
        HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_PORT, LED_YELLOW_PIN | LED_GREEN_PIN, GPIO_PIN_RESET);
        uint32_t red_time = 20;
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET) {
            red_time += 10;
        }
        delay_seconds(red_time);

        // RED + YELLOW
        HAL_GPIO_WritePin(LED_PORT, LED_YELLOW_PIN, GPIO_PIN_SET);
        delay_seconds(5);

        // GREEN
        HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN | LED_YELLOW_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
        delay_seconds(10);

        // Check for pedestrian button during GREEN
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET) {
            HAL_GPIO_WritePin(LED_PORT, LED_YELLOW_PIN, GPIO_PIN_SET);
            delay_seconds(5);
            HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN | LED_YELLOW_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        }
    }
}

void delay_seconds(uint32_t seconds) {
    HAL_Delay(seconds * 1000);
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // LEDs
    GPIO_InitStruct.Pin = LED_RED_PIN | LED_YELLOW_PIN | LED_GREEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Button
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}
