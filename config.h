#ifndef CONFIG_H_
#define CONFIG_H_

typedef struct{
  float ref_temp[3];
  float ref_hum[3];
  float p_gain_temp;
  float p_gain_hum;
} config_t;

#endif