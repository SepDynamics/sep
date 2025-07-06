namespace ccl {
class Profiler {
public:
    Profiler() = default;
    ~Profiler() = default;
    void reset(int, int) {}
    void start() {}
    void stop() {}
    void add_state(void*) {}
    void remove_state(void*) {}
    unsigned long long get_event(int) { return 0; }
    bool get_shader(int, unsigned long long&, unsigned long long&) { return false; }
    bool get_object(int, unsigned long long&, unsigned long long&) { return false; }
    bool active() const { return false; }
};
}
