import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('data/I_pakastin_muok.log',
                 sep="\t",
                 header = None,
                 names=["S", "T", "N", "P", "ax", "ay", "az" ])


print( df )
print( df.dtypes )

ax = df.plot( y = ['T'], title='Cansat', lw=2, fontsize=14, grid=True)
#ax = df.plot( )
ax.set_ylim(0,5)
ax.set_xlim(0,3000)
for line in ax.get_lines():
    if line.get_label() == 'N':
        line.set_linewidth(3)
plt.show()




