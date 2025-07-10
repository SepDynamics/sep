/**
 * SEP (Semantic Evolution Protocol) System Bring-Up Sequence
 * 
 * This module defines the initialization sequence for the SEP system,
 * including component dependencies, configuration parameters, and status tracking.
 */

// Component status constants
const STATUS = {
  PENDING: 'pending',
  INITIALIZING: 'initializing',
  READY: 'ready',
  ERROR: 'error',
  DEGRADED: 'degraded'
};

/**
 * SEP System Components in proper initialization order (low-level to high-level)
 * Each component includes:
 * - id: Unique identifier
 * - name: Display name
 * - description: Brief description
 * - dependencies: Array of component IDs that must be initialized first
 * - config: Configuration parameters with default values
 * - status: Current initialization status
 * - statusDetails: Additional status information
 * - importance: Importance score (95-75 scale from architecture)
 * - location: Source file location
 */
const sepComponents = [
  // 1. CUDA Integration (lowest level - GPU acceleration)
  {
    id: 'cuda_integration',
    name: 'CUDA Integration',
    description: 'GPU acceleration for pattern processing',
    dependencies: [],
    config: {
      enableParallelProcessing: true,
      maxBatchSize: 1024,
      preferredDevice: 0
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 85,
    location: 'src/blender/shaders/pattern_process.comp'
  },
  
  // 2. Memory Tier Architecture (low level - data storage)
  {
    id: 'memory_tier_architecture',
    name: 'Memory Tier Architecture',
    description: 'Three-tier memory system for pattern persistence',
    dependencies: ['cuda_integration'],
    config: {
      stm: {
        enableCompression: false,
        autoCleanupThreshold: 0.1, // Minimum coherence to keep patterns
        maxPoolSize: '2GB'
      },
      mtm: {
        enableCompression: false,
        promotionThreshold: 0.7, // Coherence threshold for STM → MTM
        demotionThreshold: 0.3, // Coherence threshold for MTM → STM
        maxPoolSize: '8GB'
      },
      ltm: {
        enableCompression: true,
        promotionThreshold: 0.9, // Coherence threshold for MTM → LTM
        demotionThreshold: 0.3, // Coherence threshold for LTM → MTM
        minGenerations: 100, // Minimum generations for promotion
        maxPoolSize: '32GB'
      },
      defragmentationThreshold: 0.3, // 30% fragmentation triggers defrag
      poolUtilizationThreshold: 0.9 // 90% utilization triggers resize
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 90,
    location: 'src/memory/memory_tier_manager.cpp'
  },
  
  // 3. Quantum Processing Core (low level - core processing)
  {
    id: 'quantum_processing_core',
    name: 'Quantum Processing Core',
    description: 'Core quantum-inspired processing system',
    dependencies: ['cuda_integration'],
    config: {
      qbsa: {
        stateCorrectionsEnabled: true,
        stateCollapseDetection: true,
        coherenceThreshold: 0.1 // Minimum coherence for valid states
      },
      qsh: {
        timeChunkSize: 100, // Number of states per time chunk
        maxHistoryLength: 1000 // Maximum history length to maintain
      },
      spm: {
        manifoldDimensions: 128, // Dimensions in computational manifold
        positionPrecision: 0.001 // Precision for position mapping
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 95,
    location: 'src/quantum/processor.cpp'
  },
  
  // 4. Pattern Evolution Engine (mid level - depends on QPC and MTA)
  {
    id: 'pattern_evolution_engine',
    name: 'Pattern Evolution Engine',
    description: 'Manages pattern stability and evolution',
    dependencies: ['quantum_processing_core', 'memory_tier_architecture'],
    config: {
      stabilityCalculation: {
        minCoherence: 0.1, // Minimum coherence for valid patterns
        highStabilityThreshold: 0.8, // Threshold for high stability patterns
        mutationRecalculationEnabled: true
      },
      evolutionTracking: {
        minGenerations: 5, // Minimum generations to track evolution
        relationshipStrengthThreshold: 0.6 // Minimum strength for relationships
      },
      stateTransitions: {
        booleanCollapseEnabled: true,
        weightWaitDualityEnabled: true
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 85,
    location: 'src/blender/pattern_processor.cpp'
  },
  
  // 5. Resource Prediction (mid level - optimization)
  {
    id: 'resource_prediction',
    name: 'Resource Prediction',
    description: 'Predicts resource usage and optimizes allocation',
    dependencies: ['cuda_integration'],
    config: {
      usagePatterns: {
        historyLength: 100, // Number of usage samples to keep
        updateInterval: 60 // Seconds between updates
      },
      anomalyDetection: {
        sensitivityThreshold: 2.0, // Standard deviations for anomaly
        adjustmentFactor: 0.8 // Factor to adjust resources on anomaly
      },
      batchOptimization: {
        minBatchSize: 16,
        maxBatchSize: 1024,
        adaptiveScaling: true
      },
      confidenceScoring: {
        coefficientVariationThreshold: 0.2 // Maximum CV for high confidence
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 75,
    location: 'src/context/resource_predictor.cpp'
  },
  
  // 6. Context Processing System (high level - depends on multiple components)
  {
    id: 'context_processing_system',
    name: 'Context Processing System',
    description: 'Processes and manages context information',
    dependencies: ['pattern_evolution_engine', 'memory_tier_architecture', 'resource_prediction'],
    config: {
      validationEngine: {
        multiLayerVerification: true,
        relevanceScoring: {
          weightedEmbeddingsSimilarity: true,
          baseRelevanceFormula: 'dot(embedding1, embedding2)'
        },
        timeDecay: {
          enabled: true,
          decayFormula: 'exp(-hours_since_access * 0.05)'
        },
        priorityMultipliers: {
          min: 1,
          max: 5
        }
      },
      relationshipManagement: {
        bidirectionalTracking: true,
        strengthScoring: {
          useNormalizedDotProducts: true
        },
        pruningThreshold: 0.3, // Prune relationships below this strength
        hierarchicalOrganization: true
      },
      prioritySystem: {
        tiers: ['LOW', 'NORMAL', 'HIGH', 'CRITICAL'],
        adjustments: {
          timeDecayFactor: 0.95, // Per hour
          usageMultiplier: 1.1, // Per access
          relationshipWeighting: true,
          parentInfluence: true
        }
      },
      storageRules: {
        minRelevanceThreshold: 0.4, // Minimum relevance to store
        maxRelationshipCount: 1000, // Per context
        recalculationInterval: 60, // Minutes
        expirationDays: 30 // Days of inactivity before expiration
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 80,
    location: 'src/context/processor.cpp'
  },
  
  // 7. Audio Integration (high level - specialized input)
  {
    id: 'audio_integration',
    name: 'Audio Integration',
    description: 'Processes audio input for pattern extraction',
    dependencies: ['pattern_evolution_engine', 'quantum_processing_core'],
    config: {
      realTimeProcessing: {
        enabled: true,
        bufferSize: 1024,
        sampleRate: 44100
      },
      patternExtraction: {
        sensitivityThreshold: 0.2,
        minPatternLength: 0.5, // Seconds
        coherenceMappingEnabled: true
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 70,
    location: 'src/audio/pipewire_capture.cpp'
  },
  
  // 8. Blender Integration (high level - visualization)
  {
    id: 'blender_integration',
    name: 'Blender Integration',
    description: '3D visualization of computational manifolds',
    dependencies: ['pattern_evolution_engine'],
    config: {
      visualization: {
        enabled: true,
        refreshRate: 30, // FPS
        maxVisiblePatterns: 1000
      },
      manifoldMapping: {
        dimensions: 3, // Visual dimensions
        colorMapping: 'coherence', // Map colors to coherence values
        sizeMapping: 'stability' // Map size to stability values
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 65,
    location: 'src/blender/memory_tier.cpp'
  },
  
  // 9. API Integration (highest level - external interface)
  {
    id: 'api_integration',
    name: 'API Integration',
    description: 'External API for system interaction',
    dependencies: ['context_processing_system', 'pattern_evolution_engine'],
    config: {
      rest: {
        enabled: true,
        port: 8080,
        maxConcurrentRequests: 100
      },
      jsBridge: {
        enabled: true,
        webSocketEnabled: true,
        webSocketPort: 8081
      }
    },
    status: STATUS.PENDING,
    statusDetails: '',
    importance: 60,
    location: 'src/api/endpoints.cpp'
  }
];

/**
 * Get the current status of all components
 * @returns {Object} Status object with component IDs as keys
 */
function getComponentStatus() {
  const statusObj = {};
  
  sepComponents.forEach(component => {
    statusObj[component.id] = {
      status: component.status,
      details: component.statusDetails,
      dependencies: component.dependencies.map(depId => {
        const dep = sepComponents.find(c => c.id === depId);
        return {
          id: depId,
          status: dep ? dep.status : 'unknown'
        };
      })
    };
  });
  
  return statusObj;
}

/**
 * Update the status of a component
 * @param {string} componentId - ID of the component to update
 * @param {string} status - New status (from STATUS constants)
 * @param {string} details - Optional status details
 * @returns {boolean} Success flag
 */
function updateComponentStatus(componentId, status, details = '') {
  const component = sepComponents.find(c => c.id === componentId);
  if (!component) return false;
  
  component.status = status;
  component.statusDetails = details;
  
  // Check if this affects dependent components
  if (status === STATUS.ERROR) {
    markDependentsAsDegraded(componentId);
  }
  
  return true;
}

/**
 * Mark all components that depend on the given component as degraded
 * @param {string} componentId - ID of the dependency that has an error
 */
function markDependentsAsDegraded(componentId) {
  sepComponents.forEach(component => {
    if (component.dependencies.includes(componentId) && 
        component.status !== STATUS.ERROR) {
      component.status = STATUS.DEGRADED;
      component.statusDetails = `Dependency '${componentId}' has an error`;
      
      // Recursively mark components that depend on this one
      markDependentsAsDegraded(component.id);
    }
  });
}

/**
 * Check if a component can be initialized (all dependencies are ready)
 * @param {string} componentId - ID of the component to check
 * @returns {boolean} True if component can be initialized
 */
function canInitializeComponent(componentId) {
  const component = sepComponents.find(c => c.id === componentId);
  if (!component) return false;
  
  // Check all dependencies
  for (const depId of component.dependencies) {
    const dep = sepComponents.find(c => c.id === depId);
    if (!dep || dep.status !== STATUS.READY) {
      return false;
    }
  }
  
  return true;
}

/**
 * Get the next component that should be initialized
 * @returns {Object|null} Next component to initialize or null if none
 */
function getNextComponentToInitialize() {
  for (const component of sepComponents) {
    if (component.status === STATUS.PENDING && canInitializeComponent(component.id)) {
      return component;
    }
  }
  return null;
}

/**
 * Get initialization progress
 * @returns {Object} Progress information
 */
function getInitializationProgress() {
  const total = sepComponents.length;
  const ready = sepComponents.filter(c => c.status === STATUS.READY).length;
  const error = sepComponents.filter(c => c.status === STATUS.ERROR).length;
  const degraded = sepComponents.filter(c => c.status === STATUS.DEGRADED).length;
  const initializing = sepComponents.filter(c => c.status === STATUS.INITIALIZING).length;
  const pending = sepComponents.filter(c => c.status === STATUS.PENDING).length;
  
  return {
    total,
    ready,
    error,
    degraded,
    initializing,
    pending,
    percentComplete: Math.round((ready / total) * 100)
  };
}

// Export the module
module.exports = {
  components: sepComponents,
  status: STATUS,
  getComponentStatus,
  updateComponentStatus,
  canInitializeComponent,
  getNextComponentToInitialize,
  getInitializationProgress
};