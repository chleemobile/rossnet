#include <ip.h>

void
ip_upstream(ip_state * state, rn_message * msg, tw_lp * lp)
{
	tw_event	*e;

	state->stats->s_avg_ttl += (g_rn_ttl - msg->ttl);
	state->stats->s_max_ttl = max(state->stats->s_max_ttl, g_rn_ttl - msg->ttl);
	state->stats->s_ncomplete++;
	msg->size -= g_ip_header;

	e = rn_event_new(lp->gid, 0.0, lp, UPSTREAM, msg->size);
	rn_event_send(e);

#if VERIFY_IP
	printf("%lld: IP UPSTREAM src %lld, dst %lld, ts %lf \n", 
		lp->gid, msg->src, msg->dst, tw_now(lp));
#endif
}

void
ip_rc_upstream(ip_state * state, rn_message * msg, tw_lp * lp)
{
#if VERIFY_IP
	printf("%lld: IP RC UPSTREAM src %lld, dst %lld, ts %lf \n", 
		lp->gid, msg->src, msg->dst, tw_now(lp));
#endif

	msg->size += g_ip_header;
	state->stats->s_ncomplete--;
		
	// not correct, must be state-saved
	state->stats->s_max_ttl = max(state->stats->s_max_ttl, g_rn_ttl - msg->ttl);
	state->stats->s_avg_ttl -= (g_rn_ttl - msg->ttl);

	rn_reverse_event_send(lp->gid, lp, UPSTREAM, msg->size);
}
