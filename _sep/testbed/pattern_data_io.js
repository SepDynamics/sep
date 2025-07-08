import fs from 'fs';

if (process.argv.includes('--generate')) {
  const obj = {
    id: 'node-origin',
    generation: 3,
    position: [1, 2, 3, 4],
    coherence: 0.5,
    stability: 0.6,
    entropy: 0.7,
    mutation_rate: 0.05,
    relationships: [{ target: 'cpp', strength: 0.9, type: 0 }]
  };
  console.log(JSON.stringify(obj));
  process.exit(0);
}

const file = process.argv[2];
const data = file ? fs.readFileSync(file, 'utf8') : fs.readFileSync(0, 'utf8');
const obj = JSON.parse(data);
console.log(JSON.stringify(obj));
