#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "main.h"
#include "light.h"
#include "temp.h"
#include "logger.h"
#include "sockets.h"
#include "my_signal.h"
#include "queue.h"
#include "timer.h"
#include <assert.h>

sensor_struct rcvd_struct;
/*First Initialization Function*/

int buffer_init()
{
   return 0;
}

void test_temp_cel()
{
   rcvd_struct = read_temp_data(0, 0); //converting 20 into kelvin which is expected to be 293
   printf("%f\n", rcvd_struct.sensor_data.temp_data.temp_c);
   //assert(rcvd_struct.sensor_data.temp_data.temp_c > 20);
   if ((-40 < rcvd_struct.sensor_data.temp_data.temp_c) && (rcvd_struct.sensor_data.temp_data.temp_c < 100))
   {
      printf("Temperature(Celsus)in Range, TEST PASSED!\n");
   }
   else
   {
      printf("Temperature(Celsus)in Range, TEST FAILED!\n");
   }
   
}
void test_temp_farh()
{
   rcvd_struct = read_temp_data(2, 0); //converting 20 into farheneit which is expected to be 68
   if ((-40 < rcvd_struct.sensor_data.temp_data.temp_c) && (rcvd_struct.sensor_data.temp_data.temp_c < 212))
   {
      printf("Temperature(Fahrenheit) in Range, TEST PASSED!\n");
   }
   else
   {
      printf("Temperature(Fahrenheit) in Range, TEST FAILED!\n");  
   }
   
}

void test_temp_kelv()
{
   rcvd_struct = read_temp_data(1, 0); //converting 20 into farheneit which s expected to be 68
   if (rcvd_struct.sensor_data.temp_data.temp_c > 273)
   {
      printf("Temperature(Kelvin) in Range, TEST PASSED\n");
   }
   else
   {
      printf("Temperature(Kelvin) in Range, TEST FAILED\n");
   }
}

void test_light_id()
{
   uint8_t id = light_id();
   if (id == 0x50)
   {
      printf("Light communicaion OK\n");
   }
   else
   {
      printf("LIGHT DATA: TEST FAILED\n");
   }
}

void test_light_data()
{
   float rcvd = read_light_data(0);
   if (rcvd > 10)
   {
      printf("Light data in range. TEST PASSED\n\n");
   }
   else
   {
      printf("LIGHT DATA: TEST FAILED\n");
   }
}
void test_queue_init()
{
   assert(0 == queue_init());
   printf("Queues Initialized, TEST PASSED!\n");
   queues_close();
   queues_unlink();
}

int main(void)
{
   i2c_open = open(I2C_BUS, O_RDWR);
   test_temp_cel();
   test_temp_farh();
   test_temp_kelv();
   test_light_id();
   test_light_data();
   test_queue_init();
}
