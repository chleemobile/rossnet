/*********************************************************************
		              tcp-extern.h
*********************************************************************/

#ifndef TCP_EXTERN_H
#define TCP_EXTERN_H

#include "tcp.h"

/* tcp-main.c */
extern void  tcp_finalize(tcpStatistics *Stat);

/* tcp-host.c */
extern void  tcp_host_startup(Host_State *SV, tw_lp *lp);
extern void  tcp_host_process(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_process_ack(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_process_data(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_update_cwnd(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_update_rtt(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_timeout(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_eventhandler(Host_State *SV, tw_bf *CV,Msg_Data *M, tw_lp *lp);
extern void  tcp_host_statistics_collectStats(Host_State *SV, tw_lp *lp);

/* tcp-router.c */
extern void  tcp_router_startUp(Router_State *SV, tw_lp *lp);
extern void  tcp_router_forward(Router_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_router_eventhandler(Router_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_router_statistics_collectStats(Router_State *SV, tw_lp *lp);

/* tcp-host-rc.c */
extern void  tcp_host_process_rc(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_process_ack_rc(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_process_data_rc(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_timeout_rc(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_host_rc_eventhandler(Host_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);

/* tcp-router-rc.c */
extern void  tcp_router_forward_rc(Router_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);
extern void  tcp_router_rc_eventhandler(Router_State *SV, tw_bf *CV, Msg_Data *M, tw_lp *lp);

/* tcp-util.c */
extern tw_lpid tcp_util_nexthop(tw_lp *lp, int dest);
extern int    tcp_util_nexthop_link(tw_lp *lp, int dest);
extern void   tcp_util_event(tw_lp *lp, int type, int source, int dest, int seq_num, int ack, double ts);

/* tcp-init.c */
extern void tcp_init();

/* rf-to-ross.c */

extern void init_node_list();
extern int read_rocketfuel_file( char *filename );
extern void color_topology( int levels );

extern Tcp_Statistics g_tcp_stats;
extern int g_hosts;
extern int g_routers;
extern int g_npe;
extern int g_nkp;
extern int g_iss;
extern int g_mss;
extern int g_recv_wnd;
extern int g_frequency;
extern int number_of_nodes;
extern int g_type;
extern int g_num_links;

extern char          **g_new_routing_table;
extern Routing_Table **g_routing_table;
extern Router_Link   **g_routers_links;
extern int            *g_routers_info;
extern Host_Link      *g_hosts_links;
extern Host_Info      *g_hosts_info;


#endif




