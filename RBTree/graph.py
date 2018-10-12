import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("report.csv")
df.plot(kind='line', x='count', y='time_add',color='green')
df.plot(kind='line', x='count', y='time_find',color='red')
df.plot(kind='line', x='count', y='time_del',color='blue')
plt.legend()
plt.show()