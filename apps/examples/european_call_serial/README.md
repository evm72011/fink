## European Call (CPU Serial Monte Carlo)

This example prices a European call option under the Geometric Brownian Motion model using a single-threaded Monte Carlo implementation on the CPU.  
It also prints the analytical Black–Scholes price for comparison.

### Sample output

```
Geometric Brownian motion parameters
Spot price..............................100
Volatility (stddev).....................0.1
Risk free rate..........................0.05

European call option
Time to maturity (years)................1.5
Strike price............................120

Black-Scholes analytical price..........1.35369

Monte Carlo serial
paths:..................................1M
price:..................................1.35571
std_err:................................0.00411625
```

The Monte Carlo result should be close to the analytical price within a few standard errors.
