import os
import re
import json

def parse_cpp_headers(source_dir):
    concepts = []
    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.endswith((".h", ".hpp")):
                try:
                    with open(os.path.join(root, file), 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                except IOError:
                    continue

                # Find all Doxygen-style comment blocks
                matches = re.findall(r'/\*\*(.*?)\*/', content, re.DOTALL)
                for match in matches:
                    lines = [line.strip() for line in match.strip().split('\n')]
                    cleaned_lines = [re.sub(r'^\s*\*\s?', '', line).strip() for line in lines]
                    
                    brief_search = re.search(r'@brief\s+(.*)', match)
                    if brief_search:
                        name = brief_search.group(1).strip()
                        # Remove all doxygen tags for description
                        desc_text = re.sub(r'@\w+.*', '', match)
                        # Clean up the description
                        desc_lines = [re.sub(r'^\s*\*\s?', '', line).strip() for line in desc_text.strip().split('\n')]
                        description = ' '.join(line for line in desc_lines if line)
                    else:
                        # No @brief, take the first line as name
                        non_empty_lines = [line for line in cleaned_lines if line and not line.startswith('@')]
                        if not non_empty_lines:
                            continue
                        name = non_empty_lines[0]
                        description = ' '.join(non_empty_lines[1:])

                    if name:
                        concepts.append({
                            'name': name,
                            'description': description or 'No description available.'
                        })

    # Remove duplicates
    unique_concepts = []
    seen_names = set()
    for concept in concepts:
        if concept['name'] not in seen_names:
            unique_concepts.append(concept)
            seen_names.add(concept['name'])
            
    return sorted(unique_concepts, key=lambda x: x['name'])


def main():
    source_dir = 'src'
    concepts = parse_cpp_headers(source_dir)
    # Write the concepts file to the repository root so it can be
    # served directly alongside index.html. Previously this script
    # wrote to the old `public/` directory.
    with open('concepts.json', 'w') as f:
        json.dump({'concepts': concepts}, f, indent=4)
    print(f"Generated concepts.json with {len(concepts)} concepts.")

if __name__ == '__main__':
    main()