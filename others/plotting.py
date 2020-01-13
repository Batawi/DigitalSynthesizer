import matplotlib.pyplot as plt
import csv

x = []
x2 = []

with open('silencer.log','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        x.append(float(row[0]))
        #x2.append(float(row[1]))


plt.plot(x, linewidth=1, label='Zapisy na dysku')
#plt.plot(x2, linewidth=1, label='Odczyty z dysku')


plt.xlabel('Liczba rekordow')
plt.ylabel('Liczba operacji dyskowych')
plt.title('Stosunek ilosci rekordow do ilosci operacji dyskowych')
plt.legend()
plt.grid()
plt.show()
