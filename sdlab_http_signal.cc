#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "sdlab.h"
#include "sdlab_http.h"
#include "sdlab_http_signal.h"

int phase_offset = 0;


double bridge_channel_a[BRIDGE_LEN];
double bridge_channel_b[BRIDGE_LEN];

double bridge_cross1_re[BRIDGE_LEN];
double bridge_cross1_im[BRIDGE_LEN];
double bridge_fft1_a[BRIDGE_LEN];
double bridge_fft1_b[BRIDGE_LEN];

double bridge_cross10_re[BRIDGE_LEN];
double bridge_cross10_im[BRIDGE_LEN];
double bridge_fft10_a[BRIDGE_LEN];
double bridge_fft10_b[BRIDGE_LEN];

double bridge_cross_total_re[BRIDGE_LEN];
double bridge_cross_total_im[BRIDGE_LEN];
double bridge_fft_total_a[BRIDGE_LEN];
double bridge_fft_total_b[BRIDGE_LEN];

int noprintf(const char *s, ...)
{
  return 0;
}

int sprintf_sinewave(char* buf, double amp, double phase, int num)
{
  int i;
  int len = 0;

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%f", amp * sin(phase));
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %f", amp * sin(((double) i)/100.0
                                                * 2.0 * M_PI + phase));
    if(len > AJAX_LENGTH){
      printf("too small AJAX_LENGTH %s %s %d\n",
             __FILE__, __FUNCTION__, __LINE__);
      exit(1);
    }
  }
  len += sprintf(buf + len, "]");
  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}


int sprintf_channel_a(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_udpwave");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = ((double)bridge_channel_a[i]) / 65535.0 * 100.0 + 50;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}


int sprintf_channel_b(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_channel_b");
    exit(0);
  }


  for(i = 0; i < num; i++){
    tmpbuf[i] = ((double)bridge_channel_b[i]) / 65535.0 * 100.0 + 50;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_cross1_re(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross1_re");
    exit(0);
  }

  {
    for(i = 0; i < num; i++){
      tmpbuf[i] = bridge_cross1_re[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
    }
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_cross1_im(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross1_im");
    exit(0);
  }

  {
    for(i = 0; i < num; i++){
      tmpbuf[i] = bridge_cross1_im[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
    }
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft1_a(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft1_a");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft1_a[i] + 25.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft1_b(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft1_b");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft1_b[i] + 25.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

//10sec

int sprintf_cross10_re(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross10_re");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = bridge_cross10_re[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_cross10_im(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross10_im");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = bridge_cross10_im[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft10_a(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft10_a");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft10_a[i] + 25;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft10_b(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft10_b");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft10_b[i] + 25.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}


//10sec

int sprintf_cross_total_re(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross_total_re");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = bridge_cross_total_re[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_cross_total_im(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_cross_total_im");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = bridge_cross_total_im[i] / 2.097e6 / 2.097e6 / 2 + 50.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft_total_a(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft_total_a");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft_total_a[i] + 25.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

int sprintf_fft_total_b(char* buf, double amp, int num)
{
  int i;
  int len = 0;
  double tmpbuf[BRIDGE_LEN];

  if(num > BRIDGE_LEN){
    fprintf(stderr,"error ");
    perror("sprintf_fft_total_b");
    exit(0);
  }

  for(i = 0; i < num; i++){
    tmpbuf[i] = 1.0e-16 * bridge_fft_total_b[i] + 25.0;
  }

  len += sprintf(buf, "[");
  len += sprintf(buf + len, "%5e", amp * tmpbuf[0]);
  for(i = 1; i < num; i++){
    len += sprintf(buf + len, ", %5e", amp * tmpbuf[i]);
  }
  len += sprintf(buf + len, "]");

  if(len > AJAX_LENGTH){
    printf("too small AJAX_LENGTH %s %s %d\n",
           __FILE__, __FUNCTION__, __LINE__);
    exit(1);
  }

  return len;
}

//send functions starts



void send_sinewave(int sock)
{
  char buf[12048];
  char sinbuf[AJAX_LENGTH];
  int len = 0;
  int slen = 0;
  int ret;
  double phase;

  phase_offset++;
  phase = ((double) phase_offset) * 2 * M_PI / 30.0;

  slen = sprintf_sinewave(sinbuf, 10.0, phase, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", slen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("sinewave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_sinewave");
  }

//  printf("%s", sinbuf);
  DEBUG("sinewave: sending sinebuf\n");
  ret = send(sock, sinbuf, slen, 0);
  DEBUG("sinewave: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("sine wave failed");
  }
  DEBUG("sinewave: finished\n");
}

void send_channel_a(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_channel_a(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_udpwave0");
  }

//  printf("%s", sinbuf);
  DEBUG("udpwave0: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("udpwave0: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_channel_b(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_channel_b(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_udpwave0");
  }

//  printf("%s", sinbuf);
  DEBUG("udpwave0: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("udpwave0: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

//1sec

void send_cross1_re(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross1_re(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross1_re");
  }

//  printf("%s", sinbuf);
  DEBUG("cross1_re: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross1_re: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_cross1_im(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross1_im(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross1_im");
  }

//  printf("%s", sinbuf);
  DEBUG("cross1_im: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross1_im: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_fft1_a(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft1_a(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft1_a");
  }

//  printf("%s", sinbuf);
  DEBUG("fft1_a: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft1_a: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_fft1_b(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft1_b(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft1_b");
  }

//  printf("%s", sinbuf);
  DEBUG("fft1_b: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft1_b: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

//10sec

void send_cross10_re(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross10_re(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("cross10_re: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross10_re");
  }

//  printf("%s", sinbuf);
  DEBUG("cross10_re: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross10_re: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("cross10_re: finished\n");
}

void send_cross10_im(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross10_im(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("cross10_im: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross10_im");
  }

//  printf("%s", sinbuf);
  DEBUG("cross10_im: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross10_im: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("cross10_im: finished\n");
}

void send_fft10_a(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft10_a(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft10_a");
  }

//  printf("%s", sinbuf);
  DEBUG("fft10_a: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft10_a: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_fft10_b(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft10_b(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft10_b");
  }

//  printf("%s", sinbuf);
  DEBUG("fft10_b: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft10_b: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

//total

void send_cross_total_re(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross_total_re(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("cross_total_re: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross_total_re");
  }

//  printf("%s", sinbuf);
  DEBUG("cross_total_re: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross_total_re: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("cross_total_re: finished\n");
}

void send_cross_total_im(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_cross_total_im(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("cross_total_im: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_cross_total_im");
  }

//  printf("%s", sinbuf);
  DEBUG("cross_total_im: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("cross_total_im: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("cross_total_im: finished\n");
}

void send_fft_total_a(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft_total_a(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft_total_a");
  }

//  printf("%s", sinbuf);
  DEBUG("fft_total_a: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft_total_a: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

void send_fft_total_b(int sock)
{
  char buf[12048];
  char udpbuf[AJAX_LENGTH];
  int len = 0;
  int ulen = 0;
  int ret;

  ulen = sprintf_fft_total_b(udpbuf, 1.0, 1000);

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Server: sdlab_http/1.0\r\n");
  len += sprintf(buf + len, "Accept-Ranges: bytes\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", ulen);
  len += sprintf(buf + len,
                 "Content-Type: application/json; charset=utf-8\r\n");
  len += sprintf(buf + len, "\r\n");

//  printf("%s", buf);
  DEBUG("udpwave: sending header\n");
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("send_fft_total_b");
  }

//  printf("%s", sinbuf);
  DEBUG("fft_total_b: sending udpbuf\n");
  ret = send(sock, udpbuf, ulen, 0);
  DEBUG("fft_total_b: sending sinebuf finished\n");
  if(ret < 0){
    shutdown(sock, 2);
    close(sock);
    SDLAB_ERROR("udp wave failed");
  }
  DEBUG("udpwave: finished\n");
}

