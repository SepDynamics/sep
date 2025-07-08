const fs = require('fs');

function testRoundtrip(jsonPath) {
  const data = JSON.parse(fs.readFileSync(jsonPath, 'utf8'));
  if (typeof data.stm_size !== 'number') throw new Error('stm_size missing');
  const out = JSON.stringify(data);
  const parsed = JSON.parse(out);
  return parsed.stm_size === data.stm_size;
}

if (require.main === module) {
  const ok = testRoundtrip(process.argv[2]);
  console.log(ok ? 'roundtrip ok' : 'roundtrip failed');
  process.exit(ok ? 0 : 1);
}
