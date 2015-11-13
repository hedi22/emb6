/*
 * emb6 is licensed under the 3-clause BSD license. This license gives everyone
 * the right to use and distribute the code, either in binary or source code
 * format, as long as the copyright license is retained in the source code.
 *
 * The emb6 is derived from the Contiki OS platform with the explicit approval
 * from Adam Dunkels. However, emb6 is made independent from the OS through the
 * removal of protothreads. In addition, APIs are made more flexible to gain
 * more adaptivity during run-time.
 *
 * The license text is:
 *
 * Copyright (c) 2015,
 * Hochschule Offenburg, University of Applied Sciences
 * Laboratory Embedded Systems and Communications Electronics.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*============================================================================*/
/**
 * 	 \addtogroup emb6
 * 	 @{
 *   \addtogroup stack_API Stack API
 *   @{
*/
/*! \file   emb6.h

    \author Peter Lehmann peter.lehmann@hs-offenburg.de
            Phuong Nguyen minh.nguyen@hs-offenburg.de

    \brief  emb6 API

	\version 0.0.2
*/

#ifndef EMB6_H_
#define EMB6_H_

/*==============================================================================
                      	  	  INCLUDES
==============================================================================*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/*
********************************************************************************
*                               VERSION DECLARATION
********************************************************************************
*/
#define EMB6_VERSION        000002UL          /* Version format Vx.yy.zz */


/*=============================================================================
                                BASIC CONSTANTS
==============================================================================*/
#undef  FALSE
#undef  TRUE
#define FALSE                               0 /* do not change                */
#define TRUE                                1 /* do not change                */
typedef void                                (*pfn_intCallb_t)(void *);

/* use only Exact-width integer types, linked with TMR_OVRFLOW_VAL 			  */
typedef uint32_t                            clock_time_t;
/* linked with clock_time_t, maximum value for clock_time_t variables	      */
#define TMR_OVRFLOW_VAL                     0xffffffff


/*=============================================================================
                                CONFIGURATIONS
==============================================================================*/
#include "emb6_conf.h"



/*==============================================================================
                                     MACROS
 =============================================================================*/
/** defines for the modulation type for RF transceiver */
#define MODULATION_QPSK100          0
#define MODULATION_BPSK20           1


/*==============================================================================
                        RPL Configuration
 =============================================================================*/

/*! \struct rpl_configuration
	\brief for dynamic RPL parameter configuration
*/
typedef struct rpl_configuration
{
    /* The DIO interval (n) represents 2^n ms. default value = 8 */
    uint8_t DIOintmin;
    /* Maximum amount of timer doublings. default value = 12 */
    uint8_t DIOintdoub;
    /* This value decides which DAG instance we should participate in by def.
     * default value = 0x1e (30) */
    uint8_t defInst;
    /* Initial metric attributed to a link when the ETX is unknown.
     * default value = 2 */
    uint8_t linkMetric;
    /* Default route lifetime unit. This is the granularity of time used in RPL
     * lifetime values, in seconds.
     * default value = 0xffff */
    uint16_t defRouteTimeUnit;
    /* Default route lifetime as a multiple of the lifetime unit.
     * default value = 0xff */
    uint8_t defRouteTime;
}s_rpl_conf_t;

/*! RPL configuration struct, do not change */
extern s_rpl_conf_t     rpl_config;

/*==============================================================================
                         MAC & PHY Parameter Configuration
 =============================================================================*/

/*! \struct mac_phy_configuration
*   \brief for initial mac_phy parameter configuration,
*   if changed during runtime RF-interface must be re-initialized
*/
typedef struct mac_phy_configuration
{
    /** MAC address, default value: { 0x00,0x50,0xc2,0xff,0xfe,0xa8,0xdd,0xdd}*/
    uint8_t mac_address[8];
    /** PAN ID, default value: 0xABCD */
    uint16_t pan_id;
    /** initial tx power, default value: 11 dBm */
    int8_t init_power;
    /** initial rx sensitivity, default value: -100 dBm */
    int8_t init_sensitivity;
    /** rf modulation type, default value BPSK20 */
    uint8_t modulation;
}s_mac_phy_conf_t;

/*! MAC configuration struct, do not change */
extern s_mac_phy_conf_t     mac_phy_config;

/*==============================================================================
                                     ENUMS
==============================================================================*/
/*! \enum e_radio_tx_status_t
    \brief Return code of an interface driver
*/
typedef enum {
    RADIO_TX_OK = 1,
    RADIO_TX_COLLISION = 2,
    RADIO_TX_NOACK = 3,
    RADIO_TX_ERR = 4
}e_radio_tx_status_t;

/*==============================================================================
                          SYSTEM STRUCTURES AND OTHER TYPEDEFS
 =============================================================================*/

//! We are using IEEE 802.15.4
#define UIP_CONF_LL_802154                  TRUE
#define UIP_CONF_LLH_LEN                    0UL
#define	PRINT_PCK_STAT                      FALSE
#define TIMESTAMP_PERIOD_SEC                10UL	// in sec

#ifdef LINKADDR_CONF_SIZE
#define LINKADDR_SIZE                       LINKADDR_CONF_SIZE
#else /* LINKADDR_SIZE */
#define LINKADDR_SIZE                       8UL
#endif /* LINKADDR_SIZE */


typedef union {
    unsigned char u8[LINKADDR_SIZE];
} linkaddr_t;

/** \brief 16 bit 802.15.4 address */
typedef struct uip_802154_shortaddr {
    uint8_t addr[2];
} uip_802154_shortaddr;
/** \brief 64 bit 802.15.4 address */
typedef struct uip_802154_longaddr {
    uint8_t addr[8];
} uip_802154_longaddr;

#if UIP_CONF_LL_802154
/** \brief 802.15.4 address */
typedef uip_802154_longaddr                 uip_lladdr_t;
#define UIP_802154_SHORTADDR_LEN            2UL
#define UIP_802154_LONGADDR_LEN             8UL
#define UIP_LLADDR_LEN                      UIP_802154_LONGADDR_LEN
#else /*UIP_CONF_LL_802154*/
#if UIP_CONF_LL_80211
/** \brief 802.11 address */
typedef uip_80211_addr uip_lladdr_t;
#define UIP_LLADDR_LEN 6
#else /*UIP_CONF_LL_80211*/
/** \brief Ethernet address */
typedef uip_eth_addr uip_lladdr_t;
#define UIP_LLADDR_LEN 6
#endif /*UIP_CONF_LL_80211*/
#endif /*UIP_CONF_LL_802154*/


/*==============================================================================
 *                                                                             *
 *                                NETSTACK                                     *
 *                                                                             *
 =============================================================================*/


/*
********************************************************************************
*                           ENUMERATION DECLARATIONS
********************************************************************************
*/
/**
 * @brief   Netstack error code enumeration declaration
 */
typedef enum netstk_err
{
    /*
     * Common error codes
     */
    NETSTK_ERR_NONE                         = 0U,
    NETSTK_ERR_INIT                         = 1U,
    NETSTK_ERR_BUSY                         = 2U,
    NETSTK_ERR_BUF_OVERFLOW                 = 3U,
    NETSTK_ERR_INVALID_ARGUMENT             = 4U,
    NETSTK_ERR_INVALID_FRAME                = 5U,
    NETSTK_ERR_TX_TIMEOUT                   = 6U,
    NETSTK_ERR_TX_NOACK                     = 7U,
    NETSTK_ERR_CMD_UNSUPPORTED              = 8U,
    NETSTK_ERR_CHANNEL_ACESS_FAILURE        = 9U,
    NETSTK_ERR_FATAL                        = 10U,

    /*
     * LLC error codes
     */
    NETSTK_ERR_LLC_XXX                      = 100U,

    /*
     * MAC error codes
     */
    NETSTK_ERR_MAC_XXX                      = 200U,

    /*
     * PHY error codes
     */
    NETSTK_ERR_PHY_XXX                      = 300U,


    /*
     * LPR error codes
     */
    NETSTK_ERR_LPR_XXX                      = 400U,
    NETSTK_ERR_LPR_UNSUPPORTED_FRAME        = 401U,
    NETSTK_ERR_LPR_BROADCAST_LAST_STROBE    = 402U,
    NETSTK_ERR_LPR_BROADCAST_NOACK          = 403U,
    NETSTK_ERR_LPR_INVALID_ADDR             = 404U,
    NETSTK_ERR_LPR_TX_COLLISION_SAME_DEST   = 405U,
    NETSTK_ERR_LPR_TX_COLLISION_DIFF_DEST   = 406U,
    NETSTK_ERR_LPR_INVALID_ACK              = 407U,
    NETSTK_ERR_LPR_NO_STROBE,

    /*
     * RF error codes
     */
    NETSTK_ERR_RF_XXX                       = 500U,
    NETSTK_ERR_RF_SEND                      = 501U,

}e_nsErr_t;


/**
 * @brief   Netstack I/O Control command enumeration declaration
 */
typedef enum netstk_ioc_cmd
{
    /*
     * Common command codes
     */
    NETSTK_CMD_NONE = 0,            /*!< Reserved command code          */
    NETSTK_CMD_TX_CBFNCT_SET,       /*!< Set TX Callback function       */
    NETSTK_CMD_TX_CBARG_SET,        /*!< Set TX Callback argument       */
    NETSTK_CMD_RX_CBFNT_SET,        /*!< Set RX Callback function       */

    /*
     * LLC command codes
     */
    NETSTK_CMD_LLC_RSVD = 100U,

    /*
     * MAC command codes
     */
    NETSTK_CMD_MAC_RSVD = 200U,

    /*
     * PHY command codes
     */
    NETSTK_CMD_PHY_RSVD = 300U,


    /*
     * LPR command codes
     */
    NETSTK_CMD_LPR_RSVD = 400U,

    /*
     * RF command codes
     */
    NETSTK_CMD_RF_RSVD = 500U,      /*!< RF reserved command code       */
    NETSTK_CMD_RF_TXPOWER_SET,      /*!< Set TX Power                   */
    NETSTK_CMD_RF_TXPOWER_GET,      /*!< Get TX Power                   */
    NETSTK_CMD_RF_SENS_SET,         /*!< Set Sensitivity                */
    NETSTK_CMD_RF_SENS_GET,         /*!< Get Sensitivity                */
    NETSTK_CMD_RF_RSSI_GET,         /*!< Get RSSI                       */
    NETSTK_CMD_RF_CCA_GET,          /*!< Get CCA                        */
    NETSTK_CMD_RF_ANT_DIV_SET,      /*!< Set Antenna Div.               */
    NETSTK_CMD_RF_RF_SWITCH,        /*!< */
    NETSTK_CMD_RF_IS_RX_BUSY,       /*!< Indicate if RF is in RX state  */
    NETSTK_CMD_RF_IS_TX_BUSY,       /*!< Indicate if RF is in TX state  */
    NETSTK_CMD_RF_SYNC_SET,         /*!< Set SYNC words                 */
    NETSTK_CMD_RF_SYNC_GET,         /*!< Get SYNC words                 */

}e_nsIocCmd_t;


/*
********************************************************************************
*                            DATA TYPES DECLARATIONS
********************************************************************************
*/
typedef uint16_t        NETSTK_DEV_ID;

typedef void (* mac_callback_t)(void *ptr, int status, int transmissions);

typedef void (*nsTxCbFnct_t) (void *p_arg, e_nsErr_t *p_err);

typedef void (*nsRxCbFnct_t) (uint8_t *p_data, uint16_t len, e_nsErr_t *p_err);


/*
********************************************************************************
*                           STRUCTURE DECLARATIONS
********************************************************************************
*/
typedef struct netstack                     s_ns_t;
typedef struct netstack_socket              s_nsSocket_t;
typedef struct netstack_headerCompression   s_nsHeadComp_t;
typedef struct netstack_llsec               s_nsllsec_t;
typedef struct netstack_framer              s_nsFramer_t;
typedef struct netstk_module_api            s_nsModuleDrv_t;

typedef s_nsModuleDrv_t   s_nsLLC_t;
typedef s_nsModuleDrv_t   s_nsMAC_t;
typedef s_nsModuleDrv_t   s_nsPHY_t;
typedef s_nsModuleDrv_t   s_nsRF_t;


/**
 * @brief   Netstack driver structure declaration
 */
struct netstack
{
    const s_nsFramer_t      *frame;             /*!< Pointer to 6LoWPAN framing driver                  */
    const s_nsHeadComp_t    *hc;                /*!< Pointer to 6LoWPAN header compressor Driver        */
    const s_nsllsec_t       *llsec;             /*!< Pointer to Logical Link Security Driver            */
    const s_nsLLC_t         *llc;               /*!< Pointer to Logical Link Control Driver             */
    const s_nsMAC_t         *mac;               /*!< Pointer to Medium Access Control Driver            */
    const s_nsPHY_t         *phy;               /*!< Pointer to Physical Driver                         */
    const s_nsRF_t          *rf;                /*!< Pointer to Radio Frequency Driver                  */
    uint8_t                 c_configured;       /*!< Indicate whether netstack is already configured    */
};


/**
 * @brief   Netstack submodule driver structure declaration
 */
struct netstk_module_api
{
    char     *name;                                                             /*!< Driver name                    */
    void    (*init  )(void *p_netstk, e_nsErr_t *p_err);                        /*!< Initialization handler         */
    void    (*on    )(e_nsErr_t *p_err);                                        /*!< Turn the driver on             */
    void    (*off   )(e_nsErr_t *p_err);                                        /*!< Turn the driver on             */
    void    (*send  )(uint8_t *p_data, uint16_t len, e_nsErr_t *p_err);         /*!< Data transmission handler      */
    void    (*recv  )(uint8_t *p_data, uint16_t len, e_nsErr_t *p_err);         /*!< Data reception handler         */
    void    (*ioctrl)(e_nsIocCmd_t cmd, void *p_val, e_nsErr_t *p_err);         /*!< Miscellaneous functionality    */
};


/**
 * @brief   Netstack socket driver structure declaration
 */
struct netstack_socket
{
    char *name;

    /* Initialize the BSD socket driver */
    void (* create)(s_ns_t* p_ns);

    /* Connect to remote node*/
    void (* connect)(void);

    void (* bind)(void);

    /* Send data to remote node*/
    void (* send)(void);

    /* Send data to remote node*/
    void (* sendto)(void);

    /* Close the BSD socket driver */
    void (* close)(s_ns_t* p_ns);

};


struct netstack_headerCompression
{
    char *name;

    /** Initialize the network driver */
    void (* init)(s_ns_t* p_ns);

    /** Callback for getting notified of incoming packet. */
    void (* input)(void);
};


/**
 * The structure of a link layer security driver.
 */
struct netstack_llsec
{
    char *name;

    /** Initializes link layer security and thereafter starts upper layers. */
    void (* init)(s_ns_t* p_ns);

    /** Secures outgoing frames before passing them to NETSTACK_MAC. */
    void (* send)(mac_callback_t sent_callback, void *ptr);

    /**
     * Once the netstack_framer wrote the headers, the llsec driver
     * can generate a MIC over the entire frame.
     * \return Returns != 0 <-> success
     */
    int (* on_frame_created)(void);

    /**
     * Decrypts incoming frames;
     * filters out injected or replayed frames.
     */
    void (* input)(void);

    /** Returns the security-related overhead per frame in bytes */
    uint8_t (* get_overhead)(void);
};

struct netstack_framer
{
    char *name;

    int8_t (* init)(s_ns_t* p_ns);

    int8_t (* length)(void);

    int8_t (* create)(void);

    int8_t (* create_and_secure)(s_ns_t* p_ns);

    int8_t (* parse)(void);
};


/*
********************************************************************************
*                           SOCKET DRIVERS DECLARATIONS
********************************************************************************
*/
/*! Supported BSD-like socket interface */
/*extern const s_nsSocket_t udp_socket_driver;
extern const s_nsSocket_t tcp_socket_driver;*/


/*
********************************************************************************
*                       HEADER COMPRESSOR DRIVERS DECLARATIONS
********************************************************************************
*/
/*! Supported headers compression handlers */
extern const s_nsHeadComp_t     sicslowpan_driver;


/*! This driver are pretending to be a hc layer
 *  for sniffing data and sending them via USART     */
extern const s_nsHeadComp_t     slipnet_driver;


/*
********************************************************************************
*                           LLSEC DRIVERS DECLARATIONS
********************************************************************************
*/
/*! Supported link layer security handlers */
extern const s_nsllsec_t        nullsec_driver;


/*
********************************************************************************
*                   6LOWPAN FRAMER DRIVERS DECLARATIONS
********************************************************************************
*/
/*! Supported framers */
extern const s_nsFramer_t       framer_802154;


/*
********************************************************************************
*                           LLC DRIVERS DECLARATIONS
********************************************************************************
*/
#if NETSTK_CFG_LLC_NULL_EN
extern  const s_nsLLC_t   LLCDrvNull;
#endif

#if NETSTK_CFG_LLC_802154_EN
extern  const s_nsLLC_t   LLCDrv802154;
#endif


/*
********************************************************************************
*                           MAC DRIVERS DECLARATIONS
********************************************************************************
*/
#if NETSTK_CFG_MAC_NULL_EN
extern  const s_nsMAC_t   MACDrvNull;
#endif

#if NETSTK_CFG_MAC_802154_EN
extern  const s_nsMAC_t   MACDrv802154;
#endif

#if NETSTK_CFG_MAC_802154_ULE_EN
extern  const s_nsMAC_t   MACDrv802154ULE;
#endif


/*
********************************************************************************
*                           PHY DRIVERS DECLARATIONS
********************************************************************************
*/
#if NETSTK_CFG_PHY_NULL_EN
extern  const s_nsPHY_t   PHYDrvNull;
#endif


/*
********************************************************************************
*                           RF DRIVERS DECLARATIONS
********************************************************************************
*/
#if NETSTK_CFG_RF_NULL_EN
extern  const s_nsRF_t      RFDrvNull;
#endif

#if NETSTK_CFG_RF_NATIVE_EN
extern  const s_nsRF_t      RFDrvNative;
#endif

#if NETSTK_CFG_RF_CC1120_EN
extern  const s_nsRF_t      RFDrvCC1120;
#endif

#if NETSTK_CFG_RF_CC120X_EN
extern  const s_nsRF_t      RFDrvCC120x;
#endif


/*==============================================================================
                                 API FUNCTIONS
 =============================================================================*/

/*============================================================================*/
/*!
\brief   initialize all stack functions

    This function inits the board support packet and the complete netstack

\return returns 0 if failed, 1 if success
*/
/*============================================================================*/
uint8_t emb6_init(s_ns_t * ps_netstack);

/*============================================================================*/
/*!
\brief   emb6 process function

    This function handles all events and timers of the emb6 stack in a loop

\param 	 delay sets a delay in µs at the end of the function

\return  none

*/
/*============================================================================*/
void emb6_process(uint16_t delay);

/*============================================================================*/
/*!
\brief   Function which assign a given pointer to a current network stack ptr


\return  pointer to a stack

*/
/*============================================================================*/
s_ns_t * emb6_get(void);

/*=============================================================================
                                	UTILS SECTION
==============================================================================*/
// Used in several files.
#define CCIF
#define CLIF

#ifndef	QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                   4
#endif

#ifndef QUEUEBUF_CONF_REF_NUM
#define QUEUEBUF_CONF_REF_NUM               4
#endif

#endif /* EMB6_H_ */

/** @} */
/** @} */
