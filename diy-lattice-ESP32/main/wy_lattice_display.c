#include "wy_lattice_display.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_OUTPUT_IO_A 4
#define GPIO_OUTPUT_IO_B 16
#define GPIO_OUTPUT_IO_C 17
#define GPIO_OUTPUT_IO_D 5

#define GPIO_OUTPUT_IO_EN 18
#define GPIO_OUTPUT_IO_R1 19

#define GPIO_OUTPUT_IO_STB 25
#define GPIO_OUTPUT_IO_CLK 26

#define LED_ROW 16

static int loop_index = 0;
static int data_cursor = 0;
int ct;
static int current_row = 0;
static int row_number = 64;
unsigned char graphy_mem[1024] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_OUTPUT_IO_A) | (1ULL << GPIO_OUTPUT_IO_B) | (1ULL << GPIO_OUTPUT_IO_C) | (1ULL << GPIO_OUTPUT_IO_D) | (1ULL << GPIO_OUTPUT_IO_EN) | (1ULL << GPIO_OUTPUT_IO_R1) | (1ULL << GPIO_OUTPUT_IO_STB) | (1ULL << GPIO_OUTPUT_IO_CLK))
void init_gpio_group()
{
    printf("init GPIO\n");
    gpio_config_t io_conf;

    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void hc138sacn(unsigned char r)
{ //输出行线状态ABCD （A低,D高)
    gpio_set_level(GPIO_OUTPUT_IO_A, (r & 0x01));
    gpio_set_level(GPIO_OUTPUT_IO_B, (r & 0x02));
    gpio_set_level(GPIO_OUTPUT_IO_C, (r & 0x04));
    gpio_set_level(GPIO_OUTPUT_IO_D, (r & 0x08));
}

void hc595senddata(int index)
{ 
    for (int i = 0; i < 8; i++)
    {
        gpio_set_level(GPIO_OUTPUT_IO_CLK, 0);
        int byte = graphy_mem[index + i];

        if (byte < 1)
        {
            gpio_set_level(GPIO_OUTPUT_IO_R1, 0);
        }
        else
        {
            gpio_set_level(GPIO_OUTPUT_IO_R1, 1);
        }
        gpio_set_level(GPIO_OUTPUT_IO_CLK, 1);
    }
}

void display_led()
{
    for (int i = 0; i < row_number / 8; i++)
    {
        int index = current_row * row_number + i * 8 + data_cursor;

        hc595senddata(index);
    }
}

void timer_callback_100us()
{

    if (loop_index == 0)
    {

        display_led();
        gpio_set_level(GPIO_OUTPUT_IO_EN, 0);

        hc138sacn((unsigned char)current_row);

        gpio_set_level(GPIO_OUTPUT_IO_STB, 0); //595刷新
        gpio_set_level(GPIO_OUTPUT_IO_STB, 1);

        
    }
    else if (loop_index == 10)
    {

        gpio_set_level(GPIO_OUTPUT_IO_EN, 1);
        loop_index = 0;
        if (current_row == 15)
        {
            current_row = 0;
        }
        else
        {
            current_row++;
        }
        return;
    }


    loop_index++;
}