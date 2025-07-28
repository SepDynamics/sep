#pragma once

namespace sep {
namespace demo {

// Demo panel base class
class DemoPanel {
public:
    virtual ~DemoPanel() = default;
    virtual void render() = 0;
    virtual const char* getName() const = 0;
    bool isVisible() const { return visible_; }
    void setVisible(bool visible) { visible_ = visible; }

protected:
    bool visible_ = true;
};

}
}
