#ifndef INC_gvt_mpi_allreduce_h
#define INC_gvt_mpi_allreduce_h

static tw_stime gvt_print_interval = 0.05;
static tw_stime percent_complete = 0.0;

static inline int 
tw_gvt_inprogress(tw_pe * pe)
{
	return pe->gvt_status;
}

static inline void 
gvt_print(tw_stime gvt)
{
	if(gvt_print_interval == 1.0)
		return;

	if(percent_complete == 0.0)
	{
		percent_complete = gvt_print_interval;
		return;
	}

	int min = floor(100 * (gvt/g_tw_ts_end));
	if(min > 100) min = 100;

	printf("GVT #%d: simulation %d%% complete (", g_tw_gvt_done, min);

	if (gvt == DBL_MAX)
		printf("GVT = %s", "MAX");
	else
		printf("GVT = %.4f", gvt);

	printf(").\n");
	percent_complete += gvt_print_interval;
}

#endif
