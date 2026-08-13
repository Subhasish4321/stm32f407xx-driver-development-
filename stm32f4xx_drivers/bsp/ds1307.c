#include "ds1307.h"
#include <cstring.h>
static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static void ds1307_write(uint8_t data, uint8_t reg_addr);
static uint8_t ds1307_read(uint8_t reg_addr);

I2C_Handle_t g_Ds1307I2cHandle;

/**
 * @brief  Initializes the DS1307 RTC module.
 * @retval Returns 0 if the clock is running, 1 if the clock is halted.
 */
uint8_t ds1307_init(void)
{
    // Initialization code for DS1307

    //1.Initialise the i2c pins
    ds1307_i2c_pin_config();

    //2.Initialise the i2c peripheral
    ds1307_i2c_config();

    //3. Enable the i2c peripheral
    I2C_PeriphealControl(DS1307_I2C, ENABLE);

    //4. Make clock halt(CH ) bit = 0 to start the clock.
        //Initially on power up the CH bit is set to 1 which means the clock is halted. So we need to clear this bit to start the clock.
    ds1307_write(0, DS1307_ADDR_SEC); 

    //5. read back clock halt bit to check if the clock is running or not.
    uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

    return ((clock_state >> 7 ) & 0x01) ; 
}

void ds1307_set_current_time(RTC_time_t *rtc_time)
{
    // Code to set current time
}

void ds1307_get_current_time(RTC_time_t *rtc_time)
{
    // Code to get current time
}

void ds1307_set_current_date(RTC_date_t *rtc_date)
{
    // Code to set current date
}

void ds1307_get_current_date(RTC_date_t *rtc_date)
{
    // Code to get current date
}

static void ds1307_i2c_pin_config(void)
{
    GPIO_Handle_t i2c_scl, i2c_sda;
    
    memset(&i2c_scl, 0, sizeof(i2c_scl));
    memset(&i2c_sda, 0, sizeof(i2c_sda));
    /**
     *I2C1_SCL -> PB6
     *I2C1_SDA -> PB7
     */
    i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
    i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
    i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
    i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FUN_MODE_4; //I2C1_SCL alternate function

    i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
    i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
    i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
    i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; //Open drain for I2C
    i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FUN_MODE_4; //I2C1_SDA alternate function

    GPIO_Init(&i2c_scl);
    GPIO_Init(&i2c_sda);
}

static void ds1307_i2c_config(void)
{
    g_Ds1307I2cHandle.pI2Cx = DS1307_I2C;
    g_Ds1307I2cHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;
    g_Ds1307I2cHandle.I2C_Config.I2C_DeviceAddress =  DS1307_I2C_ADDRESS;
    g_Ds1307I2cHandle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    g_Ds1307I2cHandle.Sr = I2C_ENABLE_SR; //Enable repeated start condition for I2C communication with DS1307
    I2C_Init(&g_Ds1307I2cHandle);

}
/**
 * Read data sheet to understand data write to slave(DS1307).
 * I2C frame : |START|SLA+W|ACK|REG_ADDR|ACK|DATA|ACK|STOP|
 * We directly write to the register of the DC1307 by sending the register address followed by the data to be written.
 */
static void ds1307_write(uint8_t data, uint8_t reg_addr)
{
    uint8_t tx[2];
    tx[0] = reg_addr;
    tx[1] = data;
    I2C_MasterSendData(&g_Ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0 );
}

static uint8_t ds1307_read(uint8_t reg_addr)
{
    uint8_t rec_data;
    I2C_MasterSendData(&g_Ds1307I2cHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0 );
    I2C_MasterReceiveData(&g_Ds1307I2cHandle, &rec_data, 1, DS1307_I2C_ADDRESS, 0);

    return rec_data;        
}