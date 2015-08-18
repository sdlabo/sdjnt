#ifndef SDLAB_H
#define SDLAB_H

#define BRIDGE_LEN 1024 * 100 //1 Mbyte
#define AJAX_LENGTH 1024 * 1024
//#define BRIDGE_LEN 2 * 1024 * 1024

#define TRUE 1
#define FALSE 0

//#define DEBUG printf
#define DEBUG noprintf

enum enum_content_type {
  TEXT_HTML,
  TEXT_PLAIN,
  TEXT_CSS,
  BINARY
};

typedef struct
{
  char cmd[64];
  char path[256];
  char referer[256];
  int content_length;
  enum_content_type content_type;
  int code;
  double version;
} info_type;

extern int noprintf(const char *s, ...);

extern double bridge_channel_a[BRIDGE_LEN];
extern double bridge_channel_b[BRIDGE_LEN];

extern double bridge_cross1_re[BRIDGE_LEN];
extern double bridge_cross1_im[BRIDGE_LEN];
extern double bridge_fft1_a[BRIDGE_LEN];
extern double bridge_fft1_b[BRIDGE_LEN];

extern double bridge_cross10_re[BRIDGE_LEN];
extern double bridge_cross10_im[BRIDGE_LEN];
extern double bridge_fft10_a[BRIDGE_LEN];
extern double bridge_fft10_b[BRIDGE_LEN];

extern double bridge_cross_total_re[BRIDGE_LEN];
extern double bridge_cross_total_im[BRIDGE_LEN];
extern double bridge_fft_total_a[BRIDGE_LEN];
extern double bridge_fft_total_b[BRIDGE_LEN];


extern int log_drop_count;
extern double log_calc_max;
extern double log_calc_min;
extern double log_calc_now;
extern double log_fft_max;
extern double log_fft_min;
extern double log_fft_now;

#endif
