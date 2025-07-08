const fs = require('fs');
const [,, input, output] = process.argv;
const data = JSON.parse(fs.readFileSync(input, 'utf8'));
fs.writeFileSync(output, JSON.stringify(data));
