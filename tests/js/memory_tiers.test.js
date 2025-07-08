import assert from 'assert';
import { STM, MTM, LTM, HOST, DEVICE, UNIFIED } from '../../memory_tiers.js';

// These constants must match the values defined in include/core/types.h
assert.strictEqual(STM, 0, 'STM should equal 0');
assert.strictEqual(MTM, 1, 'MTM should equal 1');
assert.strictEqual(LTM, 2, 'LTM should equal 2');
assert.strictEqual(HOST, 100, 'HOST should equal 100');
assert.strictEqual(DEVICE, 101, 'DEVICE should equal 101');
assert.strictEqual(UNIFIED, 102, 'UNIFIED should equal 102');

console.log('memory_tiers constants match C++ definitions');
