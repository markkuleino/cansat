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



ax = df.plot( y = ['N', 'a0', 'a1'], title='Cansat', lw=2, fontsize=14, grid=True)
ax.set_ylim(0,5)
ax.set_xlim(0,25000)
for line in ax.get_lines():
    if line.get_label() == 'N':
        line.set_linewidth(3)
plt.show()



