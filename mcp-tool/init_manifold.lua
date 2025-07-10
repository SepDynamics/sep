-- SEP Manifold Initialization Script
-- This script initializes the required data structures for the SEP manifold system

-- Clear existing manifold keys
redis.call('DEL', 'sep:manifold:config')
redis.call('DEL', 'sep:manifold:stats')
redis.call('DEL', 'sep:manifold:patterns')

-- Initialize configuration
redis.call('HSET', 'sep:manifold:config',
    'version', '1.0.0',
    'created_at', tostring(redis.call('TIME')[1]),
    'pattern_limit', '10000',
    'coherence_min', '0.1',
    'coherence_mtm', '0.7',
    'coherence_ltm', '0.9',
    'coherence_demotion', '0.3'
)

-- Initialize statistics
redis.call('HSET', 'sep:manifold:stats',
    'total_patterns', '0',
    'total_mutations', '0',
    'total_evolutions', '0',
    'last_updated', tostring(redis.call('TIME')[1])
)

-- Create sorted set for pattern tracking
redis.call('ZADD', 'sep:manifold:patterns', 0, 'init')

return 'Manifold structures initialized successfully'