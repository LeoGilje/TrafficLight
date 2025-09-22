#include "main.h"
#include <stdlib.h>
#include <time.h>

/* Function Prototypes */
static void MX_GPIO_Init(void);
int generateRandomNumber(int min, int max);

/* Helper Functions */
int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

uint8_t isPlayer1Pressed() {
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET; // USER button
}

uint8_t isPlayer2Pressed() {
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET;
}

void showWinner(uint8_t player) {
    if (player == 1) {
        HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET); // Player 1 wins
    } else if (player == 2) {
        HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET); // Player 2 wins
    }
}

/* Main Application */
int main(void)
{
    HAL_Init();
    MX_GPIO_Init();
    srand(HAL_GetTick()); // Seed RNG with system tick

    // Wait random time before reflex LED
    int waitTime = generateRandomNumber(1000, 20000); // 1–20s
    uint32_t elapsed = 0;
    uint8_t winner = 0;

    while (elapsed < waitTime) {
        if (isPlayer1Pressed()) {
            winner = 2; // Player 1 pressed too early → Player 2 wins
            break;
        }
        if (isPlayer2Pressed()) {
            winner = 1; // Player 2 pressed too early → Player 1 wins
            break;
        }
        HAL_Delay(10);
        elapsed += 10;
    }

    if (winner == 0) {
        // Reflex LED ON
        HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_SET);

        // Wait for first valid press
        while (winner == 0) {
            if (isPlayer1Pressed()) {
                winner = 1;
            } else if (isPlayer2Pressed()) {
                winner = 2;
            }
        }
    }

    showWinner(winner);

    while (1); // Game ends, wait for manual reset
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // LEDs: LD1 (PB0), LD2 (PB7), LD3 (PB14)
    GPIO_InitStruct.Pin = LD1_Pin | LD2_Pin | LD3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Buttons: BTN1 (PC0), BTN2 (PC1)
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}