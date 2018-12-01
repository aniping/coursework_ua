import math

y = 6
lambd = 8

prob = 0.0
for i in range(y+1):
    prob += (math.pow(lambd, i) * math.exp(-lambd)) / math.factorial(i)

print 'The probability that Y<=6 for lambda=8: ', prob
