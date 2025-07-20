# MCP Tool Documentation

This document provides a reference for the available tools provided by the connected MCP servers.

## Filesystem Server (`filesystem`)

The `filesystem` server provides tools for interacting with the local filesystem.

### Tools

#### `read_file`
- **Description**: Read the complete contents of a file from the file system. Handles various text encodings and provides detailed error messages if the file cannot be read. Use this tool when you need to examine the contents of a single file. Use the 'head' parameter to read only the first N lines of a file, or the 'tail' parameter to read only the last N lines of a file. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path to the file to read.
    - `tail` (optional): If provided, returns only the last N lines of the file.
    - `head` (optional): If provided, returns only the first N lines of the file.

#### `read_multiple_files`
- **Description**: Read the contents of multiple files simultaneously. This is more efficient than reading files one by one when you need to analyze or compare multiple files. Each file's content is returned with its path as a reference. Failed reads for individual files won't stop the entire operation. Only works within allowed directories.
- **Parameters**:
    - `paths` (*required*): An array of file paths to read.

#### `write_file`
- **Description**: Create a new file or completely overwrite an existing file with new content. Use with caution as it will overwrite existing files without warning. Handles text content with proper encoding. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path of the file to write to.
    - `content` (*required*): The content to write to the file.

#### `edit_file`
- **Description**: Make line-based edits to a text file. Each edit replaces exact line sequences with new content. Returns a git-style diff showing the changes made. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path of the file to edit.
    - `edits` (*required*): A series of edits to apply.
    - `dryRun` (optional): Preview changes using git-style diff format.

#### `create_directory`
- **Description**: Create a new directory or ensure a directory exists. Can create multiple nested directories in one operation. If the directory already exists, this operation will succeed silently. Perfect for setting up directory structures for projects or ensuring required paths exist. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path of the directory to create.

#### `list_directory`
- **Description**: Get a detailed listing of all files and directories in a specified path. Results clearly distinguish between files and directories with [FILE] and [DIR] prefixes. This tool is essential for understanding directory structure and finding specific files within a directory. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path of the directory to list.

#### `list_directory_with_sizes`
- **Description**: Get a detailed listing of all files and directories in a specified path, including sizes. Results clearly distinguish between files and directories with [FILE] and [DIR] prefixes. This tool is useful for understanding directory structure and finding specific files within a directory. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path of the directory to list.
    - `sortBy` (optional): Sort entries by `name` or `size`.

#### `directory_tree`
- **Description**: Get a recursive tree view of files and directories as a JSON structure. Each entry includes 'name', 'type' (file/directory), and 'children' for directories. Files have no children array, while directories always have a children array (which may be empty). The output is formatted with 2-space indentation for readability. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The root path to generate the tree from.

#### `move_file`
- **Description**: Move or rename files and directories. Can move files between directories and rename them in a single operation. If the destination exists, the operation will fail. Works across different directories and can be used for simple renaming within the same directory. Both source and destination must be within allowed directories.
- **Parameters**:
    - `source` (*required*): The source path of the file or directory to move.
    - `destination` (*required*): The destination path.

#### `search_files`
- **Description**: Recursively search for files and directories matching a pattern. Searches through all subdirectories from the starting path. The search is case-insensitive and matches partial names. Returns full paths to all matching items. Great for finding files when you don't know their exact location. Only searches within allowed directories.
- **Parameters**:
    - `path` (*required*): The path to start the search from.
    - `pattern` (*required*): The search pattern.
    - `excludePatterns` (optional): Patterns to exclude from the search.

#### `get_file_info`
- **Description**: Retrieve detailed metadata about a file or directory. Returns comprehensive information including size, creation time, last modified time, permissions, and type. This tool is perfect for understanding file characteristics without reading the actual content. Only works within allowed directories.
- **Parameters**:
    - `path` (*required*): The path to the file or directory.

#### `list_allowed_directories`
- **Description**: Returns the list of directories that this server is allowed to access. Use this to understand which directories are available before trying to access files.
- **Parameters**: None.

---

## Sequential Thinking Server (`sequentialthinking`)

A detailed tool for dynamic and reflective problem-solving through thoughts. This tool helps analyze problems through a flexible thinking process that can adapt and evolve.

### Tools

#### `sequentialthinking`
- **Description**: Each thought can build on, question, or revise previous insights as understanding deepens. Use this for breaking down complex problems, planning, and analysis that might require course correction.
- **Parameters**:
    - `thought` (*required*): Your current thinking step.
    - `nextThoughtNeeded` (*required*): Whether another thought step is needed.
    - `thoughtNumber` (*required*): Current thought number.
    - `totalThoughts` (*required*): Estimated total thoughts needed.
    - `isRevision` (optional): Whether this revises previous thinking.
    - `revisesThought` (optional): Which thought is being reconsidered.
    - `branchFromThought` (optional): Branching point thought number.
    - `branchId` (optional): Branch identifier.
    - `needsMoreThoughts` (optional): If more thoughts are needed.

---

## Memory Server (`memory`)

This server provides tools to interact with a knowledge graph.

### Tools

#### `create_entities`
- **Description**: Create multiple new entities in the knowledge graph.
- **Parameters**:
    - `entities` (*required*): An array of entities to create.

#### `create_relations`
- **Description**: Create multiple new relations between entities in the knowledge graph. Relations should be in active voice.
- **Parameters**:
    - `relations` (*required*): An array of relations to create.

#### `add_observations`
- **Description**: Add new observations to existing entities in the knowledge graph.
- **Parameters**:
    - `observations` (*required*): An array of observations to add.

#### `delete_entities`
- **Description**: Delete multiple entities and their associated relations from the knowledge graph.
- **Parameters**:
    - `entityNames` (*required*): An array of entity names to delete.

#### `delete_observations`
- **Description**: Delete specific observations from entities in the knowledge graph.
- **Parameters**:
    - `deletions` (*required*): An array of observations to delete.

#### `delete_relations`
- **Description**: Delete multiple relations from the knowledge graph.
- **Parameters**:
    - `relations` (*required*): An array of relations to delete.

#### `read_graph`
- **Description**: Read the entire knowledge graph.
- **Parameters**: None.

#### `search_nodes`
- **Description**: Search for nodes in the knowledge graph based on a query.
- **Parameters**:
    - `query` (*required*): The search query to match against entity names, types, and observation content.

#### `open_nodes`
- **Description**: Open specific nodes in the knowledge graph by their names.
- **Parameters**:
    - `names` (*required*): An array of entity names to retrieve.

---

## Context7 Server (`context7`)

This server provides tools for retrieving documentation and code examples for any library.

### Tools

#### `resolve-library-id`
- **Description**: Resolves a package/product name to a Context7-compatible library ID and returns a list of matching libraries. You MUST call this function before 'get-library-docs' to obtain a valid Context7-compatible library ID.
- **Parameters**:
    - `libraryName` (*required*): Library name to search for and retrieve a Context7-compatible library ID.

#### `get-library-docs`
- **Description**: Fetches up-to-date documentation for a library. You must call 'resolve-library-id' first to obtain the exact Context7-compatible library ID required to use this tool.
- **Parameters**:
    - `context7CompatibleLibraryID` (*required*): Exact Context7-compatible library ID (e.g., '/mongodb/docs').
    - `topic` (optional): Topic to focus documentation on (e.g., 'hooks', 'routing').
    - `tokens` (optional): Maximum number of tokens of documentation to retrieve (default: 10000).