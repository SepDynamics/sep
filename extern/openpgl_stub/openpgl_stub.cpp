#include <vector>
#include <cstddef>

extern "C" {

struct PGLDevice {};

struct PathSegmentStorage {
    std::vector<void*> segments;
    size_t index = 0;
};

struct SurfaceSamplingDistribution {
    double weight = 1.0;
};

struct VolumeSamplingDistribution {
    double weight = 1.0;
};

void* pglNewDevice() { return new PGLDevice(); }
void pglReleaseDevice(void* d) { delete static_cast<PGLDevice*>(d); }

void* pglNewPathSegmentStorage() { return new PathSegmentStorage(); }
void pglReleasePathSegmentStorage(void* p) { delete static_cast<PathSegmentStorage*>(p); }
void pglPathSegmentStorageAddSegment(void* storage, void* segment) {
    auto* s = static_cast<PathSegmentStorage*>(storage);
    if (s) s->segments.push_back(segment);
}
void pglPathSegmentStorageNextSegment(void* storage) {
    auto* s = static_cast<PathSegmentStorage*>(storage);
    if (s && s->index < s->segments.size()) ++s->index;
}

void* pglNewSurfaceSamplingDistribution() { return new SurfaceSamplingDistribution(); }
void pglReleaseSurfaceSamplingDistribution(void* dist) { delete static_cast<SurfaceSamplingDistribution*>(dist); }
void* pglNewVolumeSamplingDistribution() { return new VolumeSamplingDistribution(); }
void pglReleaseVolumeSamplingDistribution(void* dist) { delete static_cast<VolumeSamplingDistribution*>(dist); }

void pglFieldInitSurfaceSamplingDistribution(void* dist) {
    if (auto* d = static_cast<SurfaceSamplingDistribution*>(dist)) d->weight = 1.0;
}
void pglFieldInitVolumeSamplingDistribution(void* dist) {
    if (auto* d = static_cast<VolumeSamplingDistribution*>(dist)) d->weight = 1.0;
}

void pglSurfaceSamplingDistributionApplyCosineProduct(void* dist) {
    if (auto* d = static_cast<SurfaceSamplingDistribution*>(dist)) d->weight *= 0.5;
}
double pglSurfaceSamplingDistributionPDF(void* dist) {
    auto* d = static_cast<SurfaceSamplingDistribution*>(dist);
    return d ? d->weight : 0.0;
}
double pglSurfaceSamplingDistributionSamplePDF(void* dist) {
    auto* d = static_cast<SurfaceSamplingDistribution*>(dist);
    return d ? d->weight : 0.0;
}
void pglVolumeSamplingDistributionApplySingleLobeHenyeyGreensteinProduct(void* dist) {
    if (auto* d = static_cast<VolumeSamplingDistribution*>(dist)) d->weight *= 0.5;
}
double pglVolumeSamplingDistributionPDF(void* dist) {
    auto* d = static_cast<VolumeSamplingDistribution*>(dist);
    return d ? d->weight : 0.0;
}
double pglVolumeSamplingDistributionSamplePDF(void* dist) {
    auto* d = static_cast<VolumeSamplingDistribution*>(dist);
    return d ? d->weight : 0.0;
}

} // extern "C"
