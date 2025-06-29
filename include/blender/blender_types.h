#pragma once

#include <cstddef>
#include <cstdint>

// Blender type definitions for compilation without full Blender headers
// These match Blender's memory layout but are simplified for our use

#ifdef __cplusplus
extern "C" {
#endif

// Object types
#define OB_EMPTY 0
#define OB_MESH  1
#define OB_CURVE 2
#define OB_SURF  3
#define OB_FONT  4
#define OB_MBALL 5

// Custom data types
#define CD_PROP_FLOAT     1
#define CD_PROP_INT32    2
#define CD_PROP_STRING   3
#define CD_PROP_FLOAT3   4
#define CD_PROP_FLOAT2   5
#define CD_PROP_BYTE     6
#define CD_PROP_QUATERNION 7
#define CD_PROP_FLOAT4X4  8

// Mesh flags
#define ME_AUTOSMOOTH     (1 << 0)
#define ME_REMESH_REPROJECT (1 << 1)
#define ME_DS_EXPAND      (1 << 2)
#define ME_SCULPT_DYNAMIC_TOPOLOGY (1 << 3)

// Depsgraph tags
#define DEG_TAG_TRANSFORM  (1 << 0)
#define DEG_TAG_GEOMETRY   (1 << 1)
#define DEG_TAG_ANIMATION  (1 << 2)
#define DEG_TAG_CACHE      (1 << 3)

typedef struct Object {
    int type;              // Object type (OB_MESH etc)
    void* data;            // Object data (Mesh* etc)
    float loc[3];          // Location
    float rot[3];          // Rotation
    float scale[3];        // Scale
    float obmat[4][4];     // Object matrix
    struct Object* parent;  // Parent object
    int flag;              // Object flags
    char name[64];         // Object name
} Object;

typedef struct MVert {
    float co[3];           // Vertex coordinates
    float no[3];           // Vertex normal
    char flag;             // Selection flags
    char bweight;          // Bevel weight
} MVert;

typedef struct MEdge {
    unsigned int v1, v2;   // Vertex indices
    char crease;           // Crease weight
    char bweight;          // Bevel weight
    short flag;            // Selection flags
} MEdge;

typedef struct MLoop {
    unsigned int v;        // Vertex index
    unsigned int e;        // Edge index
} MLoop;

typedef struct MPoly {
    int loopstart;         // First loop index
    int totloop;           // Number of loops
    short mat_nr;          // Material index
    char flag;             // Selection flags
    char pad;              // Padding
} MPoly;

typedef struct CustomData {
    void* layers;          // Custom data layers
    int totlayer;          // Number of layers
    int maxlayer;          // Maximum layers
    int totsize;          // Size of all layers
} CustomData;

typedef struct Mesh {
    MVert* mvert;          // Vertex array
    MEdge* medge;          // Edge array
    MLoop* mloop;          // Loop array
    MPoly* mpoly;          // Polygon array
    CustomData vdata;      // Vertex custom data
    CustomData edata;      // Edge custom data
    CustomData ldata;      // Loop custom data
    CustomData pdata;      // Polygon custom data
    int totvert;           // Total vertices
    int totedge;           // Total edges
    int totloop;           // Total loops
    int totpoly;           // Total polygons
    int attributes_active_index; // Active attribute
    int runtime_flag;      // Runtime flags
    void* runtime_data;    // Runtime data
} Mesh;

// Function declarations matching Blender's API
Mesh* BKE_mesh_from_object(Object* ob);
void BKE_mesh_calc_normals(Mesh* mesh);
void BKE_mesh_batch_cache_dirty_tag(Mesh* mesh, int mode);
void BKE_object_deform_verts(Object* ob, Mesh* mesh);

// Depsgraph functions
void DEG_id_tag_update(void* id, int flag);

// RNA access
void* RNA_pointer_get(void* ptr, const char* name);

#ifdef __cplusplus
}
#endif
