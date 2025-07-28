import numpy as np


def sharpe_ratio(returns, risk_free_rate=0.0, periods_per_year=252):
    """Calculate the annualized Sharpe ratio."""
    r = np.asarray(returns)
    if r.size == 0:
        return 0.0
    excess = r - risk_free_rate / periods_per_year
    mean = excess.mean()
    std = excess.std(ddof=1)
    if std == 0:
        return 0.0
    return mean / std * np.sqrt(periods_per_year)


def max_drawdown(equity_curve):
    """Return the maximum drawdown for an equity curve."""
    eq = np.asarray(equity_curve)
    if eq.size == 0:
        return 0.0
    running_max = np.maximum.accumulate(eq)
    drawdowns = (eq - running_max) / running_max
    return drawdowns.min()
