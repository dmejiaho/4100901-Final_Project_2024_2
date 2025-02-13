#include "keypad.h"
#include "main.h"

// Keypad layout: rows 0..3, columns 0..3.
const uint8_t keypad_map[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void keypad_init(void)
{
  // For an open-drain configuration, the idle state is HIGH.
  HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ROW_4_GPIO_Port, ROW_4_Pin, GPIO_PIN_SET);
}

static uint8_t keypad_scan_rows(GPIO_TypeDef* colPort, uint16_t colPin)
{
    uint8_t row = 0;
    
    // Scan Row 1: drive it low and check column.
    HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_RESET); // drive row1 low
    HAL_Delay(1); // small delay for stabilization
    if (HAL_GPIO_ReadPin(colPort, colPin) == GPIO_PIN_RESET) {
        row = 1;
    }
    HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_SET); // restore row1 high
    if (row) return row;
    
    // Scan Row 2:
    HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    if (HAL_GPIO_ReadPin(colPort, colPin) == GPIO_PIN_RESET) {
        row = 2;
    }
    HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_SET);
    if (row) return row;
    
    // Scan Row 3:
    HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    if (HAL_GPIO_ReadPin(colPort, colPin) == GPIO_PIN_RESET) {
        row = 3;
    }
    HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_SET);
    if (row) return row;
    
    // Scan Row 4:
    HAL_GPIO_WritePin(ROW_4_GPIO_Port, ROW_4_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    if (HAL_GPIO_ReadPin(colPort, colPin) == GPIO_PIN_RESET) {
        row = 4;
    }
    HAL_GPIO_WritePin(ROW_4_GPIO_Port, ROW_4_Pin, GPIO_PIN_SET);
    
    return row; // will be 0 if no row detected
}

uint8_t keypad_scan(uint16_t GPIO_Pin)
{
    uint8_t key = 0;
    uint8_t row = 0;
    
    switch (GPIO_Pin)
    {
      case COLUMN_1_Pin:
        row = keypad_scan_rows(COLUMN_1_GPIO_Port, COLUMN_1_Pin);
        if (row >= 1 && row <= 4)
            key = keypad_map[row - 1][0];
        break;
      case COLUMN_2_Pin:
        row = keypad_scan_rows(COLUMN_2_GPIO_Port, COLUMN_2_Pin);  
        if (row >= 1 && row <= 4)
            key = keypad_map[row - 1][1];
        break;
      case COLUMN_3_Pin:
        row = keypad_scan_rows(COLUMN_3_GPIO_Port, COLUMN_3_Pin);
        if (row >= 1 && row <= 4)
            key = keypad_map[row - 1][2];
        break;
      case COLUMN_4_Pin:
        row = keypad_scan_rows(COLUMN_4_GPIO_Port, COLUMN_4_Pin);
        if (row >= 1 && row <= 4)
            key = keypad_map[row - 1][3];
        break;
      case B1_Pin:
        break;
      default:
        break;
    }
    // Do not call keypad_init() here; each scan already restores row state.
    return key;
}
