#!/usr/bin/env python3

import sys
import json

class Benchmark(object):
  def __init__(self, name):
    self.name = name
    self.mean = 0
    self.median = 0
    self.stddev = 0
    self.iterations = []

  def parse(self, obj):
    value = obj['cpu_time']
    if obj['run_type'] == 'iteration':
      self.iterations.append(value)
    else:
      self.__dict__[obj['aggregate_name']] = round(value, 2)

  def __repr__(self):
    return str(self.__dict__)

def get_benchmarks(bench_data):
  benchmarks = {}
  current_name = None
  current_benchmark = None
  for entry in bench_data['benchmarks']:
    name = entry['run_name']
    if current_name is None or not name == current_name:
      current_name = name
      current_benchmark = Benchmark(name)
      benchmarks[name] = current_benchmark

    current_benchmark.parse(entry)

  return benchmarks

def get_chartjs_bar_chart(benchmarks):
  labels = []
  data = []
  for b in benchmarks.values():
    labels.append(b.name)
    data.append(b.median)

  obj = {
    'labels': labels,
    'datasets': [{
      'label': 'Benchmark',
      'data': data
    }],
  }
  return json.dumps(obj, indent=2)

if __name__ == '__main__':
  filename = sys.argv[1]
  with open(filename) as f:
    bench_data = json.load(f)
    benchmarks = get_benchmarks(bench_data)
    print(get_chartjs_bar_chart(benchmarks))
