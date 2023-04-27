#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/Camera.h"
#include "Object_3D.h"
#include <string>
#include <iostream>

#include <vector>

using namespace std;
using namespace m1;

m1::Tema2::Tema2()
{
}

m1::Tema2::~Tema2()
{
}

void m1::Tema2::Init()
{

    renderCameraTarget = false;
    projectionType = true;
    right = 10.f;
    left = -10.f;
    bottom = -10.f;
    top = 10.f;
    fov = 40.f;
    rotateAngle = 0;
    car_indice = 0;
    lastPressedKey = 1;
    nrOfCars = 5;


    // Determines the starting point of each obstacle
    for (int i = 0; i < nrOfCars; i++) {
        other_cars_indices.push_back(i * 500);
    }

    // Points that generates the track
    aux_vertices = {
            VertexFormat(glm::vec3(14, 0,  0)),
            VertexFormat(glm::vec3(12, 0,  4)), 
            VertexFormat(glm::vec3(10, 0,  8)), 
            VertexFormat(glm::vec3(5, 0,  10)), 
            VertexFormat(glm::vec3(0, 0,  12)), 
            VertexFormat(glm::vec3(-4, 0, 10)), 
            VertexFormat(glm::vec3(-7.5, 0,  8.2)), 
            VertexFormat(glm::vec3(-12, 0, 9)), 
            VertexFormat(glm::vec3(-16, 0,  6.8)), 
            VertexFormat(glm::vec3(-19, 0,  4.5)), 
            VertexFormat(glm::vec3(-20, 0,  0)), 
            VertexFormat(glm::vec3(-17, 0,  -4)), 
            VertexFormat(glm::vec3(-14.9, 0,  -7)), 
            VertexFormat(glm::vec3(-10, 0, -8)), 
            VertexFormat(glm::vec3(-5.2, 0, -7.2)), 
            VertexFormat(glm::vec3(0, 0,  -5)),  
            VertexFormat(glm::vec3(4.3, 0,  -6.6)), 
            VertexFormat(glm::vec3(7.6, 0,  -7.9)), 
            VertexFormat(glm::vec3(11.1, 0,  -7.3)),
            VertexFormat(glm::vec3(13.1, 0,  -3.6)),
    };

    // Calculate track limits
    glm::vec3 dist = glm::vec3(0.3, 0, 0.3);
    glm::vec3 D, P, R, A;

    for (int i = 0; i < aux_vertices.size() - 1; i++) {
        D = aux_vertices[i + 1].position - aux_vertices[i].position;
        P = cross(D, glm::vec3(0, 1, 0));
        R = aux_vertices[i].position + dist * P;
        A = aux_vertices[i].position - dist * P;
        trackLimits.push_back(VertexFormat(R));
        trackLimits.push_back(VertexFormat(A));
    }

    D = aux_vertices[0].position - aux_vertices[aux_vertices.size() - 1].position;
    P = cross(D, glm::vec3(0, 1, 0));
    R = aux_vertices[aux_vertices.size() - 1].position + dist * P;
    A = aux_vertices[aux_vertices.size() - 1].position - dist * P;
    trackLimits.push_back(VertexFormat(R));
    trackLimits.push_back(VertexFormat(A));

    // Generates routes for other cars
    
    for (int k = 0; k < nrOfCars; k++) {
        std::vector<VertexFormat> aux;
        glm::vec3 dist_cars = glm::vec3(rand()%20/100.f, 0, rand()%30/100.f);

        for (int i = 0; i < aux_vertices.size() - 1; i++) {
            D = aux_vertices[i + 1].position - aux_vertices[i].position;
            P = cross(D, glm::vec3(0, 1, 0));
            R = aux_vertices[i].position - dist_cars * P;            
            aux.push_back(VertexFormat(R));
        }
        D = aux_vertices[0].position - aux_vertices[aux_vertices.size() - 1].position;
        P = cross(D, glm::vec3(0, 1, 0));
        R = aux_vertices[aux_vertices.size() - 1].position - dist_cars * P;

        aux.push_back(VertexFormat(R));
        other_cars.push_back(aux);

        // Calcultes all the points a car goes through
        float x, z;
        std::vector<glm::vec3> aux_routes;
        for (int i = 0; i < aux_vertices.size() - 1; i++) {
            
            D = other_cars[k][i + 1].position - other_cars[k][i].position;
            x = D.x / 200;
            z = D.z / 200;
            aux_routes.push_back(other_cars[k][i].position);
            for (int j = 1; j < 200; j++) {
                aux_routes.push_back(glm::vec3(other_cars[k][i].position.x + x * j, 0, other_cars[k][i].position.z + z * j));
            }
        }
        other_cars_routes.push_back(aux_routes);

        D = other_cars[k][0].position - other_cars[k][other_cars[k].size() - 1].position;
        x = D.x / 200;
        z = D.z / 200;
        other_cars_routes[k].push_back(other_cars[k][other_cars[k].size() - 1].position);

        for (int j = 1; j < 200; j++) {
            other_cars_routes[k].push_back(glm::vec3(other_cars[k][other_cars[k].size() - 1].position.x + x * j, 0, other_cars[k][other_cars[k].size() - 1].position.z + z * j));
        }
    }

    // Sets the camera in the first point
    camera = new implemented::Camera_t1();
    camera->Set(glm::vec3(14, 1.5, 2.7), glm::vec3(14  - 0.15, 0, -0.3), glm::vec3(0, 1, 0));

    // Sets the mini map camera
    camera_MiniMap = new implemented::Camera_t1();
    camera_MiniMap->Set(glm::vec3(14, 15, 2.7), glm::vec3(14 - 0.15, 0, -0.3), glm::vec3(0, 1, 0));
    
    // Sets the miniport area
    miniViewportArea = ViewportArea(1000, 50, window->GetResolution().x / 5.f, window->GetResolution().y / 5.f);

    Mesh* raceTrack = objects_3D::CreateRaceTrack("raceTrack", glm::vec3(0, 0, 0));
    AddMeshToList(raceTrack);

    Mesh* trees = objects_3D::CreateTrees("trees", glm::vec3(0.5, 0.35, 0.05), glm::vec3(0,0.5,0));
    AddMeshToList(trees);

    Mesh* race_car = objects_3D::CreateCar("race_car", glm::vec3(0, 1, 1));
    AddMeshToList(race_car);

    Mesh* obstacles = objects_3D::CreateCar("obstacles", glm::vec3(1, 1, 0));
    AddMeshToList(obstacles);

    Mesh* grass = objects_3D::CreateGrass("grass", glm::vec3(0.5, 1, 0));
    AddMeshToList(grass);

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    miniMap_projMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
}

void m1::Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void m1::Tema2::Update(float deltaTimeSeconds)
{

    {
        /* Renders the race track and the trees */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["raceTrack"], shaders["VertexColor"], modelMatrix);


        glm::vec3 dist = glm::vec3(0.6, 0, 0.6);
        glm::vec3 D, P, R, A;

        for (int i = 0; i < aux_vertices.size() - 1; i++) {
            D = aux_vertices[i + 1].position - aux_vertices[i].position;
            P = cross(D, glm::vec3(0, 1, 0));
            R = aux_vertices[i].position - dist * P;
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, R);
            RenderMesh(meshes["trees"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        /* Renders the car to the camera position */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        RenderMesh(meshes["race_car"], shaders["VertexColor"], modelMatrix);
    }

    {
        /* Renders other cars */
        for (int k = 0; k < nrOfCars; k++) {

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, other_cars_routes[k][other_cars_indices[k]]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
            RenderMesh(meshes["obstacles"], shaders["VertexColor"], modelMatrix);

            if (other_cars_indices[k] < other_cars_routes[k].size() - 1) {
                other_cars_indices[k]++;
            }
            else {
                other_cars_indices[k] = 0;
            }
        }
    }

    {
        /* Renders the grass */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix);
    }

    UpdateMiniMap();
}

void Tema2::UpdateMiniMap() {

    // Set the position above the race car's position
    camera_MiniMap->position = glm::vec3(camera->GetTargetPosition().x, 15, camera->GetTargetPosition().z);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    {
        /* Renders the race track and the trees */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMeshMiniMap(meshes["raceTrack"], shaders["VertexColor"], modelMatrix);


        glm::vec3 dist = glm::vec3(0.6, 0, 0.6);
        glm::vec3 D, P, R, A;

        for (int i = 0; i < aux_vertices.size() - 1; i++) {
            D = aux_vertices[i + 1].position - aux_vertices[i].position;
            P = cross(D, glm::vec3(0, 1, 0));
            R = aux_vertices[i].position - dist * P;
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, R);
            RenderMeshMiniMap(meshes["trees"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        /* Renders the car to the camera position */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        RenderMeshMiniMap(meshes["race_car"], shaders["VertexColor"], modelMatrix);
    }

    {
        /* Renders other cars */
        for (int k = 0; k < nrOfCars; k++) {

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, other_cars_routes[k][other_cars_indices[k]]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
            RenderMeshMiniMap(meshes["obstacles"], shaders["VertexColor"], modelMatrix);

            if (other_cars_indices[k] < other_cars_routes[k].size() - 1) {
                other_cars_indices[k]++;
            }
            else {
                other_cars_indices[k] = 0;
            }
        }
    }

    {
        /* Renders the grass */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMeshMiniMap(meshes["grass"], shaders["VertexColor"], modelMatrix);
    }
}

/* Checks if a collision occurs between a car and a obstacle */
bool Tema2::CheckCollisions() {
    glm::vec3 obstaclePos;
    glm::vec3 carPos = camera->GetTargetPosition();

    for (int k = 0; k < nrOfCars; k++) {
        obstaclePos = other_cars_routes[k][other_cars_indices[k]];
        float f = sqrt(pow(carPos.x - obstaclePos.x, 2) + pow(carPos.z - obstaclePos.z, 2));

        if (f <= 0.5f) {
            return true;
        }
    }
    return false;
}

/* Verify if the car is in the current, previous or next triangle */
bool Tema2::IsInsideTrack() {

    // Check if it is inside the current triangle
    if (CalculateAreas(car_indice)) {
        return true;
    }
    else { // Checks if the car moved on to the next triangle
        
        if (car_indice != trackLimits.size() - 1) {
            if (CalculateAreas(car_indice + 1)) {
                car_indice++;
                return true;
            }
        }
        else {
            if (CalculateAreas(0)) {
                car_indice = 0;
                return true;
            }
        }
        
        // Check if the car moved backwards to the last triangle
        if (car_indice != 0) {
            if (CalculateAreas(car_indice - 1)) {
                car_indice--;
                return true;
            }
        } else {
            if (CalculateAreas(trackLimits.size() - 1)) {
                car_indice = trackLimits.size() - 1;
                return true;
            }
        }
    }
    return false;
}

/* Checks if the car is in a given indice triangle */
bool Tema2::CalculateAreas(int i) {

    float triangleArea, area1, area2, area3;
    glm::vec3 pos = camera->GetTargetPosition();
    if (i == trackLimits.size() - 2) {
        triangleArea = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[i + 1].position.x, trackLimits[0].position.x,
            trackLimits[i].position.z, trackLimits[i + 1].position.z, trackLimits[0].position.z,
            1, 1, 1
        ))) / 2;
        area1 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[i + 1].position.x, pos.x,
            trackLimits[i].position.z, trackLimits[i + 1].position.z, pos.z,
            1, 1, 1
        ))) / 2;
        area2 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, pos.x, trackLimits[0].position.x,
            trackLimits[i].position.z, pos.z, trackLimits[0].position.z,
            1, 1, 1
        ))) / 2;
        area3 = abs(determinant(glm::mat3(
            pos.x, trackLimits[i + 1].position.x, trackLimits[0].position.x,
            pos.z, trackLimits[i + 1].position.z, trackLimits[0].position.z,
            1, 1, 1
        ))) / 2;
    }
    else if (i == trackLimits.size() - 1) {
        triangleArea = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[0].position.x, trackLimits[1].position.x,
            trackLimits[i].position.z, trackLimits[0].position.z, trackLimits[1].position.z,
            1, 1, 1
        ))) / 2;
        area1 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[0].position.x, pos.x,
            trackLimits[i].position.z, trackLimits[0].position.z, pos.z,
            1, 1, 1
        ))) / 2;
        area2 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, pos.x, trackLimits[1].position.x,
            trackLimits[i].position.z, pos.z, trackLimits[1].position.z,
            1, 1, 1
        ))) / 2;
        area3 = abs(determinant(glm::mat3(
            pos.x, trackLimits[0].position.x, trackLimits[1].position.x,
            pos.z, trackLimits[0].position.z, trackLimits[1].position.z,
            1, 1, 1
        ))) / 2;
    }
    else {
        triangleArea = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[i + 1].position.x, trackLimits[i + 2].position.x,
            trackLimits[i].position.z, trackLimits[i + 1].position.z, trackLimits[i + 2].position.z,
            1, 1, 1
        ))) / 2;
        area1 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, trackLimits[i + 1].position.x, pos.x,
            trackLimits[i].position.z, trackLimits[i + 1].position.z, pos.z,
            1, 1, 1
        ))) / 2;
        area2 = abs(determinant(glm::mat3(
            trackLimits[i].position.x, pos.x, trackLimits[i + 2].position.x,
            trackLimits[i].position.z, pos.z, trackLimits[i + 2].position.z,
            1, 1, 1
        ))) / 2;
        area3 = abs(determinant(glm::mat3(
            pos.x, trackLimits[i + 1].position.x, trackLimits[i + 2].position.x,
            pos.z, trackLimits[i + 1].position.z, trackLimits[i + 2].position.z,
            1, 1, 1
        ))) / 2;
    }
    if ((area1 + area2 + area3) <= (triangleArea + 0.1)) {
        return true;
    }
    return false;
}

void m1::Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void m1::Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void m1::Tema2::RenderMeshMiniMap(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera_MiniMap->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(miniMap_projMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void m1::Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    float carSpeed = 2.5f;
    if (!CheckCollisions()) {

        // If the car is on track move it with carSpeed
        if (IsInsideTrack()) {
            if (window->KeyHold(GLFW_KEY_W)) {
                camera->MoveForward(carSpeed * deltaTime);
                lastPressedKey = 1;
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                camera->RotateThirdPerson_OY(carSpeed * deltaTime);
                rotateAngle += carSpeed * deltaTime;
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                camera->MoveForward(-carSpeed * deltaTime);
                lastPressedKey = 0;

            }

            if (window->KeyHold(GLFW_KEY_D)) {
                camera->RotateThirdPerson_OY(-carSpeed * deltaTime);
                rotateAngle -= carSpeed * deltaTime;
            }
        }

        else { // Else put the car back to a valid position based on the last key pressed
            if (lastPressedKey == 1) {
                if (!IsInsideTrack()) {
                    camera->MoveForward(-carSpeed * deltaTime);
                }
            }
            else {
                if (!IsInsideTrack()) {
                    camera->MoveForward(carSpeed * deltaTime);
                }
            }
        }
    }
}

void m1::Tema2::OnKeyPress(int key, int mods)
{
    
}

void m1::Tema2::OnKeyRelease(int key, int mods)
{
}

void m1::Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void m1::Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void m1::Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void m1::Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void m1::Tema2::OnWindowResize(int width, int height)
{
}
