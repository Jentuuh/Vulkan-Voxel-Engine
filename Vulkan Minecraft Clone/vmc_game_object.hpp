#pragma once

#include "vmc_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std 
#include <memory>

namespace vmc {

    struct TransformComponent {
        glm::vec3 translation{};  // Position offset
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        /**
         Constructs and returns a TRS-matrix that corresponds to translation * rotation.y * rotation.x * rotation.z * scale.  
         https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix (optimized implementation)
        */
        glm::mat4 mat4() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f} };
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
        TransformComponent transform{};

    private:
        VmcGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}

