#include "pch.h"
#include <iostream>
#include "main.h"

#include "GameManager.h"
#include "Pathfinding.h"

#include <cstdlib>
#include <crtdbg.h>

int main()
{
    Window* window = new Window(L"Pathfinding", 1920, 1080);
    

    Camera camera(CameraType::PERSPECTIVE);
    camera.SetPosition({ 15.0f, 8.0f, 25.0f });
    camera.SetRotation({ 180.0f, 0.0f, 0.0f });
    camera.SetFOV(gce::PI / 4.0f);
    camera.SetFarPlane(500.0f);
    camera.SetNearPlane(0.001f);

	GameManager* pInstance = GameManager::Get();

    pInstance->SetWindow(window);
    pInstance->SetCamera(&camera);

	pInstance->LaunchScene<Pathfinding>();

    return 0;
}