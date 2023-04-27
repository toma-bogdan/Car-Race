#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Camera.h"
#include <iostream>
#include <vector>

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1 {
    class Tema2 : public gfxc::SimpleScene {
    public:
        Tema2();
        ~Tema2();

        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderMeshMiniMap(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void UpdateMiniMap();
        bool IsInsideTrack();
        bool CheckCollisions();
        bool CalculateAreas(int i);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera_t1* camera;
        implemented::Camera_t1* camera_MiniMap;
        glm::mat4 projectionMatrix;
        glm::mat4 miniMap_projMatrix;
        bool renderCameraTarget;
        bool projectionType;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
        std::vector<VertexFormat> aux_vertices;
        std::vector <VertexFormat> trackLimits;
        std::vector<vector<VertexFormat>> other_cars; // basic points for obstacles
        std::vector<vector<glm::vec3>> other_cars_routes; // all the points for obstacles
        int nrOfCars = 3;
        std::vector<int> other_cars_indices; // retains in which point the obstacles are
        int car_indice; // retains in which triangle the car is
        int lastPressedKey;
        GLfloat rotateAngle;
        ViewportArea miniViewportArea;
    };
}
