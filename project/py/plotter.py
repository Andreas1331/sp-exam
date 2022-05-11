import mplfinance as fplt
import matplotlib.pyplot as plt
import pandas as pd

# We just assume the files exists
candles = pd.read_json("candle_data.JSON")
oscillators = pd.read_json("indicators.JSON")

# Extracting Data for plotting
ohlc = candles.loc[:, ['time', 'opening_price', 'highest', 'lowest', 'closing_price']]
# mlpfinance requires the index to be of DatetimeIndex
ohlc = ohlc.set_index(pd.DatetimeIndex(ohlc['time']))
ohlc = ohlc.drop(['time'], axis=1)
# We also need to rename the columns for mlpfinance to distinguish them
ohlc.columns = ['Open', 'High', 'Low', 'Close']
oscillators.columns = ['Blue', 'Red']

fplt.plot(
    ohlc,
    type='candle',
    title='Danske Bank, 1-day candlesticks',
    ylabel='Price (DKK)',
    style='charles' # Classic red/green style
)

# plot lines
plt.axhline(y=80, label="Overbought area", linestyle='dashed', color='y', markersize=12)
plt.axhline(y=20, label="Oversold area", linestyle='dashed', color='g', markersize=12)
plt.plot(oscillators.Blue, label="Blue", color='b')
plt.plot(oscillators.Red, label="Red", color='r')
plt.title("Danske Bank, 1-day stochastic oscillators")
plt.legend()
plt.show()