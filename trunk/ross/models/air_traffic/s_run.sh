batch_size=4;
gvt_interval=128;

num_core=2;
num_kp=15;
start=0;

max_num_core=4;
max_run=10;


while [ $start -le $max_run ]
do

	./air_traffic --sync=1 --nkp=$num_kp --gvt-interval=$gvt_interval --batch=$batch_size

	start=$(( $start + 1 ))
doner
