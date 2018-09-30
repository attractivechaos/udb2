This repo implements a *micro*-benchmark to evaluate the performance of various
hashtable libraries in C/C++. Each library is given *N* 32-bit integers with
~25% of them are distinct. The task is to find the occurrence of each distinct
integer with a hash table. *N* takes six values 10, 18, 26, 34, 42 and 50
million to show the effect of rehashing. The peak memory and the time for each
*N* is recorded. Results can be found in the [\_\_log](__log) directory. The
following figure shows the memory vs runtime when the benchmark is run on
"n1-standard-1" machine from the Google Cloud:

![](https://raw.githubusercontent.com/attractivechaos/udb2/master/__logs/180929-gcloud-a.png)

It is worth noting that the results vary with machines. The following figure
was derived from runs on a server with a much larger cache:

![](https://raw.githubusercontent.com/attractivechaos/udb2/master/__logs/180929-server-a.png)
