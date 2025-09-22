#include "main.h"

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);

/* Helper Functions */
void setLights(GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_RED_Pin, red);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_YELLOW_Pin, yellow);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_GREEN_Pin, green);
}

uint8_t isButtonPressed() {
    return HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET;
}

/* Main Application */
int main(void)
{
    HAL_Init();
    MX_GPIO_Init();

    while (1)
    {
        // RED
        setLights(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
        uint32_t redTime = 20000; // 20s
        if (isButtonPressed()) {
            redTime += 10000; // extend RED by 10s
        }
        HAL_Delay(redTime);

        // RED + YELLOW
        setLights(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
        HAL_Delay(5000); // 5s

        // GREEN
        setLights(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
        uint32_t greenTime = 10000; // 10s
        uint32_t elapsed = 0;
        while (elapsed < greenTime) {
            if (isButtonPressed()) {
                // GREEN + YELLOW
                setLights(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
                HAL_Delay(5000);
                // Switch to RED
                setLights(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
                HAL_Delay(20000);
                break;
            }
            HAL_Delay(100);
            elapsed += 100;
        }
    }
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // LEDs: PB0, PB1, PB2
    GPIO_InitStruct.Pin = LED_RED_Pin | LED_YELLOW_Pin | LED_GREEN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    // Button: PC13
    GPIO_InitStruct.Pin = BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);
}