#!/bin/bash

# This is just a quick script to check the outputs of some of my utils
# against the system utils, to see if they're working somewhat
# correctly.

# cat
diff <(./cat cat.c) <(cat cat.c)
diff <(./cat -u cat.c) <(cat -u cat.c)

# nothing for echo because GNU echo is not XSI compliant.

# wc
diff <(./wc -l wc.c) <(wc -l wc.c)
diff <(./wc -w wc.c) <(wc -w wc.c)
diff <(./wc -m wc.c) <(wc -m wc.c)

# head
diff <(./tail tail.c) <(tail tail.c)
diff <(./tail -n 10 tail.c) <(tail -n 10 tail.c)

# tail
diff <(./tail tail.c) <(tail tail.c)
diff <(./tail -n -10 tail.c) <(tail -n -10 tail.c)
diff <(./tail -n +10 tail.c) <(tail -n +10 tail.c)



