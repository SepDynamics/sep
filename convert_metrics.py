import json
import re
import sys

def main():
    data = []
    lines = sys.stdin.readlines()
    current_data = {}
    for line in lines:
        if line.startswith('Metrics for'):
            if current_data:
                data.append(current_data)
            current_data = {'name': line.split('for ')[1].strip()}
        m = re.search(r'Average Coherence: ([\d\.]+)', line)
        if m:
            current_data['coherence'] = float(m.group(1))
        m = re.search(r'Average Stability: ([\d\.]+)', line)
        if m:
            current_data['stability'] = float(m.group(1))
        m = re.search(r'Average Entropy:   ([\d\.]+)', line)
        if m:
            current_data['entropy'] = float(m.group(1))
    if current_data:
        data.append(current_data)
    print(json.dumps(data))

if __name__ == '__main__':
    main()