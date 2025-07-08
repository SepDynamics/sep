const fs = require('fs');
const [inPath, outPath] = process.argv.slice(2);
const data = JSON.parse(fs.readFileSync(inPath, 'utf8'));
data.stm_size = data.stm_size * 2;
fs.writeFileSync(outPath, JSON.stringify(data));
