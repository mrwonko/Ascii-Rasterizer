#include "Console.h"
#include "Event.h"
#include <iostream>
#include <sstream>

#include "AsciiRenderer.h"
#include "Vector3f.h"
#include "Windows.h"
#include "Matrix4x4f.h"
#include "WAAF.h";

#include "MathHelpers.h"
//#include <cassert>

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include "btBulletDynamicsCommon.h"

void ShowMenu()
{
	system("cls"); //clear screen - fixme: hack
	Console::Write("Ascii Adventure\nPlease go to the layout tab in the console properties and set the buffer size to be equal to the window size.\nPress Return to Start or Escape to Quit");
}

struct Player
{
	Player() : physShape(btVector3(.25f, .75f, .25f)) {}
	Vector3f position;
	btCylinderShape physShape;
};

btVector3 CalculateMoveDir(bool left, bool forward, bool right, bool back, float yaw)
{
	btVector3 moveDir;
	if(left) moveDir.setX(moveDir.x() - 1);
	if(right) moveDir.setX(moveDir.x() + 1);
	if(forward) moveDir.setZ(moveDir.z() + 1);
	if(back) moveDir.setZ(moveDir.z() - 1);
	btMatrix3x3 rotMat;
	rotMat.setEulerYPR(yaw, 0, 0);
	return rotMat * moveDir;
}

int main(int argc, char** argv)
{
	//Matrix4x4f mat = Matrix4x4f::PerspectiveProjection(90, 0.1, 100, 1.f/1.f);
	//assert((mat * Matrix4x4f()) == mat); //works :)

	if(1) //actual game
	{
		WAAF level;
		if(!level.LoadFromFile("../data/level1.waa"))
		{
			std::cerr<<"Could not load data/level1.waa"<<std::endl;
			system("pause");
			return 0;
		}
		Player player;
		player.position = level.GetPlayerPos();

		//    Physics Setup

		//collision configuration contains default setup for memory, collision setup
		btDefaultCollisionConfiguration collisionConfiguration;
		//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher collisionDispatcher(&collisionConfiguration);
		btDbvtBroadphase broadphase;
		//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver solver;
		//create physics world
		btDiscreteDynamicsWorld physicsWorld(&collisionDispatcher, &broadphase, &solver, &collisionConfiguration);

		//stuff copied from bullet samples
		physicsWorld.getSolverInfo().m_splitImpulse=true;
		physicsWorld.getSolverInfo().m_numIterations = 20;
		//set gravity
		physicsWorld.setGravity(btVector3(0,-10,0));

		btRigidBody levelBody(btRigidBody::btRigidBodyConstructionInfo(0.f, NULL, level.GetPhysMesh()));
		physicsWorld.addRigidBody(&levelBody);

		btRigidBody::btRigidBodyConstructionInfo playerBodyInfo(10.f, NULL, &player.physShape);
		playerBodyInfo.m_startWorldTransform.setOrigin(btVector3(player.position.X, player.position.Y, player.position.Z));
		btRigidBody playerBody(playerBodyInfo);
		physicsWorld.addRigidBody(&playerBody);

		//    Console Setup
		
		if(!Console::Init())
		{
			std::cerr<<"Could not initialize console!"<<std::endl;
			system("pause");
			return 0;
		}

		//    Renderer Setup
		
		static const float FOV = 90;
		static const float NEAR_CLIP = .1f;
		static const float FAR_CLIP = 100.f;
		AsciiRenderer renderer;
		{
			float width = Console::GetWidth();
			float height = Console::GetHeight() - 1; //cannot use last line due to end-of-line cursor wrap
			if(!renderer.Init(width, height, FOV, NEAR_CLIP, FAR_CLIP))
			{
				std::cerr<<"Could not initialize renderer!"<<std::endl;
				Console::Destroy();
				system("pause");
				return 0;
			}
		}

		//    Main Loop

		renderer.ApplyModelviewMatrix(Matrix4x4f::Translation(level.GetPlayerPos()));

		bool running = true; //the game, not the player (I lost the game -.-)
		//movement keys
		bool moveForward = false;
		bool moveLeft = false;
		bool moveRight = false;
		bool moveBack = false;
		bool moveUp = false; //aka jump
		bool menu = true;
		ShowMenu(); //initial render
		unsigned long long lastFrameTime = GetTimeMS();
		float pitch = 0.f; //in rad
		float yaw = 0.f;
		float mouseScaleX = .05f / 180.f * 3.1415f;
		float mouseScaleY = -.05f / 180.f * 3.1415f;
		float movespeed = 1.f;

		while(running)
		{
			MouseMoveEvent mouseMove;
			mouseMove.X = 0;
			mouseMove.Y = 0;
			//    Event Loop
			Event ev;
			while(Console::PollEvent(ev))
			{
				switch(ev.Type)
				{
				case KeyPressed:
					{
						switch(ev.Key.Code)
						{
						case KEYCODE_ESCAPE: // quit on escape pressed
							{
								if(menu)
								{
									running = false;
								}
								else
								{
									menu = true;
									ShowMenu();
								}
								break;
							}
						case KEYCODE_RETURN:
							{
								menu = false;
								break;
							}
						case KEYCODE_W:
							{
								moveForward = true;
								break;
							}
						case KEYCODE_A:
							{
								moveLeft = true;
								break;
							}
						case KEYCODE_S:
							{
								moveBack = true;
								break;
							}
						case KEYCODE_D:
							{
								moveRight = true;
								break;
							}
						case KEYCODE_SPACE:
							{
								moveUp = true;
								break;
							}
						default:
							{
								break;
							}
						}
						break;
					}
				case KeyReleased:
					{
						switch(ev.Key.Code)
						{
						case KEYCODE_W:
							{
								moveForward = false;
								break;
							}
						case KEYCODE_A:
							{
								moveLeft = false;
								break;
							}
						case KEYCODE_S:
							{
								moveBack = false;
								break;
							}
						case KEYCODE_D:
							{
								moveRight = false;
								break;
							}
						default:
							{
								break;
							}
						}
						break;
					}
				case Resized: // change resolution when window resized
					{
						if(!renderer.Init(ev.Resize.SizeX, ev.Resize.SizeY - 1, FOV, NEAR_CLIP, FAR_CLIP))
						{
							std::cerr << "Could not change resolution!" << std::endl;
							Console::Destroy();
							system("pause");
							return 0;
						}
						break;
					}
				case MouseMoved:
					{
						mouseMove = ev.MouseMove;
						break;
					}
				case MouseButtonPressed:
					{
						break;
					}
				case MouseButtonReleased:
					{
						break;
					}
				case MouseWheelMoved:
					{
						break;
					}
				default:
					{
						break;
					}
				}
			} //end event loop

			//    Game Logic
			unsigned long long frameTime = GetTimeMS();
			unsigned int deltaT = frameTime - lastFrameTime;
			lastFrameTime = frameTime;

			if(menu)
			{
				//no logic required in menu, just await events
			}
			else
			{
				yaw += mouseMove.X * mouseScaleX;
				pitch += mouseMove.Y * mouseScaleY;
				while(yaw > 360.f/ 180.f * 3.1415f) yaw -= 360.f/ 180.f * 3.1415f;
				while(yaw < 0.f) yaw += 360.f;
				//clip pitch to [-90°, 90°]
				if(pitch < -3.1415f/2.f) pitch = -3.1415f/2.f;
				if(pitch > 3.1415f/2.f) pitch = 3.1415f/2.f;

				//  move mouse cursor back to 100, 100 and queue mouse moves
				Console::Update();
				if(deltaT > 0)
				{
					physicsWorld.stepSimulation(deltaT); //should ideally be fixed - cba
					//todo: updating goes here
				}

				btVector3 moveDir = CalculateMoveDir(moveLeft, moveForward, moveRight, moveBack, yaw);
				moveDir = moveDir * deltaT * movespeed;
				//playerBody.applyCentralForce(moveDir);
			}

			//    Rendering
			if(menu)
			{
				//menu is fixed, no rendering required
				Sleep(5);
			}
			else
			{
				Matrix4x4f posMat;
				posMat._14 = -playerBody.getWorldTransform().getOrigin().x();
				posMat._24 = -playerBody.getWorldTransform().getOrigin().y();
				posMat._34 = -playerBody.getWorldTransform().getOrigin().z();
				renderer.SetModelviewMatrix(Matrix4x4f());
				renderer.ApplyModelviewMatrix(Matrix4x4f::SimpleRotation(pitch , yaw));
				renderer.ApplyModelviewMatrix(posMat);
				// clear
				renderer.ClearDepth();
				renderer.ClearColor();
				// render
				level.Render(renderer);
				// "switch buffers"
				std::stringstream ss;
				char const * const * const colBuf = renderer.GetColorBuffer();
				for(unsigned int y = 0; y < renderer.GetHeight(); ++y)
				{
					ss << std::string(colBuf[y], renderer.GetWidth());// << "\n"; //if width is not correct
				}
				Console::GotoXY(0, 0);
				Console::Write(ss.str());
			}
		}

		Console::Destroy();

		return 0;
	}
}