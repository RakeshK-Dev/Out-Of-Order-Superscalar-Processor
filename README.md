# Out-Of-Order-Superscalar-Processor
Simulator for an out-of-order superscalar processor that fetches and issues N instructions per cycle. Only the dynamic scheduling mechanism will be modeled in detail, i.e., perfect caches and perfect branch prediction are assumed.

1. Type "make" to build.  (Type "make clean" first if you already compiled and want to recompile from scratch.)

2. Run trace reader:

   To run without throttling output:
   ./sim 256 32 4 gcc_trace.txt

   To run with throttling (via "less"):
   ./sim 256 32 4 gcc_trace.txt | less
