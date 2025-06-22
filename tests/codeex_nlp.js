export async function extractPatterns(text, opts = {}) {
    const endpoint = opts.endpoint || process.env.CODEEX_URL || 'http://localhost:8080';
    const apiKey = opts.apiKey || process.env.CODEEX_API_KEY;

    const res = await fetch(`${endpoint}/v1/patterns`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            ...(apiKey ? { 'Authorization': `Bearer ${apiKey}` } : {})
        },
        body: JSON.stringify({ text })
    });

    if (!res.ok) {
        throw new Error(`Codeex service error: ${res.status} ${res.statusText}`);
    }

    const data = await res.json();
    return data.patterns || [];
}
