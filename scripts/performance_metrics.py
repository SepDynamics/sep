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


def information_ratio(strategy_returns, benchmark_returns, periods_per_year=252):
    """Return the annualized information ratio."""
    sr = np.asarray(strategy_returns)
    br = np.asarray(benchmark_returns)
    if sr.size == 0 or br.size == 0 or sr.size != br.size:
        return 0.0
    active = sr - br
    mean = active.mean()
    std = active.std(ddof=1)
    if std == 0:
        return 0.0
    return mean / std * np.sqrt(periods_per_year)


def profit_factor(returns):
    """Return the profit factor for a series of returns."""
    r = np.asarray(returns)
    if r.size == 0:
        return 0.0
    gains = r[r > 0].sum()
    losses = -r[r < 0].sum()
    if losses == 0:
        return float("inf") if gains > 0 else 0.0
    return gains / losses


def calculate_advanced_metrics(returns, benchmark_returns=None):
    """Return common risk-adjusted performance metrics."""
    r = np.asarray(returns)
    if r.size == 0:
        return {
            "sharpe": 0.0,
            "calmar": 0.0,
            "max_dd": 0.0,
            "var_95": 0.0,
            "info_ratio": 0.0,
            "profit_factor": 0.0,
        }

    sr = sharpe_ratio(r)
    equity = (1 + r).cumprod()
    max_dd = max_drawdown(equity)
    annual_return = equity[-1] ** (252.0 / len(r)) - 1.0
    calmar = annual_return / abs(max_dd) if max_dd != 0 else 0.0
    var_95 = np.quantile(r, 0.05)
    info = information_ratio(r, benchmark_returns) if benchmark_returns is not None else 0.0
    pf = profit_factor(r)

    return {
        "sharpe": sr,
        "calmar": calmar,
        "max_dd": max_dd,
        "var_95": var_95,
        "info_ratio": info,
        "profit_factor": pf,
    }
