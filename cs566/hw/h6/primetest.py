import math
import sys

num = 299
for i in range(2, int(math.sqrt(num))+1):
    if num % i == 0:
        print("{} is not a prime number, it has factor {}".format(num, i))
        sys.exit(-1)
    
print("{} is a prime number".format(num))
        
