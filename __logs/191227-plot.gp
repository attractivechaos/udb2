set t po eps co noenh dashed dl 0.3 "Helvetica,18"

set style line 1  dt 1  lc rgb "#a6cee3" lw 3;
set style line 2  dt 1  lc rgb "#1f78b4" lw 3;
set style line 3  dt 3  lc rgb "#b2df8a" lw 3;
set style line 4  dt 4  lc rgb "#33a02c" lw 3;
set style line 5  dt 5  lc rgb "#fb9a99" lw 3;
set style line 6  dt 6  lc rgb "#e31a1c" lw 3;
set style line 7  dt 7  lc rgb "#fdbf6f" lw 3;
set style line 8  dt 8  lc rgb "#ff7f00" lw 3;
set style line 9  dt 9  lc rgb "#cab2d6" lw 3;
set style line 10 dt 2  lc rgb "#6a3d9a" lw 3;

set xlab "CPU seconds per million inputs"
set ylab "#bytes per distinct key"
set xran [0:0.14]

set pointsize 1.5

set yran [0:*]

set out "udb2-ins.eps"
set key top right font "Helvetica,16"
plot \
	"khashl/run-test.log" u 5:($6*4) t 'khashl' w lp ls 6, \
	"khash/run-test.log" u 5:($6*4) t 'khash' w lp ls 5, \
	"ska_bytell_hash_map/run-test.log" u 5:($6*4) t 'ska::bytell_hash_map' w lp ls 2, \
	"phmap/run-test.log" u 5:($6*4) t 'phmap::flat_hash_map' w lp ls 4, \
	"tsl_hopscotch_map/run-test.log" u 5:($6*4) t 'tsl::hopscotch_map' w lp ls 8, \
	"robin_hood/run-test.log" u 5:($6*4) t 'robin_hood::unordered_map' w lp ls 10, \
	"emilib/run-test.log" u 5:($6*4) t 'emilib::HashMap' w lp ls 7

set out "udb2-del.eps"
set key top left width -4 font "Helvetica,16"
plot \
	"khashl/run-test-del.log" u 5:($6*4) t 'khashl' w lp ls 6, \
	"khash/run-test-del.log" u 5:($6*4) t 'khash' w lp ls 5, \
	"ska_bytell_hash_map/run-test-del.log" u 5:($6*4) t 'ska::bytell_hash_map' w lp ls 2, \
	"phmap/run-test-del.log" u 5:($6*4) t 'phmap::flat_hash_map' w lp ls 4, \
	"tsl_hopscotch_map/run-test-del.log" u 5:($6*4) t 'tsl::hopscotch_map' w lp ls 8, \
	"robin_hood/run-test-del.log" u 5:($6*4) t 'robin_hood::unordered_map' w lp ls 10, \
	"emilib/run-test-del.log" u 5:($6*4) t 'emilib::HashMap' w lp ls 7
