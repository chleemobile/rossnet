#include "airport.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>

tw_peid mapping(tw_lpid gid)
{
  return ((tw_peid )gid) / g_tw_nlp;
}

void init(airport_state *s,tw_lp *lp)
{
  struct BSStack *stack = ::new BSStack ;
  lp -> tw_lp::stack_pointer = stack;
  int i;
  tw_event *e;
  airport_message *m;
  s -> airport_state::landings = 0;
  s -> airport_state::planes_in_the_sky = 0;
  s -> airport_state::planes_on_the_ground = planes_per_airport;
  s -> airport_state::waiting_time = 0.0;
  s -> airport_state::furthest_flight_landing = 0.0;
  for (i = 0; i < planes_per_airport; i++) {
    e = tw_event_new((lp -> tw_lp::gid),bs_rand_exponential(s -> airport_state::rn,30.0),lp);
    m = ((airport_message *)(tw_event_data(e)));
    m -> airport_message::type = DEPARTURE;
    tw_event_send(e);
  }
}

void p_init(airport_state *s,tw_lp *lp)
{
  struct BSStack *stack = ::new BSStack ;
  lp -> tw_lp::stack_pointer = stack;
  int i;
  tw_event *e;
  airport_message *m;
  s -> airport_state::landings = 0;
  s -> airport_state::planes_in_the_sky = 0;
  s -> airport_state::planes_on_the_ground = planes_per_airport;
  s -> airport_state::waiting_time = 0.0;
  s -> airport_state::furthest_flight_landing = 0.0;
  for (i = 0; i < planes_per_airport; i++) {
    e = tw_event_new((lp -> tw_lp::gid),bs_rand_exponential2(s -> airport_state::rn,30.0,lp),lp);
    m = ((airport_message *)(tw_event_data(e)));
    m -> airport_message::type = DEPARTURE;
    tw_event_send(e);
  }
}

void event_handler(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int rand_result;
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
{
// Schedule a landing in the future	
//constructive operation
        s -> airport_state::planes_in_the_sky++;
//msg->saved_furthest_flight_landing = s->furthest_flight_landing;
        tw_stime __temp0__;
        __temp0__ = (tw_now(lp));
        s -> airport_state::furthest_flight_landing = std::max< tw_stime  > (s -> airport_state::furthest_flight_landing,__temp0__);;
        ts = bs_rand_exponential(s -> airport_state::rn,10.0);
        tw_stime __temp1__;
        __temp1__ = ((ts + (s -> airport_state::furthest_flight_landing)) - tw_now(lp));
        e = tw_event_new((lp -> tw_lp::gid),__temp1__,lp);;
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = LAND;
        m -> airport_message::waiting_time = ((s -> airport_state::furthest_flight_landing) - tw_now(lp));
        s -> airport_state::furthest_flight_landing += ts;
        tw_event_send(e);
        break; ;
      };
    }
    case DEPARTURE:
{
      s -> airport_state::planes_on_the_ground--;
      ts = bs_rand_exponential(s -> airport_state::rn,mean_flight_time);
      rand_result = (bs_rand_integer(s -> airport_state::rn,0,3));
      dst_lp = 0;
      switch(rand_result){
        case 0:
{
// Fly north
          if ((lp -> tw_lp::gid) < sqrt_nlp) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) + (sqrt_nlp_1 * sqrt_nlp));;
          }
          else {
            dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);;
          }
          break; ;
        }
        case 1:
{
// Fly south
          if ((lp -> tw_lp::gid) >= (sqrt_nlp_1 * sqrt_nlp)) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) - (sqrt_nlp_1 * sqrt_nlp));;
          }
          else {
            dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);;
          }
          break; ;
        }
        case 2:
{
// Fly east
          if (((lp -> tw_lp::gid) % sqrt_nlp) == sqrt_nlp_1) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);;
          }
          else {
            dst_lp = ((lp -> tw_lp::gid) + 1);;
          }
          break; ;
        }
        case 3:
{
// Fly west
          if (((lp -> tw_lp::gid) % sqrt_nlp) == 0) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);;
          }
          else {
            dst_lp = ((lp -> tw_lp::gid) - 1);;
          }
          break; ;
        };
      }
      e = tw_event_new(dst_lp,ts,lp);
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = ARRIVAL;
      tw_event_send(e);
      break; ;;
    }
    case LAND:
{
{
        s -> airport_state::planes_on_the_ground++;
        s -> airport_state::planes_in_the_sky--;
        s -> airport_state::landings++;
        s -> airport_state::waiting_time += (msg -> airport_message::waiting_time);
        double __temp2__;
        __temp2__ = bs_rand_exponential(s -> airport_state::rn,30.0);
        e = tw_event_new((lp -> tw_lp::gid),__temp2__,lp);;
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = DEPARTURE;
        tw_event_send(e);
        break; ;
      };
    };
  };
}

void event_handler_forward(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int __num0 = 0;
  int rand_result;
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
{
// Schedule a landing in the future	
//constructive operation
        __store__< int  > (s -> airport_state::planes_in_the_sky,lp);
        s -> airport_state::planes_in_the_sky++;
//msg->saved_furthest_flight_landing = s->furthest_flight_landing;
        tw_stime __temp0__;
        __temp0__ = (tw_now(lp));
        __store__(s -> airport_state::furthest_flight_landing,lp);
        s -> airport_state::furthest_flight_landing = std::max< tw_stime  > (s -> airport_state::furthest_flight_landing,__temp0__);;
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,10.0);
        tw_stime __temp1__;
        __temp1__ = ((ts + (s -> airport_state::furthest_flight_landing)) - tw_now(lp));
        e = tw_event_new((lp -> tw_lp::gid),__temp1__,lp);;
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = LAND;
        m -> airport_message::waiting_time = ((s -> airport_state::furthest_flight_landing) - tw_now(lp));
        s -> airport_state::furthest_flight_landing += ts;
        tw_event_send(e);
        break; ;
      };
    }
    __num0 -= 16;
    case DEPARTURE:
{
      __num0 += 16;
      __store__< int  > (s -> airport_state::planes_on_the_ground,lp);
      s -> airport_state::planes_on_the_ground--;
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,mean_flight_time);
      rand_result = (bs_rand_integer(s -> airport_state::rn,0,3));
      dst_lp = 0;
      switch(rand_result){
        case 0:
{
// Fly north
          if ((lp -> tw_lp::gid) < sqrt_nlp) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) + (sqrt_nlp_1 * sqrt_nlp));;
          }
          else {
            __num0 += 1;
            dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);;
          }
          break; ;
        }
        __num0 -= 2;
        case 1:
{
          __num0 += 2;
// Fly south
          if ((lp -> tw_lp::gid) >= (sqrt_nlp_1 * sqrt_nlp)) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) - (sqrt_nlp_1 * sqrt_nlp));;
          }
          else {
            __num0 += 1;
            dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);;
          }
          break; ;
        }
        __num0 -= 4;
        case 2:
{
          __num0 += 4;
// Fly east
          if (((lp -> tw_lp::gid) % sqrt_nlp) == sqrt_nlp_1) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);;
          }
          else {
            __num0 += 1;
            dst_lp = ((lp -> tw_lp::gid) + 1);;
          }
          break; ;
        }
        __num0 -= 6;
        case 3:
{
          __num0 += 6;
// Fly west
          if (((lp -> tw_lp::gid) % sqrt_nlp) == 0) {
// Wrap around
            dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);;
          }
          else {
            __num0 += 1;
            dst_lp = ((lp -> tw_lp::gid) - 1);;
          }
          break; ;
        };
        default:
        __num0 += 8;
      }
      e = tw_event_new(dst_lp,ts,lp);
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = ARRIVAL;
      tw_event_send(e);
      break; ;;
    }
    __num0 -= 32;
    case LAND:
{
      __num0 += 32;
{
        __store__< int  > (s -> airport_state::planes_on_the_ground,lp);
        s -> airport_state::planes_on_the_ground++;
        __store__< int  > (s -> airport_state::planes_in_the_sky,lp);
        s -> airport_state::planes_in_the_sky--;
        __store__< int  > (s -> airport_state::landings,lp);
        s -> airport_state::landings++;
        __store__(s -> airport_state::waiting_time,lp);
        s -> airport_state::waiting_time += (msg -> airport_message::waiting_time);
        double __temp2__;
        __store__< int  > (s -> airport_state::rn,lp);
        __temp2__ = bs_rand_exponential(s -> airport_state::rn,30.0);
        e = tw_event_new((lp -> tw_lp::gid),__temp2__,lp);;
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = DEPARTURE;
        tw_event_send(e);
        break; ;
      };
    };
    default:
    __num0 += 48;
  };
  __store__(__num0,lp);
}

void event_handler_reverse(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int __num0;
  __restore__(__num0,lp);
  if ((__num0 & 32) == 32) {
    __restore__(s -> airport_state::rn,lp);
    __restore__(s -> airport_state::waiting_time,lp);
    __restore__(s -> airport_state::landings,lp);
    __restore__(s -> airport_state::planes_in_the_sky,lp);
    __restore__(s -> airport_state::planes_on_the_ground,lp);
  }
  else {
    if ((__num0 & 16) == 16) {
      __restore__(s -> airport_state::rn,lp);
      __restore__(s -> airport_state::planes_on_the_ground,lp);
    }
    else {
      if ((__num0 & 16) == 0) {
        __restore__(s -> airport_state::rn,lp);
        __restore__(s -> airport_state::furthest_flight_landing,lp);
        __restore__(s -> airport_state::planes_in_the_sky,lp);
      }
      else {
      }
    }
  }
}

void fw_event_handler_ss(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int rand_result;
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
{
// Schedule a landing in the future	
        __store__< int  > ((s -> airport_state::planes_in_the_sky),lp);
        s -> airport_state::planes_in_the_sky++;
        __store__< double  > ((s -> airport_state::furthest_flight_landing),lp);
        msg -> airport_message::saved_furthest_flight_landing = (s -> airport_state::furthest_flight_landing);
        s -> airport_state::furthest_flight_landing = std::max< tw_stime  > (s -> airport_state::furthest_flight_landing,tw_now(lp));
        ts = bs_rand_exponential2(s -> airport_state::rn,10.0,lp);
        e = tw_event_new((lp -> tw_lp::gid),((ts + (s -> airport_state::furthest_flight_landing)) - tw_now(lp)),lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = LAND;
        m -> airport_message::waiting_time = ((s -> airport_state::furthest_flight_landing) - tw_now(lp));
        s -> airport_state::furthest_flight_landing += ts;
        tw_event_send(e);
        break; 
      }
    }
    case DEPARTURE:
{
{
        __store__< int  > ((s -> airport_state::planes_on_the_ground),lp);
        s -> airport_state::planes_on_the_ground--;
        ts = bs_rand_exponential2(s -> airport_state::rn,mean_flight_time,lp);
        rand_result = (bs_rand_integer2(s -> airport_state::rn,0,3,lp));
        dst_lp = 0;
        switch(rand_result){
          case 0:
{
// Fly north
            if ((lp -> tw_lp::gid) < sqrt_nlp) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) + (sqrt_nlp_1 * sqrt_nlp));
            else 
              dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);
            break; 
          }
          case 1:
{
// Fly south
            if ((lp -> tw_lp::gid) >= (sqrt_nlp_1 * sqrt_nlp)) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) - (sqrt_nlp_1 * sqrt_nlp));
            else 
              dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);
            break; 
          }
          case 2:
{
// Fly east
            if (((lp -> tw_lp::gid) % sqrt_nlp) == sqrt_nlp_1) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);
            else 
              dst_lp = ((lp -> tw_lp::gid) + 1);
            break; 
          }
          case 3:
{
// Fly west
            if (((lp -> tw_lp::gid) % sqrt_nlp) == 0) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);
            else 
              dst_lp = ((lp -> tw_lp::gid) - 1);
            break; 
          }
        }
        e = tw_event_new(dst_lp,ts,lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = ARRIVAL;
        tw_event_send(e);
        break; 
      }
    }
    case LAND:
{
{
        __store__< int  > ((s -> airport_state::planes_on_the_ground),lp);
        s -> airport_state::planes_on_the_ground++;
        __store__< int  > ((s -> airport_state::planes_in_the_sky),lp);
        s -> airport_state::planes_in_the_sky--;
        __store__< int  > ((s -> airport_state::landings),lp);
        s -> airport_state::landings++;
        __store__< double  > ((s -> airport_state::waiting_time),lp);
        s -> airport_state::waiting_time += (msg -> airport_message::waiting_time);
        e = tw_event_new((lp -> tw_lp::gid),bs_rand_exponential2(s -> airport_state::rn,30.0,lp),lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = DEPARTURE;
        tw_event_send(e);
        break; 
      }
    }
  }
}

void rc_event_handler_ss(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
      bs_rand_rvs(s -> airport_state::rn,lp);
      __restore__< tw_stime  > (s -> airport_state::furthest_flight_landing,lp);
      __restore__< int  > (s -> airport_state::planes_in_the_sky,lp);
//            s->planes_in_the_sky--;
//            s->furthest_flight_landing = msg->saved_furthest_flight_landing;
      break; 
    }
    case DEPARTURE:
{
      bs_rand_rvs(s -> airport_state::rn,lp);
      bs_rand_rvs(s -> airport_state::rn,lp);
      __restore__< int  > (s -> airport_state::planes_on_the_ground,lp);
      break; 
    }
    case LAND:
{
      bs_rand_rvs(s -> airport_state::rn,lp);
      __restore__< tw_stime  > (s -> airport_state::waiting_time,lp);
      __restore__< int  > (s -> airport_state::landings,lp);
      __restore__< int  > (s -> airport_state::planes_in_the_sky,lp);
      __restore__< int  > (s -> airport_state::planes_on_the_ground,lp);
    }
  }
}

void fw_event_handler(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int rand_result;
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
{
// Schedule a landing in the future	
        s -> airport_state::planes_in_the_sky++;
        msg -> airport_message::saved_furthest_flight_landing = (s -> airport_state::furthest_flight_landing);
        s -> airport_state::furthest_flight_landing = std::max< tw_stime  > (s -> airport_state::furthest_flight_landing,tw_now(lp));
        ts = bs_rand_exponential2(s -> airport_state::rn,10.0,lp);
        e = tw_event_new((lp -> tw_lp::gid),((ts + (s -> airport_state::furthest_flight_landing)) - tw_now(lp)),lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = LAND;
        m -> airport_message::waiting_time = ((s -> airport_state::furthest_flight_landing) - tw_now(lp));
        s -> airport_state::furthest_flight_landing += ts;
        tw_event_send(e);
        break; 
      }
    }
    case DEPARTURE:
{
{
        s -> airport_state::planes_on_the_ground--;
        ts = bs_rand_exponential2(s -> airport_state::rn,mean_flight_time,lp);
        rand_result = (bs_rand_integer2(s -> airport_state::rn,0,3,lp));
        dst_lp = 0;
        switch(rand_result){
          case 0:
{
// Fly north
            if ((lp -> tw_lp::gid) < sqrt_nlp) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) + (sqrt_nlp_1 * sqrt_nlp));
            else 
              dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);
            break; 
          }
          case 1:
{
// Fly south
            if ((lp -> tw_lp::gid) >= (sqrt_nlp_1 * sqrt_nlp)) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) - (sqrt_nlp_1 * sqrt_nlp));
            else 
              dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);
            break; 
          }
          case 2:
{
// Fly east
            if (((lp -> tw_lp::gid) % sqrt_nlp) == sqrt_nlp_1) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) - sqrt_nlp_1);
            else 
              dst_lp = ((lp -> tw_lp::gid) + 1);
            break; 
          }
          case 3:
{
// Fly west
            if (((lp -> tw_lp::gid) % sqrt_nlp) == 0) 
// Wrap around
              dst_lp = ((lp -> tw_lp::gid) + sqrt_nlp_1);
            else 
              dst_lp = ((lp -> tw_lp::gid) - 1);
            break; 
          }
        }
        e = tw_event_new(dst_lp,ts,lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = ARRIVAL;
        tw_event_send(e);
        break; 
      }
    }
    case LAND:
{
{
        s -> airport_state::planes_on_the_ground++;
        s -> airport_state::planes_in_the_sky--;
        s -> airport_state::landings++;
        s -> airport_state::waiting_time += (msg -> airport_message::waiting_time);
        e = tw_event_new((lp -> tw_lp::gid),bs_rand_exponential2(s -> airport_state::rn,30.0,lp),lp);
        m = ((airport_message *)(tw_event_data(e)));
        m -> airport_message::type = DEPARTURE;
        tw_event_send(e);
        break; 
      }
    }
  }
}

void rc_event_handler(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  switch((msg -> airport_message::type)){
    case ARRIVAL:
{
      s -> airport_state::planes_in_the_sky--;
      s -> airport_state::furthest_flight_landing = (msg -> airport_message::saved_furthest_flight_landing);
      bs_rand_rvs(s -> airport_state::rn,lp);
      break; 
    }
    case DEPARTURE:
{
      s -> airport_state::planes_on_the_ground++;
      bs_rand_rvs(s -> airport_state::rn,lp);
      bs_rand_rvs(s -> airport_state::rn,lp);
      break; 
    }
    case LAND:
{
      s -> airport_state::planes_on_the_ground--;
      s -> airport_state::planes_in_the_sky++;
      s -> airport_state::landings--;
      s -> airport_state::waiting_time -= (msg -> airport_message::waiting_time);
      bs_rand_rvs(s -> airport_state::rn,lp);
    }
  }
}

void final(airport_state *s,tw_lp *lp)
{
  wait_time_avg += (((s -> airport_state::waiting_time) / ((double )(s -> airport_state::landings))) / nlp_per_pe);
}
/*
 Sequential Running
 */
//tw_lptype airport_lps[] =
//{
//	{
//		(init_f) init,
//		(event_f) event_handler,
//		(revent_f) rc_event_handler,
//		(final_f) final,
//		(map_f) mapping,
//		sizeof(airport_state),
//	},
//	{0},
//};
/*
 Parallel Running
 */
tw_lptype airport_lps[] = {{((init_f )p_init), ((event_f )event_handler_forward), ((revent_f )event_handler_reverse), ((final_f )final), ((map_f )mapping), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_STIME), ("mean"), ("mean flight time for planes"), ((&mean_flight_time))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, {((tw_opttype )0), (0L), (0L), (0L)}};

int main(int argc,char **argv,char **env)
{
  int i;
  tw_opt_add(app_opt);
  tw_init(&argc,&argv);
  nlp_per_pe /= ((tw_nnodes()) * g_tw_npe);
  g_tw_events_per_pe = (((planes_per_airport * nlp_per_pe) / g_tw_npe) + opt_mem);
  tw_define_lps(nlp_per_pe,(sizeof(airport_message )),0);
  for (i = 0; i < g_tw_nlp; i++) 
    tw_lp_settype(i,(airport_lps + 0));
  sqrt_nlp = ((int )(sqrt(nlp)));
  sqrt_nlp_1 = (sqrt_nlp - 1);
  tw_run();
  if ((tw_ismaster())) {
    printf("\nAirport Model Statistics:\n");
    printf("\t%-50s %11.4lf\n","Average Waiting Time",wait_time_avg);
    printf("\t%-50s %11lld\n","Number of airports",((nlp_per_pe * g_tw_npe) * (tw_nnodes())));
    printf("\t%-50s %11lld\n","Number of planes",(((planes_per_airport * nlp_per_pe) * g_tw_npe) * (tw_nnodes())));
  }
  tw_end();
  cout<<memusage<<","<<endl;
  return 0;
}
