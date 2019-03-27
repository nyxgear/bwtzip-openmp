# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
# This import registers the 3D projection, but is otherwise unused.
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import

scale_factor = 2
figsize_line_chart = (6.4 * scale_factor, 4.0 * scale_factor)
figsize_heat_map = (6.4 * scale_factor, 4.0 * scale_factor)
figsize_3d_bar = (18 * scale_factor, 7.0 * scale_factor)


def get_vis_cnk_sizes(chunk_sizes):
    return ['{0:.0f} Kb'.format(x / 1024) for x in chunk_sizes]


def plot_line_chart(title, parameters_matrix, chunk_sizes, thread_confs):
    plt.figure(figsize=figsize_line_chart)

    max_p = float('-inf')
    min_p = float('inf')

    for i, r in enumerate(parameters_matrix):
        plt.plot(thread_confs, r, 'C' + str(i) + 'o-', label='{} Kb'.format(int(chunk_sizes[i] / 1024)))

        max_p = max(max(r), max_p)
        min_p = min(min(r), min_p)
        # for t in zip(thread_confs, r):
        #    fmt = (t[0], round(t[1], 3))
        #    dd = (t[0], t[1] + 0.01)
        #    ax.annotate('%s, %s' % fmt, xy=dd, textcoords='data')

    plt.title(title, fontsize=17, pad=18)
    plt.grid(True)
    plt.ylim(min_p - 0.2, max_p + 0.2)
    plt.legend(loc='upper left', title='Chunk size', fontsize=12)
    plt.xlabel('Threads configuration', fontsize=15, labelpad=12)
    plt.ylabel('Speedup', fontsize=15, labelpad=12)
    plt.show()


def plot_heatmap(title, data, chunk_sizes, thread_confs, cmap, text_color):
    fig = plt.figure(figsize=figsize_heat_map)
    ax = fig.add_subplot(111)
    im = ax.imshow(data, cmap=cmap)

    farmers = thread_confs
    vegetables = chunk_sizes

    # We want to show all ticks...
    ax.set_xticks(np.arange(len(thread_confs)))
    ax.set_yticks(np.arange(len(chunk_sizes)))

    # ... and label them with the respective list entries
    ax.set_xticklabels(thread_confs, fontsize=12)
    ax.set_yticklabels(chunk_sizes, fontsize=12)

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right", rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    for i in range(len(vegetables)):
        for j in range(len(farmers)):
            text = ax.text(j, i, round(data[i, j], 2), fontsize=16, ha="center", va="center", color=text_color)

    ax.set_title(title, fontsize=18, pad=15)
    fig.tight_layout()
    plt.xlabel('Threads configuration', fontsize=15, labelpad=10)
    plt.ylabel('Chunk size', fontsize=15, labelpad=10)
    plt.colorbar(im)
    plt.show()


def plot_3d_bar(title, data, chunk_sizes, thread_confs, color):
    fig = plt.figure(figsize=figsize_3d_bar)
    ax = fig.add_subplot(121, projection='3d')

    _x = np.arange(len(chunk_sizes))
    _y = np.arange(len(thread_confs))
    _xx, _yy = np.meshgrid(_x, _y)
    x, y = _xx.ravel(), _yy.ravel()
    dz = data.flatten(order='C')
    z = np.zeros_like(dz)
    dx = 0.4
    dy = 0.4

    ax.bar3d(x, y, z, dx, dy, dz, color=color, shade=True)

    ax.set_title(title, pad=40, fontsize=17)

    _x_ticks = np.arange(dx / 2, len(chunk_sizes))
    _y_ticks = np.arange(dy / 2, len(thread_confs))

    # set ticks
    ax.w_xaxis.set_ticks(_x_ticks)
    ax.w_yaxis.set_ticks(_y_ticks)

    # set tick labels
    ax.set_xticklabels(chunk_sizes)
    ax.set_yticklabels(thread_confs)

    ax.set_xlabel("Chunk size", labelpad=20, fontsize=14)
    ax.set_ylabel("Threads configuration", labelpad=20, fontsize=14)
    ax.set_zlabel(title, labelpad=20, fontsize=14, rotation='vertical')

    idx = 0
    for _y in _y_ticks:
        for _x in _x_ticks:
            lbl = str(round(dz[idx], 2))
            ax.text(_x, _y, dz[idx] + 0.05, lbl, color='k', fontsize=12,
                    backgroundcolor=(1, 1, 1, 0.8),
                    va='top')
            idx += 1

    ax.view_init(azim=-60, elev=55)

    plt.show()
