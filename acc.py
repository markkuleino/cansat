import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('satelliitti2021.log',
                   sep=" ",
                   header = None,
                   names=["v1", "v2", "v3", "ax", "ay", "az"])
#data.columns = ["time", "v1", "v2", "v3", "ax", "ay", "az"]
#df.index.name="foo"
print( df )
print( df.columns )
print(df.v1)

df.plot()
plt.show()
