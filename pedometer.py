#!/usr/bin/env python3

import sys
import csv
import _pedometer

import matplotlib.pyplot as plt
import numpy as np


def PlotAccelerations(plot, title, t, x, y, z):
    plot.title.set_text(title)
    plot.plot(t, x, label='x')
    plot.plot(t, y, label='y')
    plot.plot(t, z, label='z')
    plot.legend(loc='upper left', shadow=True, fontsize='small')
    plot.set_xticks(np.arange(np.floor(min(t)), np.ceil(max(t)), 100))
    plot.set_yticks(np.arange(np.floor(min(min(x), min(y), min(z))),
                              np.ceil(max(max(x), max(y), max(z))), 0.5))


if len(sys.argv) < 2:
    sys.exit('Usage: python3 pedometer.py <FILE>')

with open(sys.argv[1]) as f:
    reader = csv.reader(f)
    next(reader, None)

    t, x, y, z = [], [], [], []
    for sample in reader:
        t.append(float(sample[0]) * 100)
        x.append(float(sample[1]))
        y.append(float(sample[2]))
        z.append(float(sample[3]))

fig, (pltData, pltAccUser, pltAccGravity) = plt.subplots(3, sharex='col')
fig.set_size_inches(19.2, 10.8, forward=True)

pdm = _pedometer.Pedometer([x, y, z])
PlotAccelerations(pltData, 'Input acceleration', t, x, y, z)
PlotAccelerations(pltAccUser, 'User acceleration',
                  t, pdm.accUser[0], pdm.accUser[1], pdm.accUser[2])
PlotAccelerations(pltAccGravity, 'Gravity acceleration',
                  t, pdm.accGravity[0], pdm.accGravity[1], pdm.accGravity[2])

fig.tight_layout()
fig.show()
fig.savefig("1.png", dpi=100)
