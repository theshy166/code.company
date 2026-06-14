#ifndef QUACAST_NETWORK_H
#define QUACAST_NETWORK_H
int quacast_start_ap(char *ssid, char *passwd, unsigned char channel, int blue_start);
int quacast_stop_ap(void);
int quacast_connect_ap(const char *ssid, const char *passwd, const char *key);
int quacast_disconnect_ap(void);
#endif