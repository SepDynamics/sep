#include "digital_physics_demo.hpp"
#include <config.hpp>
#include <random>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

void DigitalPhysicsDemo::parseRule(const std::string& rule) {
    birth_rules_.clear();
    survive_rules_.clear();

    auto b_pos = rule.find('B');
    auto s_pos = rule.find('S');
    if (b_pos == std::string::npos || s_pos == std::string::npos) {
        // default to B3/S23
        birth_rules_ = {3};
        survive_rules_ = {2,3};
        return;
    }
    auto parseNums = [&](std::size_t start, std::size_t end, std::vector<int>& out){
        for (std::size_t i = start; i < end; ++i) {
            if (std::isdigit(rule[i])) out.push_back(rule[i]-'0');
        }
    };
    if (b_pos < s_pos) {
        parseNums(b_pos+1, s_pos, birth_rules_);
        parseNums(s_pos+1, rule.size(), survive_rules_);
    } else {
        parseNums(s_pos+1, b_pos, survive_rules_);
        parseNums(b_pos+1, rule.size(), birth_rules_);
    }
    if (birth_rules_.empty()) birth_rules_.push_back(3);
    if (survive_rules_.empty()) { survive_rules_.push_back(2); survive_rules_.push_back(3); }
}

void DigitalPhysicsDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    width_ = 50;
    height_ = 50;
    rule_string_ = "B3/S23";
#else
    const auto& cfg = getConfigManager().getEngineConfig().digital_physics();
    width_ = cfg.grid.width;
    height_ = cfg.grid.height;
    rule_string_ = cfg.rules.rule;
#endif

    parseRule(rule_string_);
    grid_.resize(width_ * height_);
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0,1);
    for (auto& cell : grid_) {
        cell.alive = dist(rng) == 1;
    }
}

int DigitalPhysicsDemo::neighborCount(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <=1; ++dy) {
        for (int dx = -1; dx <=1; ++dx) {
            if (dx==0 && dy==0) continue;
            int nx = (x + dx + width_) % width_;
            int ny = (y + dy + height_) % height_;
            if (grid_[index(nx,ny)].alive) ++count;
        }
    }
    return count;
}

void DigitalPhysicsDemo::step() {
    std::vector<Cell> next = grid_;
    for (int y=0; y<height_; ++y) {
        for (int x=0; x<width_; ++x) {
            int n = neighborCount(x,y);
            bool alive = grid_[index(x,y)].alive;
            if (alive) {
                bool survive = std::find(survive_rules_.begin(), survive_rules_.end(), n) != survive_rules_.end();
                next[index(x,y)].alive = survive;
            } else {
                bool birth = std::find(birth_rules_.begin(), birth_rules_.end(), n) != birth_rules_.end();
                next[index(x,y)].alive = birth;
            }
        }
    }
    grid_.swap(next);
}

void DigitalPhysicsDemo::update(float) {
    step();
}

void DigitalPhysicsDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        std::vector<glm::vec3> points;
        for (int y=0; y<height_; ++y) {
            for (int x=0; x<width_; ++x) {
                if (grid_[index(x,y)].alive) {
                    points.emplace_back((float)x/width_, 0.0f, (float)y/height_);
                }
            }
        }
        renderer_->renderPatternState(points);
    }
#else
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (int y=0; y<height_; ++y) {
        for (int x=0; x<width_; ++x) {
            if (grid_[index(x,y)].alive) {
                points.emplace_back((float)x/width_, 0.0f, (float)y/height_);
            }
        }
    }
    renderer_->renderPatternState(points);
#endif
}

void DigitalPhysicsDemo::cleanup() {
    grid_.clear();
}

void DigitalPhysicsDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(0,1);
        for (auto& cell : grid_) cell.alive = dist(rng)==1;
    }
}

void DigitalPhysicsDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
