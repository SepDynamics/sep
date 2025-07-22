import os
import re
import json

def parse_cpp_headers(source_dir):
    concepts = []
    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.endswith(".h") or file.endswith(".hpp"):
                with open(os.path.join(root, file), 'r') as f:
                    content = f.read()
                    # Find all documentation blocks
                    matches = re.findall(r'/\*\*(.*?)\*/', content, re.DOTALL)
                    for match in matches:
                        concept = {}
                        # Extract the brief description
                        brief = re.search(r'@brief(.*?)\n', match)
                        if brief:
                            concept['name'] = brief.group(1).strip()
                            # Extract the full description
                            description = re.search(r'\n(.*?)\n', match.replace(brief.group(0), ''))
                            if description:
                                concept['description'] = description.group(1).strip()
                                concepts.append(concept)
    return concepts

def main():
    source_dir = 'src'
    concepts = parse_cpp_headers(source_dir)
    with open('public/concepts.json', 'w') as f:
        json.dump({'concepts': concepts}, f, indent=4)

if __name__ == '__main__':
    main()