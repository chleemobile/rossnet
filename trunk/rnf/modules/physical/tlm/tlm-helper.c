#include <tlm.h>

#define DEBUG 0

inline
double
tlm_getelevation(double * p)
{
	int	npe = tw_nnodes() * g_tw_npe;
	int	nrows_per_pe = ceil(g_tlm_spatial_grid[0] / npe);
	int	normalized_p0 = 0;

	if(0 == g_tw_mynode || g_tw_mynode == tw_nnodes() - 1)
		nrows_per_pe--;

	normalized_p0 = ((int) (p[0] / g_tlm_spatial_d[1])) %
					(nrows_per_pe + 2);

#if DEBUG
//if(!g_tw_mynode)
{
	printf("\n\t\tGETELEVATION\n");	
	printf("\t\t\tp(%lf, %lf) nrows %d, norm p0 %d\n", p[0], p[1], nrows_per_pe, normalized_p0);
}
#endif

	return g_tlm_z_values[normalized_p0][(int) (p[1] / g_tlm_spatial_d[1])];
}

inline
tw_lpid
tlm_getcell(double * position)
{
	tw_lpid	 id;

	double	 tmp = position[2];

	int	 n;
	int	 i;

	position[2] -= tlm_getelevation(position);

#if DEBUG
printf("\n\tGETCELL\n");
printf("\t\telev before %lf, %lf\n", tmp, position[2]);
#endif

	for(id = 0, i = g_tlm_spatial_dim-1; i >= 0; i--)
	{
		id += g_tlm_spatial_grid_i[i] ?
			g_tlm_spatial_grid_i[i] * (position[i] / g_tlm_spatial_d[i]) :
			(position[i] / g_tlm_spatial_d[i]);
#if DEBUG
		printf("\t\tid %ld (%lf, %lf, %lf)\n", id, position[0], position[1], position[2]);
#endif
	}

	// have to offset this LP id with respect to the PE it is on
	n = id / ntlm_lp_per_pe;
//((g_tlm_spatial_grid[0] * g_tlm_spatial_grid[1] * g_tlm_spatial_grid[2] / g_tw_npe * tw_nnode()) + g_tlm_spatial_offset);
#if DEBUG
printf("id %ld, n %d, += %d\n", id, n, g_tlm_spatial_offset * (n+1));
#endif
	id += g_tlm_spatial_offset * (n+1);
	position[2] = tmp;

#if DEBUG
printf("\t\telev after %lf\n", position[2]);
#endif

	return id;
}

inline
double	*
tlm_getlocation(tw_lp * lp)
{
	double	*position;
	tw_lpid	 id;
	int	 i;

	position = tw_calloc(TW_LOC, "position", sizeof(double) * g_tlm_spatial_dim, 1);
	id = lp->gid - (g_tlm_spatial_offset * (g_tw_mynode + 1));

#if DEBUG
//if(!g_tw_mynode)
printf("%ld: GETLOCATION: id %d\n", lp->gid, id);
#endif

	for(i = g_tlm_spatial_dim-1; id >= 0 && i >= 0; i--)
	{
		if(g_tlm_spatial_grid_i[i])
		{
			position[i] = floor(id / g_tlm_spatial_grid_i[i]);
		} else
			position[i] = id;

#if DEBUG
	//if(!g_tw_mynode)
		printf("\ti %d, p %lf \n", i, position[i]);
#endif

		if(id && id >= g_tlm_spatial_grid_i[i])
			id -= (position[i] * g_tlm_spatial_grid_i[i]);

		if(position[i] < 0 || position[i] > g_tlm_spatial_grid[i])
			tw_error(TW_LOC, "Off grid in %dD: 0 <= %d <= %d, LP %ld", 
				 i+1, position[i], g_tlm_spatial_grid[i], lp->id);

		position[i] *= g_tlm_spatial_d[i];

#if DEBUG
	//if(!g_tw_mynode)
		printf("\ti %d, p %lf \n\n", i, position[i]);
#endif
	}

#if DEBUG
//if(!g_tw_mynode)
	printf("\t\tp2 before %lf \n", position[2]);
#endif

	// offset Z-value by base
	position[2] += tlm_getelevation(position);

#if DEBUG
//if(!g_tw_mynode)
	printf("\t\tp2 after %lf \n", position[2]);
#endif

	if(tlm_getcell(position) != lp->gid)
	{
		printf("%d %lld %lld %lld: (%lf, %lf, %lf) gid: %lld != %lld\n", 
			g_tw_mynode, lp->gid, lp->id, id, 
			position[0], position[1], position[2], 
			lp->gid, tlm_getcell(position));

		if(rn_map(tlm_getcell(position)) == g_tw_mynode)
		{
			if(tw_getlocal_lp(tlm_getcell(position))->type.state_sz != sizeof(tlm_state))
				tw_error(TW_LOC, "got user model LP!");
		}

		if(tlm_getcell(position) != lp->gid)
			tw_error(TW_LOC, "%d: Did not get correct cell location!", 
				 g_tw_mynode);
	}

	return position;
}
