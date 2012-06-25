batch_size=4;
gvt_interval=128;
num_core=2;
num_kp=15;
start=0;

max_num_core=4;
max_run=10;


#while [ $num_core -le $max_num_core ]
#do
#	start=0;
#	while [ $start -le $max_run ]
#	do
#
#		mpirun --mca btl self,sm,tcp -np $num_core ./air_traffic --sync=3 --nkp=$num_kp --gvt-interval=$gvt_interval --batch=$batch_size

#		start=$(( $start + 1 ))
#	done
#	num_core=$(( $num_core * 2 ))
#done

start=0;
while [ $start -le $max_run ]
do

	./air_traffic --sync=1 --nkp=$num_kp --gvt-interval=$gvt_interval --batch=$batch_size

	start=$(( $start + 1 ))
doner
