#!/bin/env python

import sys
import time

for i in xrange(5):
    sys.stdout.write(' '*10+'\r')
    sys.stdout.flush()
    print i
    sys.stdout.write(str(i)*(5-i)+'\r')
    sys.stdout.flush()
    time.sleep(1)

