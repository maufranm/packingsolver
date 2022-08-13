# PackingSolver

A state-of-the-art solver for (geometrical) Packing Problems.

Only `rectangleguillotine` problem type has been implemented yet.

![Example](example.png?raw=true "Example")

## Problem type `rectangleguillotine`

Features:
* Objectives:
  * Bin packing
  * Knapsack
  * Strip packing X
  * Strip packing Y
  * Bin packing with leftovers
  * Variable-sized bin packing
* Item rotations
* Item stacks
* Bins with defects
* Trims
* Two- and three-staged, exact, non-exact, roadef2018 and homogenous patterns
* First cut vertical, horizontal or any
* Minimum and maximum distance between consecutive 1- and 2-cuts
* Minimum distance between cuts

## Usage

Compile:
```shell
bazel build -- //...
# Or, to enable VBPP objective:
bazel build --define cplex=true -- //...
```

Execute:
```shell
./bazel-bin/packingsolver/rectangleguillotine/main  --verbosity-level 1 --objective knapsack  --predefined 3NHO  --items data/rectangle/alvarez2002/ATP35_items.csv  --bins data/rectangle/alvarez2002/ATP35_bins.csv  --certificate ATP35_solution.csv  --output ATP35_output.json  --time-limit 1
```

Or in short:
```shell
./bazel-bin/packingsolver/rectangleguillotine/main -v 1 -f KP -p 3NHO -i data/rectangle/alvarez2002/ATP35 -c ATP35_solution.csv -o ATP35_output.json -t 1
```
```
===================================
           PackingSolver           
===================================

Problem type
------------
rectangleguillotine

Instance
--------
Objective:                Knapsack
Number of item types:     29
Number of items:          153
Number of bin types:      1
Number of bins:           1
Number of stacks:         29
Number of defects:        0
Cut type 1:               ThreeStagedGuillotine
Cut type 2:               NonExact
First stage orientation:  Horinzontal
min1cut:                  0
max1cut:                  -1
min2cut:                  0
max2cut:                  -1
Minimum waste:            1
one2cut:                  0
No item rotation:         1
Cut through defects:      0

        Time        Profit   # items                         Comment
        ----        ------   -------                         -------
       0.000         68970         1              IBS (thread 1) q 1
       0.000         72000         1              IBS (thread 1) q 1
       0.007         76395         2              IBS (thread 1) q 1
       0.007         90705         2              IBS (thread 1) q 1
       0.008        140970         2              IBS (thread 4) q 1
       0.008        144000         2              IBS (thread 4) q 1
       0.008        212970         3              IBS (thread 4) q 1
       0.008        216000         3              IBS (thread 4) q 1
       0.008        284970         4              IBS (thread 4) q 1
       0.008        292395         5              IBS (thread 4) q 1
       0.008        303517        12              IBS (thread 1) q 1
       0.009        306705         5              IBS (thread 4) q 1
       0.009        348839         5              IBS (thread 4) q 1
       0.009        358042         6              IBS (thread 4) q 1
       0.009        372343         6              IBS (thread 4) q 1
       0.009        379768         7              IBS (thread 4) q 1
       0.009        388389         7              IBS (thread 4) q 1
       0.009        408379         7              IBS (thread 4) q 1
       0.010        415804         8              IBS (thread 4) q 1
       0.010        424425         8              IBS (thread 4) q 1
       0.010        444415         8              IBS (thread 4) q 1
       0.010        451840         9              IBS (thread 4) q 1
       0.010        460461         9              IBS (thread 4) q 1
       0.010        480451         9              IBS (thread 4) q 1
       0.010        496497        10              IBS (thread 4) q 1
       0.011        502186        10              IBS (thread 4) q 1
       0.011        523921        11              IBS (thread 4) q 1
       0.011        539967        12              IBS (thread 4) q 1
       0.011        577834        21              IBS (thread 3) q 1
       0.012        581548         9              IBS (thread 4) q 2
       0.012        588973        10              IBS (thread 4) q 2
       0.012        597058        10              IBS (thread 4) q 2
       0.012        599368        11              IBS (thread 4) q 2
       0.013        602118        14              IBS (thread 1) q 2
       0.015        605793        11              IBS (thread 4) q 9
       0.019        606147        13             IBS (thread 4) q 19
       0.025        606672        12             IBS (thread 4) q 42
       0.037        607062        14             IBS (thread 4) q 94
       0.063        609550        15            IBS (thread 4) q 211
       0.104        610101        31            IBS (thread 3) q 141
       0.104        610578        31            IBS (thread 3) q 141
       0.105        610787        32            IBS (thread 3) q 141
       0.149        611135        34            IBS (thread 1) q 156
       0.221        614725        31            IBS (thread 3) q 316
       0.229        614967        42            IBS (thread 3) q 316
       0.276        616880        16            IBS (thread 2) q 857
       0.689        619897        28           IBS (thread 3) q 1066

Final statistics
----------------
Profit:            619897
Number of items:   28
Time:              1.0024
```

A solution visualizer is available here: https://librallu.gitlab.io/packing-viz/

## Benchmarks

The performances of PackingSolver have been compared to all published results from the scientific literature on corresponding Packing Problems.
Detailed results are available in `results_*.ods`.
`output.7z` contains all output files and solutions.

Do not hesitate to contact us if you are aware of any variant or article that we missed.

All experiments can be reproduced using the following scripts:
```shell
python3 packingsolver/scripts/bench.py "roadef2018_A" "roadef2018_B" "roadef2018_X" # ~50h
python3 packingsolver/scripts/bench.py "3NEGH-BPP-O" "3NEGH-BPP-R" "long2020_BPP" # ~10h
python3 packingsolver/scripts/bench.py "3GH-BPP-O" "3HG-BPP-O" "3HGV-BPP-O" # ~7h
python3 packingsolver/scripts/bench.py "2NEGH-BPP-O" "2NEGH-BPP-R" "2GH-BPP-O" # ~30h
python3 packingsolver/scripts/bench.py "3NEG-KP-O" "3NEG-KP-R" "3NEGV-KP-O" "3HG-KP-O" # ~10h
python3 packingsolver/scripts/bench.py "2NEG-KP-O" "2NEGH-KP-O" "2NEGV-KP-O" "2NEGH-KP-R"  # 1h
python3 packingsolver/scripts/bench.py "2G-KP-O" "2GH-KP-O" "2GV-KP-O" # 1m
python3 packingsolver/scripts/bench.py "3NEGH-SPP-O" "3NEGH-SPP-R" # ~20h
python3 packingsolver/scripts/bench.py "2NEGH-SPP-O" "2NEGH-SPP-R" # ~4h
python3 packingsolver/scripts/bench.py "3NEGH-CSP-O" "3NEGH-CSP-R" "long2020_CSP" # ~20h
python3 packingsolver/scripts/bench.py "3GH-CSP-O" "3HG-CSP-O" "3HGV-CSP-O" # ~15h
python3 packingsolver/scripts/bench.py "2NEGH-CSP-O" "2NEGH-CSP-R" "2GH-CSP-O" # ~30h
python3 packingsolver/scripts/bench.py "3NEG-VBPP-O" "3NEG-VBPP-R" "2GH-VBPP-O" "2GH-VBPP-R" # ~16h
```

