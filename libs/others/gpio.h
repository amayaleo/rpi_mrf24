#pragma once

#define IN_INTERRUPT    23
#define OUT_INTERRUPT   12    //GPIO LED
#define READING_STEPS   2     //10 originalmente

#define SYSFS_GPIO_PATH             "/sys/class/gpio"
#define SYSFS_GPIO_EXPORT_FN        "/export"
#define SYSFS_GPIO_UNEXPORT_FN      "/unexport"
#define SYSFS_GPIO_VALUE            "/value"
#define SYSFS_GPIO_DIRECTION        "/direction"
#define SYSFS_GPIO_EDGE             "/edge"

#define DIR_IN                      "in"
#define DIR_OUT                     "out"

#define VALUE_HIGH                  "1"
#define VALUE_LOW                   "0"

#define EDGE_RISING                 "rising"
#define EDGE_FALLING                "falling"

#define POLL_TIMEOUT        10*1000


namespace GPIO{
struct Gpio{
   // Gpio();
      Gpio(bool& st)
      :state{st}
      {}
    ~Gpio();
            int file_open_and_write_value(const char *, const char *);
            int gpio_export(int );
            int gpio_unexport(int );
            int gpio_set_direction(int , const char *);
            int gpio_set_value(int , const char *);
            int gpio_set_edge(int , const char *);
            int gpio_get_fd_to_value(int );
            const bool app() ;
        private :
            static inline int static_file_open_and_write_value{0};
            const bool state{false};
    };
}