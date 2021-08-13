#pragma once

#include "vmc_model.hpp"
#include "chunk_component.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std 
#include <memory>

namespace vmc {

 // TODO: Make `Component` a base type?
 // TODO: Find cleaner solution for chunk management! (each gameobject currently contains a chunk)

    struct TransformComponent {
        glm::vec3 translation{};  // Position offset
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };
     
    class VmcGameObject {

    public:
        using id_t = unsigned int;

        ~VmcGameObject();

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
        ChunkComponent * chunk;

    private:
        VmcGameObject(id_t objId) : id{ objId } {
            // Init chunk of width 16
            chunk = new ChunkComponent(16);
        }
        id_t id;
    };
}

