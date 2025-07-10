/**
 * SEP Visualization Updater
 * 
 * This module updates Mermaid diagrams in the SEP system architecture document
 * to reflect the current status of system components during the bring-up process.
 */

const fs = require('fs');
const path = require('path');
const sepBringUp = require('./sep_bring_up_sequence');

// Paths
const ARCHITECTURE_DOC_PATH = path.join(__dirname, 'docs', 'sep_system_architecture.md');

// Status color mapping
const STATUS_COLORS = {
  pending: '#bbbbbb',      // Gray
  initializing: '#ffcc00',  // Yellow
  ready: '#00cc66',        // Green
  error: '#ff3333',        // Red
  degraded: '#ff9900'      // Orange
};

// Status icons
const STATUS_ICONS = {
  pending: '⚪',
  initializing: '🔄',
  ready: '✅',
  error: '❌',
  degraded: '⚠️'
};

/**
 * Read the architecture document
 * @returns {string} Document content
 */
function readArchitectureDoc() {
  try {
    return fs.readFileSync(ARCHITECTURE_DOC_PATH, 'utf8');
  } catch (error) {
    console.error(`Error reading architecture document: ${error.message}`);
    return null;
  }
}

/**
 * Write updated content to the architecture document
 * @param {string} content - Updated document content
 * @returns {boolean} Success flag
 */
function writeArchitectureDoc(content) {
  try {
    fs.writeFileSync(ARCHITECTURE_DOC_PATH, content, 'utf8');
    return true;
  } catch (error) {
    console.error(`Error writing architecture document: ${error.message}`);
    return false;
  }
}

/**
 * Extract Mermaid diagrams from the document
 * @param {string} docContent - Document content
 * @returns {Array} Array of objects with diagram info
 */
function extractMermaidDiagrams(docContent) {
  const diagrams = [];
  const diagramRegex = /```mermaid\n([\s\S]*?)```/g;
  
  let match;
  let startIndex = 0;
  
  while ((match = diagramRegex.exec(docContent)) !== null) {
    diagrams.push({
      fullMatch: match[0],
      content: match[1],
      startIndex: match.index,
      endIndex: match.index + match[0].length
    });
    startIndex = diagramRegex.lastIndex;
  }
  
  return diagrams;
}

/**
 * Find component node definitions in a Mermaid diagram
 * @param {string} diagramContent - Mermaid diagram content
 * @returns {Object} Map of component IDs to node definitions
 */
function findComponentNodes(diagramContent) {
  const componentMap = {};
  const lines = diagramContent.split('\n');
  
  // Map of component names to IDs from sep_bring_up_sequence
  const componentNameToId = {};
  sepBringUp.components.forEach(comp => {
    // Create mappings for both full name and ID
    componentNameToId[comp.name] = comp.id;
    componentNameToId[comp.id] = comp.id;
    
    // Also map abbreviations used in diagrams
    if (comp.name === 'Quantum Processing Core') componentNameToId['QPC'] = comp.id;
    if (comp.name === 'Memory Tier Architecture') componentNameToId['MTA'] = comp.id;
    if (comp.name === 'Pattern Evolution Engine') componentNameToId['PEE'] = comp.id;
    if (comp.name === 'Context Processing System') componentNameToId['CPS'] = comp.id;
    if (comp.name === 'Resource Prediction') componentNameToId['RP'] = comp.id;
    if (comp.name === 'CUDA Integration') componentNameToId['CUDA'] = comp.id;
    if (comp.name === 'Blender Integration') componentNameToId['Blender'] = comp.id;
    if (comp.name === 'Audio Integration') componentNameToId['Audio'] = comp.id;
    if (comp.name === 'API Integration') componentNameToId['API'] = comp.id;
  });
  
  // Regular expression to find node definitions
  // Matches patterns like: QPC["Quantum Processing Core"]
  const nodeRegex = /\s*(\w+)\s*\["([^"]+)"\]/;
  
  lines.forEach(line => {
    const match = line.match(nodeRegex);
    if (match) {
      const nodeId = match[1];
      const nodeName = match[2];
      
      // Check if this node corresponds to a component
      const componentId = componentNameToId[nodeName] || componentNameToId[nodeId];
      if (componentId) {
        componentMap[componentId] = {
          nodeId,
          nodeName,
          line
        };
      }
    }
  });
  
  return componentMap;
}

/**
 * Update node styling in a Mermaid diagram based on component status
 * @param {string} diagramContent - Mermaid diagram content
 * @param {Object} componentNodes - Map of component IDs to node definitions
 * @returns {string} Updated diagram content
 */
function updateNodeStyling(diagramContent, componentNodes) {
  let updatedContent = diagramContent;
  const lines = diagramContent.split('\n');
  const updatedLines = [...lines];
  
  // Find class definitions section
  let classDefIndex = -1;
  for (let i = 0; i < lines.length; i++) {
    if (lines[i].trim().startsWith('classDef ')) {
      classDefIndex = i;
      break;
    }
  }
  
  // Add status class definitions if not present
  if (classDefIndex >= 0) {
    // Check if status classes already exist
    let hasStatusClasses = false;
    for (let i = classDefIndex; i < lines.length; i++) {
      if (lines[i].includes('status_')) {
        hasStatusClasses = true;
        break;
      }
    }
    
    // Add status class definitions if not present
    if (!hasStatusClasses) {
      const statusClassDefs = [
        `classDef status_pending fill:${STATUS_COLORS.pending},stroke:#333,stroke-width:1px`,
        `classDef status_initializing fill:${STATUS_COLORS.initializing},stroke:#333,stroke-width:1px`,
        `classDef status_ready fill:${STATUS_COLORS.ready},stroke:#333,stroke-width:1px`,
        `classDef status_error fill:${STATUS_COLORS.error},stroke:#333,stroke-width:1px`,
        `classDef status_degraded fill:${STATUS_COLORS.degraded},stroke:#333,stroke-width:1px`
      ];
      
      // Insert status class definitions after existing class definitions
      updatedLines.splice(classDefIndex + 1, 0, ...statusClassDefs);
    }
  }
  
  // Find class assignments section
  let classAssignIndex = -1;
  for (let i = 0; i < updatedLines.length; i++) {
    if (updatedLines[i].trim().startsWith('class ')) {
      classAssignIndex = i;
    }
  }
  
  // Add status class assignments
  if (classAssignIndex >= 0) {
    // Remove existing status class assignments
    for (let i = classAssignIndex; i < updatedLines.length; i++) {
      if (updatedLines[i].includes('status_')) {
        updatedLines.splice(i, 1);
        i--;
      }
    }
    
    // Add new status class assignments
    const statusAssignments = [];
    
    // Group components by status
    const componentsByStatus = {
      pending: [],
      initializing: [],
      ready: [],
      error: [],
      degraded: []
    };
    
    // Collect node IDs by status
    Object.entries(componentNodes).forEach(([componentId, nodeInfo]) => {
      const component = sepBringUp.components.find(c => c.id === componentId);
      if (component) {
        const status = component.status.toLowerCase();
        componentsByStatus[status].push(nodeInfo.nodeId);
      }
    });
    
    // Create class assignments for each status
    Object.entries(componentsByStatus).forEach(([status, nodeIds]) => {
      if (nodeIds.length > 0) {
        statusAssignments.push(`class ${nodeIds.join(',')} status_${status}`);
      }
    });
    
    // Add status class assignments
    if (statusAssignments.length > 0) {
      updatedLines.splice(classAssignIndex + 1, 0, ...statusAssignments);
    }
  }
  
  // Update node labels with status information
  Object.entries(componentNodes).forEach(([componentId, nodeInfo]) => {
    const component = sepBringUp.components.find(c => c.id === componentId);
    if (component) {
      const status = component.status.toLowerCase();
      const statusIcon = STATUS_ICONS[status];
      
      // Find the line with this node definition
      for (let i = 0; i < updatedLines.length; i++) {
        if (updatedLines[i].includes(`${nodeInfo.nodeId}["`) && !updatedLines[i].includes(`${statusIcon}`)) {
          // Update node label to include status
          const updatedLine = updatedLines[i].replace(
            `${nodeInfo.nodeId}["${nodeInfo.nodeName}"]`,
            `${nodeInfo.nodeId}["${nodeInfo.nodeName}<br/>${statusIcon} ${status.toUpperCase()}"]`
          );
          updatedLines[i] = updatedLine;
          break;
        }
      }
    }
  });
  
  return updatedLines.join('\n');
}

/**
 * Update a single Mermaid diagram with component status information
 * @param {Object} diagram - Diagram information
 * @returns {Object} Updated diagram
 */
function updateDiagram(diagram) {
  // Find component nodes in the diagram
  const componentNodes = findComponentNodes(diagram.content);
  
  // Update node styling based on component status
  const updatedContent = updateNodeStyling(diagram.content, componentNodes);
  
  // Return updated diagram
  return {
    ...diagram,
    content: updatedContent,
    updatedFullMatch: '```mermaid\n' + updatedContent + '```'
  };
}

/**
 * Update all Mermaid diagrams in the architecture document
 * @returns {boolean} Success flag
 */
function updateAllDiagrams() {
  // Read the architecture document
  const docContent = readArchitectureDoc();
  if (!docContent) return false;
  
  // Extract Mermaid diagrams
  const diagrams = extractMermaidDiagrams(docContent);
  if (diagrams.length === 0) {
    console.error('No Mermaid diagrams found in the architecture document');
    return false;
  }
  
  // Update each diagram
  let updatedContent = docContent;
  let offset = 0;
  
  diagrams.forEach(diagram => {
    const updatedDiagram = updateDiagram(diagram);
    
    // Replace the original diagram with the updated one
    updatedContent = updatedContent.substring(0, diagram.startIndex + offset) +
                    updatedDiagram.updatedFullMatch +
                    updatedContent.substring(diagram.endIndex + offset);
    
    // Update offset for subsequent replacements
    offset += updatedDiagram.updatedFullMatch.length - diagram.fullMatch.length;
  });
  
  // Write the updated content back to the document
  return writeArchitectureDoc(updatedContent);
}

/**
 * Update the visualization for a specific component
 * @param {string} componentId - ID of the component to update
 * @returns {boolean} Success flag
 */
function updateComponentVisualization(componentId) {
  // Validate component ID
  const component = sepBringUp.components.find(c => c.id === componentId);
  if (!component) {
    console.error(`Component with ID '${componentId}' not found`);
    return false;
  }
  
  // Update all diagrams (for simplicity, we update all diagrams even for a single component)
  return updateAllDiagrams();
}

/**
 * Get a progress summary for visualization
 * @returns {Object} Progress summary
 */
function getVisualizationProgress() {
  const progress = sepBringUp.getInitializationProgress();
  
  // Add color information
  return {
    ...progress,
    colors: {
      ready: STATUS_COLORS.ready,
      error: STATUS_COLORS.error,
      degraded: STATUS_COLORS.degraded,
      initializing: STATUS_COLORS.initializing,
      pending: STATUS_COLORS.pending
    },
    icons: STATUS_ICONS
  };
}

// Export the module
module.exports = {
  updateAllDiagrams,
  updateComponentVisualization,
  getVisualizationProgress,
  STATUS_COLORS,
  STATUS_ICONS
};