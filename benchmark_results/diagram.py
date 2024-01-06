import plotly.graph_objects as go
import json
from logging import info
from itertools import groupby

def convert_to_ms(value_list):
    result_list = []
    for value in value_list:
        split = value.split(' ')
        val  = split[0]
        size = split[1]
        if(size == "us"):
            result_list.append(str(float(val)/1000) + " ms")
        if(size == "s"):
            result_list.append(str(float(val)*1000) + " ms")
        else:
            result_list.append(value)

    return result_list


def parse_data(data):
    names = []
    means = []
    std_devs = []

    for benchmark in data['benchmarks']:
        for result in benchmark['results']:
            names.append(result['name'])
            means.append(result['mean'])
            std_devs.append(result['std_dev'])


    means = convert_to_ms(means)
    std_devs = convert_to_ms(std_devs)

    return names, means, std_devs


def format_data(names, means, std_devs, names_mpi, means_mpi, std_devs_mpi):
    r_names = names + names_mpi
    r_types = [name.split(' ')[1] for name in r_names]
    r_means = means + means_mpi
    r_means = [float(mean.split(' ')[0]) for mean in r_means]
    r_std_devs = std_devs + std_devs_mpi

    zipped = zip(r_means, r_names, r_std_devs, r_types)
    zipped_sorted = sorted(zipped, key=lambda x: x[3])

    grouped = {key: list(group) for key, group in groupby(zipped_sorted, key=lambda x: x[3])}

    for r_type, group in grouped.items():
        grouped[r_type] = sorted(group, key=lambda x:x[0])

    return grouped


benchmark_file = open("benchmark_results.json")
benchmark_file_mpi = open("mpi_benchmark_results.json")

benchmark_results = json.load(benchmark_file)
benchmark_results_mpi = json.load(benchmark_file_mpi)

names, means, std_devs = parse_data(benchmark_results)
names_mpi, means_mpi, std_devs_mpi = parse_data(benchmark_results_mpi)

groups = format_data(names, means, std_devs, names_mpi, means_mpi, std_devs_mpi)

fig = go.Figure()

for r_type, group in groups.items():
    r_means_group, r_names_group, r_std_devs_group, _ = zip(*group)

    fig.add_trace(go.Scatter(
        x=r_names_group,
        y=r_means_group,
        error_y=dict(type='data', array=r_std_devs_group),
        mode='markers',
        name=f'{r_type}',
        marker=dict(size=16)
    ))

fig.update_layout(
    title='Benchmark comparison',
    title_font_size=24,
    xaxis=dict(
        title='Benchmark Name',
        title_font=dict(size=24),
        tickfont=dict(size=20)
    ),
    yaxis=dict(
        title='Mean Time (ms)',
        title_font=dict(size=24),
        tickfont=dict(size=20)
    ),
    legend_title='Benchmark Type',
    legend_title_font_size=16,
    legend_font_size=14,
)

fig.show()
