set style line 1 lt 1 lc rgb "#a6cee3" lw 3;
set style line 2 lt 2 lc rgb "#1f78b4" lw 3;
set style line 3 lt 3 lc rgb "#b2df8a" lw 3;
set style line 4 lt 4 lc rgb "#33a02c" lw 3;
set style line 5 lt 5 lc rgb "#fb9a99" lw 3;
set style line 6 lt 6 lc rgb "#e31a1c" lw 3;
set style line 7 lt 7 lc rgb "#fdbf6f" lw 3;
set style line 8 lt 8 lc rgb "#ff7f00" lw 3;
set style line 9 lt 9 lc rgb "#cab2d6" lw 3;
set style line 10 lt 10 lc rgb "#6a3d9a" lw 3;

set t po eps co "Helvetica,18"

set xlab "CPU seconds per million inputs"
set ylab "#bytes per distinct key"
set xran [0:*]

set pointsize 1.5

set yran [0:*]

set out "udb2-all.eps"
set key top left width -6 font "Helvetica,16"
plot \
	"_absl/run-test.log" u 5:($6*4) t 'absl::flag_hash_map' w lp ls 5, \
	"sparsehash/run-test.log" u 5:($6*4) t 'google_dense' w lp ls 6, \
	"sparsehash/run-test-1.log" u 5:($6*4) t 'google_sparse' w lp ls 9, \
	"khash/run-test-1.log" u 5:($6*4) t 'khash' w lp ls 7, \
	"libcuckoo/run-test.log" u 5:($6*4) t 'libcuckoo' w lp ls 8, \
	"ska_bytell_hash_map/run-test.log" u 5:($6*4) t 'ska::bytell_hash_map' w lp ls 1, \
	"sparsepp/run-test.log" u 5:($6*4) t 'sparsepp' w lp ls 10, \
	"tsl_robin_map/run-test.log" u 5:($6*4) t 'tsl::robin_map' w lp ls 4, \
	"c++11/run-test.log" u 5:($6*4) t 'std::unordered_map/gcc' w lp ls 2, \
	"uthash/run-test.log" u 5:($6*4) t 'uthash' w lp ls 3

set out "udb2-fast.eps"
set key top right width -6 font "Helvetica,16"
plot \
	"_absl/run-test.log" u 5:($6*4) t 'absl::flag_hash_map' w lp ls 5, \
	"sparsehash/run-test.log" u 5:($6*4) t 'google_dense' w lp ls 6, \
	"sparsehash/run-test-1.log" u 5:($6*4) t 'google_sparse' w lp ls 9, \
	"khash/run-test-1.log" u 5:($6*4) t 'khash' w lp ls 7, \
	"libcuckoo/run-test.log" u 5:($6*4) t 'libcuckoo' w lp ls 8, \
	"ska_bytell_hash_map/run-test.log" u 5:($6*4) t 'ska::bytell_hash_map' w lp ls 1, \
	"ska_flat_hash_map/run-test.log" u 5:($6*4) t 'ska::flat_hash_map' w lp ls 2, \
	"sparsepp/run-test.log" u 5:($6*4) t 'sparsepp' w lp ls 10, \
	"tsl_hopscotch_map/run-test.log" u 5:($6*4) t 'tsl::hopscotch_map' w lp ls 3, \
	"tsl_robin_map/run-test.log" u 5:($6*4) t 'tsl::robin_map' w lp ls 4
