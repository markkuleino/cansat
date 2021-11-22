import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('data/N_jaakaappiKaikki.txt',
                 sep="|",
                 header = None,
                 names=["d", "S", "T", "N", "P", "a" ])

#Poista ylimääräiset merkit, eli S, T, N, P, A ja :. Poistetaan kaikki:
df = df.replace('[A-Z:]', '', regex=True)

#Kiihtyvyysdata on erotettu toisistaan välilyönnillä. Muutetaan sekin numerodataksi
#vaikka kiihtyvyysanturi ei toiminutkaan.
dfa = ( df['a'].str.split(expand=True).add_prefix('a') )
df = df.join( dfa ).drop("a", 1)

#Poista muutama ylimääräinen pystyrivi
df = df.drop("d",1)

#Muuta merkkijonot luvuiksi
df.S = df.S.astype(float)
df['T'] = df['T'].astype(float) #df.T on transpoosi!
df.N = df.N.astype(float)
df.P = df.P.astype(float)
df.a0 = df.a0.astype(float)
df.a1 = df.a1.astype(float)
df.a2 = df.a2.astype(float)

print( df)
print( df.dtypes )

#Muuta asteikko. Tiedetään että 
#    x   =>    y
# 4.44 V =>  -18 C
# 2.70 V =>   21 C
#
# k = Dy/Dx x + b
# Dy/Dx = (21+18)/(2.7-4.44) = -39/1.74 = -22,41
# y = k*x + b, ratkaistaan b: b = y - k*x
#                               = 21 + 22,41*2,70 = 81,5
# Siis T = -22,41*V + 120,5
#
df['NT'] = -22.41*df['N'] + 81.5

ax = df.plot( y = ['NT'], title='Cansat Temp in C', lw=2, fontsize=14, grid=True)
ax.set_ylim(-25,25)
ax.set_xlim(0,25000)
ax.set_xlabel("Time")
ax.set_ylabel("Temperature")
for line in ax.get_lines():
    if line.get_label() == 'N':
        line.set_linewidth(3)
plt.show()



