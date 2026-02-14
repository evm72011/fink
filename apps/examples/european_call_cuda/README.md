## European Call (GPU Monte Carlo)

This example prices a European call option under the Geometric Brownian Motion model.
It compares the analytical Black–Scholes price with a Monte Carlo estimate computed on the GPU.

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

Monte Carlo GPU
paths:..................................1M
price:..................................1.35639
std_err:................................0.0041215

```

The Monte Carlo result should be close to the analytical price within a few standard errors.