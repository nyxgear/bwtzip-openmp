# -*- coding: utf-8 -*-
from pathlib import Path, PureWindowsPath
from os.path import basename
import csv
import subprocess
import os
import shutil
import numpy as np

application_root = subprocess.check_output(["git rev-parse --show-toplevel"], shell=True, encoding="UTF8")[:-1]
application_root = Path(application_root)
build_dir = 'cmake-build-release'

# datasets
datasets_folder_names = ['speedup_analysis_files']

# number of times to repeat the function computation
# and taking always the minimum result
repeat_computation_for_results_soundness = 3

# executable paths
bwtzip_path = application_root / build_dir / 'bwtzip'
pbwtzip_path = application_root / build_dir / 'pbwtzip'
pbwtzip2_path = application_root / build_dir / 'pbwtzip2'

# first line of csv with columns titles
bwtzip_csv_head_line = 'Filename,Chunk_size,Iterations,Times_lasted_longer_function1,Times_lasted_longer_function2,Times_lasted_longer_function3,Times_lasted_longer_function4,Avg_exec_time_function1,Avg_exec_time_function2,Avg_exec_time_function3,Avg_exec_time_function4,Total_compression_time'
pbwtzip_csv_head_line = 'Filename,Threads_configuration,Chunk_size,Iterations,Times_lasted_longer_read,Times_lasted_longer_stage1,Times_lasted_longer_stage2,Times_lasted_longer_stage3,Times_lasted_longer_write,Avg_exec_time_read,Avg_exec_time_stage1,Avg_exec_time_stage2,Avg_exec_time_stage3,Avg_exec_time_write,Total_compression_time\n'
pbwtzip2_csv_head_line = 'Filename,Threads_configuration,Chunk_size,Iterations,Times_lasted_longer_read,Times_lasted_longer_stage1,Times_lasted_longer_stage2,Times_lasted_longer_write,Avg_exec_time_read,Avg_exec_time_stage1,Avg_exec_time_stage2,Avg_exec_time_write,Total_compression_time'


def get_sequential_compression_times(chunk_sizes, dataset_names=datasets_folder_names,
                                     repeat_computation=repeat_computation_for_results_soundness):
    """
    Compute the total time required to compress all the files in the datasets by using
    chunk_sizes as parameter space and repeating the computation "repeat_computation"
    times (for results soundness) by taking always the minimum of the computed sum of compression times.
    """
    # remove output dir and its contents
    output_base = application_root / build_dir / 'compression_output'

    min_compression_time = np.zeros(len(chunk_sizes))

    # repeat t times
    for t in range(repeat_computation):
        shutil.rmtree(output_base, ignore_errors=True)

        total_compression_time = np.zeros(len(chunk_sizes))
        for dataset_name in dataset_names:
            input_dir = application_root / 'dataset' / dataset_name
            input_files = [x for x in input_dir.iterdir() if x.is_file()]

            output_dir = output_base / dataset_name
            # create empty output dir
            os.makedirs(output_dir)

            for (cnk_idx, chunk_size) in enumerate(chunk_sizes):
                print(chunk_size)
                csv_log = output_base / 'bwtzip_{}_{}.csv'.format(dataset_name, chunk_size)

                with open(csv_log, 'w') as csv_file:
                    csv_file.write(bwtzip_csv_head_line)
                    for input_file in input_files:
                        filename = basename(input_file)
                        output_file = output_dir / (filename + '.bwtzip')

                        args = str(bwtzip_path) + " {} {} {}".format(chunk_size, input_file, output_file)
                        #print(args)
                        stats_row = os.popen(args).read()
                        csv_file.write(stats_row)
                        # print(stats_row[:-1])
                        # print(re.split(pattern, stats_row[:-1]))
                        b = np.genfromtxt([bwtzip_csv_head_line, stats_row], delimiter=',', names=True, dtype=float,
                                          encoding="UTF8")
                        # print(b['Total_compression_time'])
                        total_compression_time[cnk_idx] += b['Total_compression_time']
        print("Repetition ", t+1, "performed")
        print(total_compression_time)
        if t == 0:
            min_compression_time = total_compression_time
        else:
            min_compression_time = np.minimum(min_compression_time, total_compression_time)

    return min_compression_time


def get_parallel_compression_times(pbwtzipx_path,
                                   pbwtzipx_name,
                                   chunk_sizes,
                                   thread_confs,
                                   csv_head_line,
                                   dataset_names=datasets_folder_names,
                                   repeat_computation=repeat_computation_for_results_soundness):
    """
    Compute the total time required to compress all the files in the datasets by using
    chunk_sizes and thread_confs as parameter space and repeating the computation
    "repeat_computation" times (for results soundness) by taking always the minimum of
    the computed sum of times.
    """
    # remove output dir and its contents
    output_base = application_root / build_dir / 'compression_output'

    min_compression_time = np.zeros((len(chunk_sizes), len(thread_confs)))

    # repeat t times
    for t in range(repeat_computation):
        shutil.rmtree(output_base, ignore_errors=True)

        total_compression_time = np.zeros((len(chunk_sizes), len(thread_confs)))
        for dataset_name in dataset_names:
            input_dir = application_root / 'dataset' / dataset_name
            input_files = [x for x in input_dir.iterdir() if x.is_file()]
            output_dir = output_base / dataset_name
            # create empty output dir
            os.makedirs(output_dir)

            for (cnk_idx, chunk_size) in enumerate(chunk_sizes):
                for (thr_idx, thread_conf) in enumerate(thread_confs):
                    print(thread_conf, chunk_size)
                    csv_log = output_base / '{}_{}_{}_{}.csv'.format(pbwtzipx_name, dataset_name, thread_conf,
                                                                     chunk_size)

                    with open(csv_log, 'w') as csv_file:
                        csv_file.write(csv_head_line)
                        for input_file in input_files:
                            filename = basename(input_file)
                            output_file = output_dir / (filename + '.bwtzip')

                            args = str(pbwtzipx_path) + " {} {} {} {}".format(thread_conf, chunk_size, input_file,
                                                                              output_file)
                            # print(args)
                            stats_row = os.popen(args).read()
                            # stats_row = subprocess.check_output([args], shell=True, encoding="UTF8")
                            csv_file.write(stats_row)
                            # print(stats_row[:-1])
                            # print(re.split(pattern, stats_row[:-1]))
                            b = np.genfromtxt([csv_head_line, stats_row], delimiter=',', names=True,
                                              dtype=float, encoding="UTF8")
                            # print(b['Total_compression_time'])
                            total_compression_time[cnk_idx][thr_idx] += b['Total_compression_time']
        print("Repetition ", t+1, "performed")
        print(total_compression_time)
        if t == 0:
            min_compression_time = total_compression_time
        else:
            min_compression_time = np.minimum(min_compression_time, total_compression_time)

    return min_compression_time


def get_pbwtzip_parallel_compression_times(chunk_sizes, thread_confs, dataset_names=datasets_folder_names):
    return get_parallel_compression_times(pbwtzip_path,
                                          "pbwtzip",
                                          chunk_sizes,
                                          thread_confs,
                                          pbwtzip_csv_head_line,
                                          dataset_names)


def get_pbwtzip2_parallel_compression_times(chunk_sizes, thread_confs, dataset_names=datasets_folder_names):
    return get_parallel_compression_times(pbwtzip2_path,
                                          "pbwtzip2",
                                          chunk_sizes,
                                          thread_confs,
                                          pbwtzip2_csv_head_line,
                                          dataset_names)
