#include "SceneNode.h"

// The constructor
SceneNode::SceneNode(Object* ob) : object(ob) {
	init();      
}

// Used to set light offsets when creating a terrain
SceneNode::SceneNode(Object* ob, int xSegs, int zSegs) : object(ob), m_xSegments(xSegs), m_zSegments(zSegs) {
	init();      
}

// The destructor 
SceneNode::~SceneNode(){
	// Remove each object
	for(unsigned int i =0; i < children.size(); ++i){
		delete children[i];
	}
}

void SceneNode::init() {
	std::cout << "(SceneNode.cpp) Constructor Called\n";

	// Initialize timer settings
	m_currentTime = SDL_GetPerformanceCounter();
	m_previousTime = 0.0f;
	m_elapsedTime = 0.0f;

	// By default, we do not know the parent
	// at the time of construction of this node.
	// If the SceneNode is the root of the tree,
	// then there is no parent.
	parent = nullptr;
	
	// Setup shaders for the node.
	std::string vertexShader = myShader.LoadShader("./shaders/vert.glsl");
	std::string fragmentShader = myShader.LoadShader("./shaders/frag.glsl");
	// Actually create our shader
	myShader.CreateShader(vertexShader, fragmentShader); 
}
// Adds a child node to our current node.
void SceneNode::AddChild(SceneNode* n){
	// For the node we have added, we can set
	// it's parent now to our current node.
	// 'this' is the current instance of our
	// object, which is a pointer to our current
	// SceneNode.
	n->parent = this;
	// Add a child node into our SceneNode
	children.push_back(n);
}

// Draw simply draws the current nodes
// object and all of its children. This is done by calling directly
// the objects draw method.
void SceneNode::Draw(){
	myShader.Bind();
	if(object!=nullptr){
		object->render();
		for(int i = 0; i < children.size(); ++i){
			children[i]->Draw();
		}
	}	
}

// Update simply updates the current nodes
// object. This is done by calling directly
// the objects update method.
// TODO: Consider not passting projection and camera here
void SceneNode::Update(glm::mat4 projectionMatrix, Camera* camera) {
    if (object != nullptr) {
		m_previousTime = m_currentTime;
		m_currentTime = SDL_GetPerformanceCounter();
		// * 1000 converts milliseconds into seconds
		float deltaTime = (m_currentTime - m_previousTime) * 1000 / SDL_GetPerformanceFrequency();

		m_elapsedTime += deltaTime;

		int planeMode_ID;

		if (m_planeMode == "flat") {
			planeMode_ID = 0;
		} else if (m_planeMode == "yAxis") {
			planeMode_ID = 1;
		} else {
			planeMode_ID = 2;
		}

    	// Apply our shader transformation

		myShader.Bind();
    	// Set the uniforms in our current shader

		myShader.setUniform1i("planeMode", planeMode_ID);

        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        // myShader.setUniform1i("material.u_diffuse", 0);
		myShader.setUniform1f("material_shininess", 32.0f);

		myShader.setUniform1f("amplitude", m_amplitude);
		myShader.setUniform1f("waveNumber", m_waveNumber);
		myShader.setUniform1f("wavePeriod", m_wavePeriod);
		
		myShader.setUniform1f("time", m_elapsedTime / 60.0f);


        // Set the MVP Matrix for our object
        // Send it into our shader
        myShader.setUniformMatrix4fv("model", &worldTransform.getInternalMatrix()[0][0]);
        myShader.setUniformMatrix4fv("view", &camera->getWorldToViewmatrix()[0][0]);
        myShader.setUniformMatrix4fv("projection", &projectionMatrix[0][0]);

		myShader.setUniform3f("viewPos",
							  camera->getEyeXPosition(),
							  camera->getEyeYPosition(),
							  camera->getEyeZPosition());

        // Create a directional light
        myShader.setUniform3f("dirLight.direction", 0.0f, 5.0f, 0.0f);
        myShader.setUniform3f("dirLight.color", 1.0f, 1.0f, 1.0f);
        myShader.setUniform1f("dirLight.ambientIntensity", 0.1f);
		myShader.setUniform1f("dirLight.specularStrength", 0.3f);

		// Set point lights attributes
		const int numLights = 13;
		const float pointLightsOffsetX = (m_xSegments / 2) * 0.8f;
		const float pointLightsOffsetZ = (m_zSegments / 2) * 0.8f;
		const float pointLightsHeight = 5.0f;
		const glm::vec3 pointLightsColor = glm::vec3(1.0f);
		const float pointLightsIntensity = 8.0f;
		const float pointLightsSpecularStrength = 0.2f;
		const float constant = 1.0f;
		const float linear = 0.09f;
		const float quadratic = 0.032f;

		// Set point light positions
		myShader.setUniform3f("pointLights[0].position",
			0.0f, pointLightsHeight, 0.0f);
		myShader.setUniform3f("pointLights[1].position",
			-pointLightsOffsetX, pointLightsHeight, -pointLightsOffsetZ);
		myShader.setUniform3f("pointLights[2].position",
			-pointLightsOffsetX, pointLightsHeight, pointLightsOffsetZ);
		myShader.setUniform3f("pointLights[3].position",
			pointLightsOffsetX, pointLightsHeight, -pointLightsOffsetZ);
		myShader.setUniform3f("pointLights[4].position",
			pointLightsOffsetX, pointLightsHeight, pointLightsOffsetZ);
		myShader.setUniform3f("pointLights[5].position",
			-pointLightsOffsetX / 1.75f, pointLightsHeight, -pointLightsOffsetZ / 1.75f);
		myShader.setUniform3f("pointLights[6].position",
			-pointLightsOffsetX / 1.75f, pointLightsHeight, pointLightsOffsetZ / 1.75f);
		myShader.setUniform3f("pointLights[7].position",
			pointLightsOffsetX / 1.75f, pointLightsHeight, -pointLightsOffsetZ / 1.75f);
		myShader.setUniform3f("pointLights[8].position",
			pointLightsOffsetX / 1.75f, pointLightsHeight, pointLightsOffsetZ / 1.75f);
		myShader.setUniform3f("pointLights[9].position",
			-pointLightsOffsetX / 1.25f, pointLightsHeight, 0.0f);
		myShader.setUniform3f("pointLights[10].position",
			pointLightsOffsetX / 1.25f,
			pointLightsHeight, 0.0f);
		myShader.setUniform3f("pointLights[11].position",
			0.0f, pointLightsHeight, -pointLightsOffsetZ / 1.25f);
		myShader.setUniform3f("pointLights[12].position",
			0.0f, pointLightsHeight, pointLightsOffsetZ / 1.25f);

		// Set other point light values
		std::string tempString;
		for (unsigned int i = 0; i < numLights; i++) {
			// Color
			tempString = "pointLights[" + std::to_string(i) + "].color";
			myShader.setUniform3f(strToCharArray(tempString), pointLightsColor.x, pointLightsColor.y, pointLightsColor.z);

			// Ambient Intensity
			tempString = "pointLights[" + std::to_string(i) + "].ambientIntensity";
			myShader.setUniform1f(strToCharArray(tempString), pointLightsIntensity);

			// Specular Strength
			tempString = "pointLights[" + std::to_string(i) + "].specularStrength";
			myShader.setUniform1f(strToCharArray(tempString), pointLightsSpecularStrength);

			// Constant
			tempString = "pointLights[" + std::to_string(i) + "].constant";
			myShader.setUniform1f(strToCharArray(tempString), constant);

			// Linear
			tempString = "pointLights[" + std::to_string(i) + "].linear";
			myShader.setUniform1f(strToCharArray(tempString), linear);

			// Quadratic
			tempString = "pointLights[" + std::to_string(i) + "].quadratic";
			myShader.setUniform1f(strToCharArray(tempString), quadratic);
		}
		
		for(int i =0; i < children.size(); ++i){
			children[0]->Update(projectionMatrix, camera);
		}
	}
}

// Returns the actual local transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::getLocalTransform(){
    return localTransform; 
}

// Returns the worled  transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::getWorldTransform(){
    return worldTransform; 
}
