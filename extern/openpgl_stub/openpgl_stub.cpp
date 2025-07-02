#ifdef __cplusplus
extern "C" {
#endif
void* pglNewDevice() { return nullptr; }
void pglReleaseDevice(void*) {}
void* pglNewPathSegmentStorage() { return nullptr; }
void pglReleasePathSegmentStorage(void*) {}
void pglPathSegmentStorageAddSegment(void*, void*) {}
void pglPathSegmentStorageNextSegment(void*) {}
void* pglNewSurfaceSamplingDistribution() { return nullptr; }
void pglReleaseSurfaceSamplingDistribution(void*) {}
void* pglNewVolumeSamplingDistribution() { return nullptr; }
void pglReleaseVolumeSamplingDistribution(void*) {}
void pglFieldInitSurfaceSamplingDistribution(void*) {}
void pglFieldInitVolumeSamplingDistribution(void*) {}
void pglSurfaceSamplingDistributionApplyCosineProduct(void*) {}
double pglSurfaceSamplingDistributionPDF(void*) { return 0.0; }
double pglSurfaceSamplingDistributionSamplePDF(void*) { return 0.0; }
void pglVolumeSamplingDistributionApplySingleLobeHenyeyGreensteinProduct(void*) {}
double pglVolumeSamplingDistributionPDF(void*) { return 0.0; }
double pglVolumeSamplingDistributionSamplePDF(void*) { return 0.0; }
#ifdef __cplusplus
}
#endif
