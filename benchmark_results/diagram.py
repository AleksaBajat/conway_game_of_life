import plotly.graph_objects as go
import json
from logging import info


def parse_data(data):
    names = []
    means = []
    std_devs = []

    for benchmark in data['benchmarks']:
        for result in benchmark['results']:
            names.append(result['name'])
            means.append(result['mean'])
            std_devs.append(result['std_dev'])


    return names, means, std_devs


def sort_data(names, means, std_devs, names_mpi, means_mpi, std_devs_mpi):
    r_names = names + names_mpi
    r_means = means + means_mpi
    r_means = [float(mean.split(' ')[0]) for mean in r_means]
    r_std_devs = std_devs + std_devs_mpi

    r_means, r_names, r_std_devs = zip(*sorted(zip(r_means, r_names, r_std_devs)))
    return r_names, r_means, r_std_devs


benchmark_file = open("benchmark_results.json")
benchmark_file_mpi = open("mpi_benchmark_results.json")

benchmark_results = json.load(benchmark_file)
benchmark_results_mpi = json.load(benchmark_file_mpi)

names, means, std_devs = parse_data(benchmark_results)
names_mpi, means_mpi, std_devs_mpi = parse_data(benchmark_results_mpi)

r_names, r_means, r_std_devs = sort_data(names, means, std_devs, names_mpi, means_mpi, std_devs_mpi)

print(r_means)

fig = go.Figure()

fig.add_trace(go.Scatter(
    x=r_names,
    y=r_means,
    error_y=dict(type='data', array=r_std_devs),
    mode='markers',
    name='benchmarks'
))


fig.update_layout(
        title='Benchmark comparison',
        xaxis_title='Benchmark Name',
        yaxis_title='Mean Time (ms)',
        legend_title='Benchmark Type'
)

fig.show()

