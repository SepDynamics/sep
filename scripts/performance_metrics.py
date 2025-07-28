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


def calculate_advanced_metrics(returns):
    """Return common risk-adjusted performance metrics."""
    r = np.asarray(returns)
    if r.size == 0:
        return {
            "sharpe": 0.0,
            "calmar": 0.0,
            "max_dd": 0.0,
            "var_95": 0.0,
        }

    sr = sharpe_ratio(r)
    equity = (1 + r).cumprod()
    max_dd = max_drawdown(equity)
    annual_return = equity[-1] ** (252.0 / len(r)) - 1.0
    calmar = annual_return / abs(max_dd) if max_dd != 0 else 0.0
    var_95 = np.quantile(r, 0.05)

    return {
        "sharpe": sr,
        "calmar": calmar,
        "max_dd": max_dd,
        "var_95": var_95,
    }
