/**
 * Content Type Handler System
 * Manages different content types with custom serialization/deserialization logic
 */

class ContentTypeHandler {
    constructor() {
        this.handlers = new Map();
        this.registerDefaultHandlers();
    }

    registerHandler(contentType, handler) {
        if (!handler.serialize || !handler.deserialize || !handler.calculateCoherence) {
            throw new Error(`Invalid handler for content type ${contentType}. Must implement serialize, deserialize, and calculateCoherence methods.`);
        }
        this.handlers.set(contentType, handler);
    }

    getHandler(contentType) {
        return this.handlers.get(contentType) || this.handlers.get('default');
    }

    detectContentType(data) {
        if (typeof data === 'string') {
            // Check for Markdown
            if (data.includes('# ') || data.includes('## ') || data.includes('```')) {
                return 'markdown';
            }
            // Default to text
            return 'text';
        }
        
        if (typeof data === 'object') {
            return 'json';
        }

        return 'default';
    }

    registerDefaultHandlers() {
        // Default handler
        this.registerHandler('default', {
            serialize: (data) => JSON.stringify(data),
            deserialize: (data) => JSON.parse(data),
            calculateCoherence: () => 0.5 // Default coherence
        });

        // Markdown handler
        this.registerHandler('markdown', {
            serialize: (data) => data,
            deserialize: (data) => data,
            calculateCoherence: (data) => {
                // Basic markdown coherence based on structure
                const hasHeaders = data.includes('#');
                const hasCodeBlocks = data.includes('```');
                const hasLists = data.includes('- ') || data.includes('* ');
                
                let coherence = 0.5; // Base coherence
                if (hasHeaders) coherence += 0.1;
                if (hasCodeBlocks) coherence += 0.1;
                if (hasLists) coherence += 0.1;
                
                return Math.min(coherence, 1.0);
            }
        });

        // JSON handler
        this.registerHandler('json', {
            serialize: (data) => JSON.stringify(data),
            deserialize: (data) => JSON.parse(data),
            calculateCoherence: (data) => {
                try {
                    // Coherence based on object complexity and structure
                    const complexity = JSON.stringify(data).length;
                    const hasNesting = JSON.stringify(data).includes('{"');
                    
                    let coherence = 0.5; // Base coherence
                    coherence += Math.min(complexity / 1000, 0.3); // Size factor
                    if (hasNesting) coherence += 0.1; // Structure factor
                    
                    return Math.min(coherence, 1.0);
                } catch (error) {
                    return 0.3; // Lower coherence for invalid JSON
                }
            }
        });

        // Text handler
        this.registerHandler('text', {
            serialize: (data) => data,
            deserialize: (data) => data,
            calculateCoherence: (data) => {
                // Coherence based on text structure and length
                const lines = data.split('\n');
                const avgLineLength = lines.reduce((sum, line) => sum + line.length, 0) / lines.length;
                
                let coherence = 0.5; // Base coherence
                coherence += Math.min(data.length / 5000, 0.2); // Length factor
                coherence += Math.min(avgLineLength / 80, 0.2); // Line length factor
                
                return Math.min(coherence, 1.0);
            }
        });
    }
}

export default ContentTypeHandler;