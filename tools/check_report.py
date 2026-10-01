#!/usr/bin/env python3
"""Refuse to publish an inconsistent objdiff progress report (decomp.dev shows whatever CI uploads).

  python3 tools/check_report.py build/report.json
"""
import json
import sys

r = json.load(open(sys.argv[1]))
errors = []
m = r.get('measures', {})
for k, v in m.items():
    if k.endswith('_percent') and not 0 <= float(v) <= 100:
        errors.append(f'{k} = {v} is outside 0..100')
for a, b in (('matched_code', 'total_code'), ('matched_functions', 'total_functions'), ('complete_code', 'total_code')):
    if int(m.get(a, 0)) > int(m.get(b, 0)):
        errors.append(f'{a} ({m.get(a)}) > {b} ({m.get(b)})')
if int(m.get('total_functions', 0)) < 1900:
    errors.append(f'total_functions = {m.get("total_functions")}: the report is missing units (expected ~1976)')
if not r.get('units'):
    errors.append('no units in the report')
if errors:
    sys.exit('report check failed:\n  ' + '\n  '.join(errors))
print(f'report OK: {m.get("matched_functions")}/{m.get("total_functions")} functions, '
      f'{float(m.get("matched_code_percent", 0)):.2f}% code')
