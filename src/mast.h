/*

  mast.h

  MAST: Multicast Audio Streaming Toolkit
  Copyright (C) 2019  Nicholas Humfrey
  License: MIT

*/

#include "config.h"

#ifndef MAST_H
#define MAST_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/in.h>

#ifndef TRUE
#define TRUE  (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif



typedef struct
{
    int fd;
    int is_multicast;

    struct sockaddr_storage saddr;

    union {
        struct ipv6_mreq imr6;
        struct ip_mreq imr;
    };

} mast_socket_t;


int mast_socket_open(mast_socket_t* sock, const char* address, const char* port, const char *ifname);
int mast_socket_recv(mast_socket_t* sock, void* data, unsigned int len);
void mast_socket_close(mast_socket_t* sock);



// ------- SAP packet handling ---------

#define MAST_SAP_ADDRESS_LOCAL  "239.255.255.255"
#define MAST_SAP_ADDRESS_ORG    "239.195.255.255"
#define MAST_SAP_ADDRESS_GLOBAL "224.2.127.254"
#define MAST_SAP_PORT           "9875"

enum
{
    MAST_SAP_MESSAGE_ANNOUNCE = 0,
    MAST_SAP_MESSAGE_DELETE = 1
};

typedef struct
{

   // FIXME: add originating source address
    uint8_t message_type;
    uint16_t message_id_hash;

    char sdp[2048];
} mast_sap_t;


int mast_sap_parse(const uint8_t* data, size_t data_len, mast_sap_t* sap);


// ------- SDP handling ---------

#define MAST_SDP_MIME_TYPE      "application/sdp"

typedef struct
{
    char address[256];
    char port[6];

    char session_id[256];
    char session_origin[256];
    char session_name[256];
    char information[256];

    int payload_type;
    int sample_size;
    int sample_rate;
    int channel_count;
    int packet_duration;

} mast_sdp_t;


int mast_sdp_parse(const uint8_t* data, size_t data_len, mast_sdp_t* sdp);


// ------- Utilities ---------

void setup_signal_hander();
extern int running;
extern int exit_code;
extern int verbose;
extern int quiet;

typedef enum {
    mast_LOG_DEBUG,
    mast_LOG_INFO,
    mast_LOG_WARN,
    mast_LOG_ERROR
} mast_log_level;


void mast_log(mast_log_level level, const char *fmt, ...);

// Only display debug if verbose
#define mast_debug( ... ) \
		mast_log(mast_LOG_DEBUG, __VA_ARGS__ )

// Don't show info when quiet
#define mast_info( ... ) \
		mast_log(mast_LOG_INFO, __VA_ARGS__ )

#define mast_warn( ... ) \
		mast_log(mast_LOG_WARN, __VA_ARGS__ )

// All errors are fatal
#define mast_error( ... ) \
		mast_log(mast_LOG_ERROR, __VA_ARGS__ )


#endif
