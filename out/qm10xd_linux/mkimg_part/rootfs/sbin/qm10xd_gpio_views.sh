#!/bin/sh

get_pin_cfg() {
  local addr_hex="$1"

  case "$addr_hex" in
    0x10200008)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "EPHY_RXN"
      else
        echo ""
      fi
      ;;
    0x1020000c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "EPHY_RXP"
      else
        echo ""
      fi
      ;;
    0x10200010)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "EPHY_TXN"
      else
        echo ""
      fi
      ;;
    0x10200014)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "EPHY_TXP"
      else
        echo ""
      fi
      ;;
    0x10200018)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO0_7"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SENSOR_RSTN0"
      else
        echo ""
      fi
      ;;
    0x1020001c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_0"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "UART0_TXD"
      else
        echo ""
      fi
      ;;
    0x10200020)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_1"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_CD"
      else
        echo ""
      fi
      ;;
    0x10200024)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_2"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_D1"
          elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI0_CSN0"
      else
        echo ""
      fi
      ;;
    0x10200028)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_3"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI0_DI"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_D0"
          else
            echo ""
      fi
      ;;
    0x1020002c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI0_CLK"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_CLK"
          else
            echo ""
      fi
      ;;
    0x10200030)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI0_DO"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_CMD"
          else
            echo ""
      fi
      ;;
    0x10200034)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_D3"
      elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_6"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART1_TXD"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SCL"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_DAT"
      else
        echo ""
      fi
      ;;
    0x10200038)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SD1_D2"
      elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO1_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART1_RXD"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SDA"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_LINK"
      else
        echo ""
      fi
      ;;
    0x1020003c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART1_RXD"
          elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SDA"
      elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_LINK"
      elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_STBY"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_HOLD_IO3"
      else
        echo ""
      fi
      ;;
    0x10200040)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART1_TXD"
          elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SCL"
      elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_DAT"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_WP_IO2"
      else
        echo ""
      fi
      ;;
    0x10200044)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_3"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_MISO_IO1"
      else
        echo ""
      fi
      ;;
    0x10200048)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_0"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_CLK"
      else
        echo ""
      fi
      ;;
    0x1020004c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_MOSI_IO0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_2"
      else
        echo ""
      fi
      ;;
    0x10200050)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_CS1N"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_6"
      else
        echo ""
      fi
      ;;
    0x10200054)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "SFC_CS0N"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_1"
      else
        echo ""
      fi
      ;;
    0x10200058)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "TEST_MODE"
      else
        echo ""
      fi
      ;;
    0x1020005c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "JTAG_TCK"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_1"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT3"
      elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "CAN_TX"
      else
        echo ""
      fi
      ;;
    0x10200060)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "JTAG_TMS"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_2"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SDA"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT2"
      elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "CAN_RX"
      else
        echo ""
      fi
      ;;
    0x10200064)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_DE"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_3"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_TE"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "MDCK"
      elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2S_DI"
      else
        echo ""
      fi
      ;;
    0x10200068)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_HSYNC"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_CSN"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "EPHY_RSTN"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2S_DO"
      else
        echo ""
      fi
      ;;
    0x1020006c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_VSYNC"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_RS"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "MDIO"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2S_BCLK"
      else
        echo ""
      fi
      ;;
    0x10200070)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_CLK"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_6"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_WRN"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "EPHY_CLK"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2S_MCLK"
      else
        echo ""
      fi
      ;;
    0x10200074)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT0"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_TXD1"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2S_WS"
      else
        echo ""
      fi
      ;;
    0x10200078)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_0"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT1"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_TXD0"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART3_TXD"
          elif [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D1"
      else
        echo ""
      fi
      ;;
    0x1020007c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D2"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_1"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT2"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_TXEN"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART3_RXD"
      else
        echo ""
      fi
      ;;
    0x10200080)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D4"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_3"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_RXD1"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_DAT"
      else
        echo ""
      fi
      ;;
    0x10200084)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D3"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_2"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT3"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_CLK"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "AUX_CLK"
      else
        echo ""
      fi
      ;;
    0x10200088)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D5"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SDA"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_RXD0"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "EPHY_LED_LINK"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_STBY"
      else
        echo ""
      fi
      ;;
    0x1020008c)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D7"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_6"
          elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SDA"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART1_RXD"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_RX"
      else
        echo ""
      fi
      ;;
    0x10200090)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D6"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "RMII_CRSDV"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART1_TXD"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_TX"
      else
        echo ""
      fi
      ;;
    0x10200094)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D9"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_0"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SDA"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "CAN_RX"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_D2"
      else
        echo ""
      fi
      ;;
    0x10200098)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D8"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO4_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "CAN_TX"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_D3"
      else
        echo ""
      fi
      ;;
    0x1020009c)
          func_sel=$(devmem "$addr_hex" 32)
          if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D10"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_1"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI_SLV_CSN0"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI1_CSN0"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_D1"
      else
        echo ""
      fi
      ;;
    0x102000a0)
          func_sel=$(devmem "$addr_hex" 32)
          if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D11"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_2"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI_SLV_DI"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI1_DI"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_D0"
      else
        echo ""
      fi
      ;;
    0x102000a4)
          func_sel=$(devmem "$addr_hex" 32)
          if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D12"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_3"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI_SLV_DO"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI1_DO"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_CMD"
      else
        echo ""
      fi
      ;;
    0x102000a8)
          func_sel=$(devmem "$addr_hex" 32)
          if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D13"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "SPI_SLV_CLK"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI1_CLK"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_CLK"
      else
        echo ""
      fi
      ;;
    0x102000ac)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D14"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT0"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART2_RTSN"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SD0_CD"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_TX"
      else
        echo ""
      fi
      ;;
    0x102000b0)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D15"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_6"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "PWM_OUT1"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART2_CTSN"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "PWM_OUT2"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_RX"
      else
        echo ""
      fi
      ;;
    0x102000b4)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D16"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO2_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART2_TXD"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "PWM_OUT3"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "EPHY_LED_DAT"
      else
        echo ""
      fi
      ;;
    0x102000b8)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "LCD_D17"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO3_0"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C0_SDA"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART2_RXD"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "CAN_STBY"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "EPHY_LED_LINK"
      else
        echo ""
      fi
      ;;
    0x102000bc)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DP3"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_2"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D21"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SDA"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART2_CTSN"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "I2S_WS"
      else
        echo ""
      fi
      ;;
    0x102000c0)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DN3"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_1"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D20"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "I2C1_SCL"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART2_RTSN"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "I2S_BCLK"
      else
        echo ""
      fi
      ;;
    0x102000c4)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DP2"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_4"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D23"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT1"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART2_RXD"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_CD"
      else
        echo ""
      fi
      ;;
    0x102000c8)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DN2"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_3"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D22"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT0"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "UART2_TXD"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "I2S_MCLK"
      else
        echo ""
      fi
      ;;
    0x102000cc)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_CKP1"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_0"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D19"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "LCD_RDN"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2C0_SDA"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "I2S_DI"
      else
        echo ""
      fi
      ;;
    0x102000d0)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_CKN1"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO5_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "LCD_D18"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT2"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2C0_SCL"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "I2S_DO"
      else
        echo ""
      fi
      ;;
    0x102000d4)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_CKN0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART3_TXD"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT2"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2C0_SCL"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_D3"
      else
        echo ""
      fi
      ;;
    0x102000d8)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_CKP0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_6"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "UART3_RXD"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "PWM_OUT3"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "I2C0_SDA"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_D2"
      else
        echo ""
      fi
      ;;
    0x102000dc)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DN1"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO6_7"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2S_DO"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI_SLV_CSN0"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SPI1_CSN0"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_D1"
      else
        echo ""
      fi
      ;;
    0x102000e0)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DP1"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO7_0"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2S_DI"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI_SLV_DI"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SPI1_DI"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_D0"
      else
        echo ""
      fi
      ;;
    0x102000e4)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DN0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO7_1"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2S_BCLK"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI_SLV_DO"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SPI1_DO"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_CMD"
      else
        echo ""
      fi
      ;;
    0x102000e8)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "DSI_DP0"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO7_2"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2S_WS"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "SPI_SLV_CLK"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "SPI1_CLK"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "SD0_CLK"
      else
        echo ""
      fi
      ;;
    0x102000ec)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "XAIN0_SADC"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO0_5"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SCL"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART1_TXD"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "PWM_OUT0"
      else
        echo ""
      fi
      ;;
    0x102000f0)
          func_sel=$(devmem "$addr_hex" 32)
      if [[ $((func_sel & 0x7)) -eq 0 ]]; then
        echo "XAIN1_SADC"
          elif [[ $((func_sel & 0x7)) -eq 1 ]]; then
        echo "GPIO0_6"
      elif [[ $((func_sel & 0x7)) -eq 2 ]]; then
        echo "I2C1_SDA"
      elif [[ $((func_sel & 0x7)) -eq 3 ]]; then
        echo "UART1_RXD"
          elif [[ $((func_sel & 0x7)) -eq 4 ]]; then
        echo "PWM_OUT1"
          elif [[ $((func_sel & 0x7)) -eq 5 ]]; then
        echo "CAN_STBY"
      else
        echo ""
      fi
      ;;
    *) echo "";;
  esac
}

get_pin_gpio() {
  local addr_hex="$1"

  case "$addr_hex" in
    0x10200008)
        echo "0x10200008"
      ;;
    0x1020000c)
        echo "0x1020000c"
      ;;
    0x10200010)
        echo "0x10200010"
      ;;
    0x10200014)
        echo "0x10200014"
      ;;
    0x10200018)
        echo "0x10200018(GPIO0_7)"
      ;;
    0x1020001c)
        echo "0x1020001c(GPIO1_0)"
      ;;
    0x10200020)
                echo "0x10200020(GPIO1_1)"
      ;;
    0x10200024)
                echo "0x10200024(GPIO1_2)"
      ;;
    0x10200028)
                echo "0x10200028(GPIO1_3)"
      ;;
    0x1020002c)
                echo "0x1020002c(GPIO1_4)"
      ;;
    0x10200030)
                echo "0x10200030(GPIO1_5)"
      ;;
    0x10200034)
                echo "0x10200034(GPIO1_6)"
      ;;
    0x10200038)
                echo "0x10200038(GPIO1_7)"
      ;;
    0x1020003c)
                echo "0x1020003c(GPIO2_5)"
      ;;
    0x10200040)
                echo "0x10200040(GPIO2_4)"
      ;;
    0x10200044)
        echo "0x10200044(GPIO2_3)"
      ;;
    0x10200048)
        echo "0x10200048(GPIO2_0)"
      ;;
    0x1020004c)
        echo "0x1020004c(GPIO2_2)"
      ;;
    0x10200050)
        echo "0x10200050(GPIO2_6)"
      ;;
    0x10200054)
        echo "0x10200054(GPIO2_1)"
      ;;
    0x10200058)
        echo "0x10200058"
      ;;
    0x1020005c)
        echo "0x1020005c(GPIO3_1)"
      ;;
    0x10200060)
        echo "0x10200060(GPIO3_2)"
      ;;
    0x10200064)
        echo "0x10200064(GPIO3_3)"
      ;;
    0x10200068)
        echo "0x10200068(GPIO3_4)"
      ;;
    0x1020006c)
        echo "0x1020006c(GPIO3_5)"
      ;;
    0x10200070)
        echo "0x10200070(GPIO3_6)"
      ;;
    0x10200074)
        echo "0x10200074(GPIO3_7)"
      ;;
    0x10200078)
        echo "0x10200078(GPIO4_0)"
      ;;
    0x1020007c)
        echo "0x1020007c(GPIO4_1)"
      ;;
    0x10200080)
        echo "0x10200080(GPIO4_3)"
      ;;
    0x10200084)
        echo "0x10200084(GPIO4_2)"
      ;;
    0x10200088)
        echo "0x10200088(GPIO4_4)"
      ;;
    0x1020008c)
        echo "0x1020008c(GPIO4_6)"
      ;;
    0x10200090)
        echo "0x10200090(GPIO4_5)"
      ;;
    0x10200094)
        echo "0x10200094(GPIO5_0)"
      ;;
    0x10200098)
        echo "0x10200098(GPIO4_7)"
      ;;
    0x1020009c)
        echo "0x1020009c(GPIO5_1)"
      ;;
    0x102000a0)
        echo "0x102000a0(GPIO5_2)"
      ;;
    0x102000a4)
        echo "0x102000a4(GPIO5_3)"
      ;;
    0x102000a8)
        echo "0x102000a8(GPIO5_4)"
      ;;
    0x102000ac)
        echo "0x102000ac(GPIO5_5)"
      ;;
    0x102000b0)
        echo "0x102000b0(GPIO5_6)"
      ;;
    0x102000b4)
        echo "0x102000b4(GPIO2_7)"
      ;;
    0x102000b8)
        echo "0x102000b8(GPIO3_0)"
      ;;
    0x102000bc)
        echo "0x102000bc(GPIO6_2)"
      ;;
    0x102000c0)
        echo "0x102000c0(GPIO6_1)"
      ;;
    0x102000c4)
        echo "0x102000c4(GPIO6_4)"
      ;;
    0x102000c8)
        echo "0x102000c8(GPIO6_3)"
      ;;
    0x102000cc)
        echo "0x102000cc(GPIO6_0)"
      ;;
    0x102000d0)
        echo "0x102000d0(GPIO5_7)"
      ;;
    0x102000d4)
        echo "0x102000d4(GPIO6_5)"
      ;;
    0x102000d8)
        echo "0x102000d8(GPIO6_6)"
      ;;
    0x102000dc)
        echo "0x102000dc(GPIO6_7)"
      ;;
    0x102000e0)
        echo "0x102000e0(GPIO7_0)"
      ;;
    0x102000e4)
        echo "0x102000e4(GPIO7_1)"
      ;;
    0x102000e8)
        echo "0x102000e8(GPIO7_2)"
      ;;
    0x102000ec)
        echo "0x102000ec(GPIO0_5)"
      ;;
    0x102000f0)
        echo "0x102000f0(GPIO0_6)"
      ;;
    *) echo "";;
  esac
}

start_addr=0x10200008
end_addr=0x102000f0

offset=$(( (start_addr - start_addr) / 4 ))
end_offset=$(( (end_addr - start_addr) / 4 ))

while [[ $offset -le $end_offset ]]; do
  pin_addr=$((start_addr + offset * 4))
  addr_hex=$(printf "0x%x" $pin_addr)

  pin_name=$(get_pin_gpio "$addr_hex")
  pin_cfg=$(get_pin_cfg "$addr_hex")
    if [[ -n "$pin_cfg" ]]; then
      printf "$pin_name: $pin_cfg\n"
    fi

  offset=$((offset + 1))  # Important: Increment offset inside the loop
done

