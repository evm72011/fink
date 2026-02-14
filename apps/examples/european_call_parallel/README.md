## European Call (CPU Parallel Monte Carlo)

This example prices a European call option under the Geometric Brownian Motion model using a multi-threaded Monte Carlo implementation on the CPU.  
It also prints the analytical Black–Scholes price for a quick sanity check.

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

Monte Carlo parallel
paths:..................................1M
threads:................................10
price:..................................1.35205
std_err:................................0.0041136
```

The Monte Carlo result should be close to the analytical price within a few standard errors.