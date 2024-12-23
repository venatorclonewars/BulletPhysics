#include <GL/glew.h>          // GLEW must come before OpenGL-related headers
#include <GLFW/glfw3.h>       // GLFW headers for windowing and input
#include "game.h"
#include <stdio.h>
#include <string>
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "skinnedMesh.h"
#include "time.h"
#include "lightingTechnique.h"
#include "terrainTehnique.h"
#include "faultFormationTerrain.h"
#include "midPointDispTerrain.h"
#include "textureGenerator.h"
#include "3DOBjects/cube.h"
#include "3DOBjects/lightweightCube.h"
#include "3DOBjects/plane.h"
#include "customDebugDrawer.h"
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

GLuint VAO = -1;
GLuint VBO = -1;
GLuint IBO = -1;

GLFWwindow* window = NULL;

Texture* pTexture = NULL;
Mesh* pMesh = NULL;
SkinnedMesh* pSkinnedMesh = NULL;
LightingTechnique* pLightingTech = NULL;
TerrainTehnique* pTerrainTech = NULL;
BaseLight baseLight;
DirectionalLight dirLight;
//FaultFormationTerrain m_terrain;
MidpointDispTerrain m_terrain;

Camera camera;
Time* pTime = new Time();

btDiscreteDynamicsWorld* dynamicsWorld;
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
//btDiscreteDynamicsWorld* dynamicsWorld;
btRigidBody* fallRigidBody;

//vector<Cube*> m_objects;
vector<Cube*> m_objects;
Plane* plane;

vector<Vector3f> m_positions
{
    Vector3f(25.0f, 20.0f, 25.5f),
    Vector3f(25.0f, 30.0f, 25.0f)
};

int numOfObjects = 2;

Game::Game()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // First init this window, later fullscreen
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("BattlefrontRemake");
    printf("window id: %d\n", win);

    glutFullScreen();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        exit(1);
    }
    camera = Camera(this);
    //setGUI();

    pMesh = new Mesh();
    pSkinnedMesh = new SkinnedMesh();

    pSkinnedMesh->loadMesh("Models/Doom/boblampclean.md5mesh");

    /*vector<Matrix4f> transforms;
    pSkinnedMesh->getBoneTransforms(transforms);*/

    //Models/Chess/chess_set_1k.fbx
    if (!pMesh->loadMesh("Models/Vase/antique_ceramic_vase_01_1k.fbx"))
        exit(1);



    pLightingTech = new LightingTechnique();
    if (!pLightingTech->init())
        exit(1);

    pTerrainTech = new TerrainTehnique();
    if (!pTerrainTech->init())
        exit(1);


    pLightingTech->enable();


    //pLightingTech->setTextureUnit(COLOR_TEXTURE_UNIT_INDEX_0);
    //pLightingTech->setSpecularExpTextureUnit(SPECULAR_EXPONENT_UNIT_INDEX_0);
    initializeBulletPhysics();
    initTerrainTextureGenerator();

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);


    // Good for skeletal animation
    //dirLight.ambientIntensity = 0.1f;
    //dirLight.diffuseIntensity = 1.0f;
    //dirLight.worldDirection = Vector3f(10.0f, 0.0f, 50.0f);

    dirLight.ambientIntensity = 10.0f;
    dirLight.diffuseIntensity = 10.0f;
    dirLight.worldDirection = Vector3f(10.0f, 0.0f, 5.0f);

    camera.setWeightColorDebug(pSkinnedMesh, pLightingTech);


    CustomDebugDrawer* debugDrawer = new CustomDebugDrawer();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(debugDrawer);
}

Game::~Game() {
    // Clean up Bullet Physics components
    if (dynamicsWorld) {
        for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) 
            {
                delete body->getMotionState();
            }
            dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }
    }

    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;

    // Clean up game objects
    for (Cube* object : m_objects) 
    {
        delete object;
    }

    m_objects.clear();

    delete plane;

    // Clean up other dynamically allocated objects
    delete pTexture;
    delete pMesh;
    delete pSkinnedMesh;
    delete pLightingTech;
    delete pTerrainTech;
    delete pTime;

    // Clean up GLFW window if it exists
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    // Terminate GLFW (if this is your main context)
    glfwTerminate();
}


void Game::run()
{
    GLclampf red = 0.0f, green = 0.0f, blue = 0.0f, alpha = 0.0f;
    glClearColor(red, green, blue, alpha);

    glutCallbacks();

    glutMainLoop();
}

void Game::initializeBulletPhysics()
{
    broadphase = new btDbvtBroadphase();

    // Create the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    // Set gravity (falling downward)
    dynamicsWorld->setGravity(btVector3(0, -5, 0));


    for (int i = 0; i < numOfObjects; i++)
    {
        m_objects.push_back(new Cube(m_positions[i], true, dynamicsWorld, Vector3f(10.0f, 5.0f, 5.0f)));
    }

    //plane = new Plane(Vector3f(25.0f, 0.0f, 25.0f), true, dynamicsWorld, 20.0f);
}

void Game::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float FOV = 45.0f;

    float aspectRatio = (float)glutGet(GLUT_SCREEN_WIDTH) / (float)glutGet(GLUT_SCREEN_HEIGHT);
    float nearZ = 0.001f;
    float farZ = 1000.0f;

    Matrix4f projection;
    projection.setProjection(FOV, aspectRatio, nearZ, farZ);

    Matrix4f temp, view, world;

    temp.setIdentity();
    temp.setRotationY(camera.rotY);
    view *= temp;

    temp.setIdentity();
    temp.setRotationX(camera.rotX);
    view *= temp;


    //camera.transformPos = m_terrain.constrainCameraPosToTerrain(camera.transformPos);
    temp.setIdentity();
    temp.setTranslation(camera.transformPos);
    temp.setTranslation(Vector3f(camera.transformPos.x, camera.transformPos.y, camera.transformPos.z));
    //view *= temp;
    Matrix4f _view = view * temp;

    Matrix4f camTranslation = camera.getMatrix();

    view.setTranslation(camTranslation.getTranslation());


    camera.cameraTransform = view;

    Matrix4f viewNonInverse(view);

    view.inverse();

    world = pSkinnedMesh->getWorldTransform();
    //world.setScale(Vector3f(5.0f, 5.0f, 5.0f));
    temp.setIdentity();
    temp.setRotationX(4.71239f);
    world *= temp;
    temp.setIdentity();
    temp.setRotationZ(3.141f);
    world *= temp;

    Matrix4f WVP = projection * view * world;

    pLightingTech->enable();
    pLightingTech->setWVP(WVP);
    pLightingTech->setDirectionalLight(dirLight);

    //pLightingTech->setMaterial(pMesh->getMaterial());
    dirLight.calcLocalDirection(world);


    // Reflection
    Matrix4f camToLocalTrans;
    camToLocalTrans.setTranslation(camera.transformPos.negate());

    Matrix4f camToLocalRot(view);
    camToLocalRot = camToLocalRot.transpose();

    Matrix4f camToLocalTransform = camToLocalRot * camToLocalTrans;

    Vector4f cameraWorldPos = Vector4f(camera.transformPos.x, camera.transformPos.y, camera.transformPos.z, 1.0f);
    //Vector4f camLocalPos = camToLocalTrans * cameraWorldPos;
    Vector4f camLocalPos = camToLocalTrans * Vector4f(camera.transformPos.x, camera.transformPos.y, camera.transformPos.z, 0.0f);

    Vector3f camLocalPos3f = Vector3f(camLocalPos.x, camLocalPos.y, camLocalPos.z);


    pLightingTech->setCameraLocalPos(camLocalPos3f);


    pLightingTech->setMaterial(pMesh->getMaterial());
    long long currentTimeMillis = pTime->getCurrentTimeInMillis();
    float animTimeSec = ((float)currentTimeMillis - pTime->startTimeMilliseconds) / 1000.0f;

    vector<Matrix4f> transforms;
    pSkinnedMesh->getBoneTransforms(animTimeSec, transforms);

    for (unsigned int i = 0; i < transforms.size(); i++)
    {
        pLightingTech->setBoneTransform(i, transforms[i]);
    }

    //pSkinnedMesh->render();

    dynamicsWorld->stepSimulation(1.0f / 60.f, 10);
    

    WVP = projection * view;

    Matrix4f objectWVP;

    for (int i = 0; i < numOfObjects; i++)
    {    
        objectWVP = WVP * m_objects[i]->getTransform();
        m_objects[i]->update(objectWVP, m_objects[i]->getTransform());
    }

    //plane->update(WVP * plane->getTransform());

    m_terrain.render(projection, viewNonInverse, _view, dirLight, camera.transformPos);
    //dynamicsWorld->debugDrawWorld();

    glutSwapBuffers();

}

void Game::displayNewFrame(int deltaTime)
{
    glutPostRedisplay();
    // call timerFunction again after ~16 ms (for ~60 FPS)
    glutTimerFunc(16, displayNewFrame, 0);
}

void Game::keyboardListener(unsigned char key, int mouse_x, int mouse_y)
{
    camera.onKeyboard(key);
}

void Game::specialKeyboardListener(int key, int mouse_x, int mouse_y)
{
    camera.onKeyboard(key);

}

void Game::mouseListener(int x, int y)
{
    camera.onMouse(x, y);
}

void Game::glutCallbacks()
{
    displayNewFrame(16);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyboardListener);
    glutPassiveMotionFunc(mouseListener);
    glutDisplayFunc(renderScene);
}

void Game::initTerrainTextureGenerator()
{
    float worldScale = 2.0f;
    float textureScale = 1.0f;

    m_terrain.initTerrain(worldScale, textureScale);

    int size = 17;
    float roughness = 1.0f;
    float minHeight = 0.0f;
    float maxHeight = 10.0f;


    m_terrain.createMidpointDisplacement(size, roughness, minHeight, maxHeight, dirLight, dynamicsWorld);

    TextureGenerator texGen;

    //texGen.loadTile("Textures/tilable-IMG_0044-verydark.png");

    texGen.loadTile("Textures/rock02_2.jpg");
    texGen.loadTile("Textures/rock01.jpg");
    texGen.loadTile("Textures/tilable-IMG_0044-verydark.png");
    texGen.loadTile("Textures/water.png");


    int texSize = 1024;
    Texture* pTexture = texGen.generateTexture(texSize, &m_terrain, minHeight, maxHeight);
    m_terrain.setTexture(pTexture);
}