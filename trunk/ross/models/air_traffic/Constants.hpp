const char *GRAPH_CSV_FILE_PATH = "adjlist.csv";

const int DEBUG = 0;

const int NUMBER_OF_LP = 348;
const int NUMBER_OF_REGION_CONTROLLER = 20;
const int NUMBER_OF_PLANES_PER_AIRPORT = 1;

const float MEAN_DEQ = 5.0;
const float MEAN_ARRIVAL = 6.2;
const float MEAN_REQ = 4.8;
const float MEAN_TAXI = 3.0;
const float MEAN_FLIGHT = 30.0;
const float MEAN_LAND = 10.0;
const float MEAN_DELAY = 4.0;
const float MEAN_TAKE_OFF = 5.5;

const int NUMBER_OF_RUNWAY_LARGE_AIRPORT = 6;
const int NUMBER_OF_RUNWAY_MEDIUM_AIRPORT =  3;
const int NUMBER_OF_RUNWAY_SMALL_AIRPORT = 2;
const int NUMBER_OF_RUNWAY_NH_AIRPORT = 1;

const int AIRCRAFT_CAPACITY_OF_LARGE_REGION = 200;
const int AIRCRAFT_CAPACITY_OF_MEDIUM_REGION = 100;
const int AIRCRAFT_CAPACITY_OF_SMALL_REGION = 50;

static int total_transit_accepted = 0;
static int total_transit_rejected = 0;

static int total_dep_req_accepted = 0;
static int total_dep_req_rejected = 0;

static int total_landing_req_accepted = 0;
static int total_landing_req_rejected = 0;

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;
static int p_run = 1;
static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 100000;
static int       planes_per_airport = NUMBER_OF_PLANES_PER_AIRPORT;

static tw_stime	 wait_time_avg = 0.0;
