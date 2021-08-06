#pragma once

#include "vmc_model.hpp"

// std 
#include <memory>

namespace vmc {

    struct Transform2DComponent {
        glm::vec2 translation{};  // Position offset
        glm::vec2 scale{ 1.f, 1.f };
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{ {c, s}, {-s, c} };

            glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
            return rotMatrix * scaleMat;
        }
    };

    class VmcGameObject {
    public:
        using id_t = unsigned int;

        static VmcGameObject createGameObject() {
            static id_t currentId = 0;
            return VmcGameObject{ currentId++ };
        }

        VmcGameObject(const VmcGameObject&) = delete;
        VmcGameObject& operator=(const VmcGameObject&) = delete;
        VmcGameObject(VmcGameObject&&) = default;
        VmcGameObject& operator=(VmcGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<VmcModel> model{};
        glm::vec3 color{};
        Transform2DComponent transform2d{};

    private:
        VmcGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}

