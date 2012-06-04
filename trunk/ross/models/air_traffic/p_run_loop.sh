batch_size=2;
gvt_interval=2;
loop_size=4000;
num_core=2;
num_kp=15;

max_num_core=4;
max_loop_size=4000;
max_batch_size=64;
max_gvt_interval=4096;

while [ $num_core -le $max_num_core ]
do
	loop_size=4000;
	while [ $loop_size -le $max_loop_size ]
	do
		batch_size=2;
		while [ $batch_size -le $max_batch_size ]
		do
			gvt_interval=2;
			while [ $gvt_interval -le $max_gvt_interval ]
			do
				mpirun --mca btl self,sm,tcp -np $num_core ./air_traffic --sync=3 --nkp=$num_kp --gvt-interval=$gvt_interval --batch=$batch_size --loopsize=$loop_size
				gvt_interval=$(( $gvt_interval * 2 ))
			done	
			batch_size=$(( $batch_size * 2 ))
		done
		loop_size=$(( $loop_size * 2 ))
	done
	num_core=$(( $num_core * 2 ))
done

batch_size=2;
gvt_interval=2;
loop_size=500;
num_kp=15;


#while [ $loop_size -le $max_loop_size ]
#do
#	batch_size=2;
#	while [ $batch_size -le $max_batch_size ]
#	do
#		gvt_interval=2;
#		while [ $gvt_interval -le $max_gvt_interval ]
#		do
#			./air_traffic --sync=1 --nkp=$num_kp --gvt-interval=$gvt_interval --batch=$batch_size --loopsize=$loop_size
#			gvt_interval=$(( $gvt_interval * 2 ))
#		done	
#		batch_size=$(( $batch_size * 2 ))
#	done
#	loop_size=$(( $loop_size * 2 ))
3done

