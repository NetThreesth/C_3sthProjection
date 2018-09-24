#include "lightSet.h"

void lightSet::setup()
{
	_pointLight.setDiffuseColor(ofColor(64));

	// specular color, the highlight/shininess color //
	_pointLight.setSpecularColor(ofColor(255));
	_pointLight.setPointLight();


	_spotLight.setDiffuseColor(ofColor(255));
	_spotLight.setSpecularColor(ofColor(255));

	// turn the light into spotLight, emit a cone of light //
	_spotLight.setSpotlight();

	// size of the cone of emitted light, angle between light axis and side of cone //
	// angle range between 0 - 90 in degrees //
	_spotLight.setSpotlightCutOff(90);

	// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	// range 0 - 128, zero is even illumination, 128 is max falloff //
	_spotLight.setSpotConcentration(128);


	// Directional Lights emit light based on their orientation, regardless of their position //
	_directionalLight.setDiffuseColor(ofColor(255.f));
	_directionalLight.setSpecularColor(ofColor(255.f));
	_directionalLight.setDirectional();

	// set the direction of the light
	_directionalLight.setOrientation(ofVec3f(0, 0, 0));
}

void lightSet::update(float delta)
{
	_spotLight.setPosition(600 * sin(ofGetElapsedTimef()), 0, 500 * cos(ofGetElapsedTimef()));
	_spotLight.lookAt(ofVec3f());

	_pointLight.setPosition(600 * (1 - sin(ofGetElapsedTimef())), 0, 600 * (1 - cos(ofGetElapsedTimef())));
	_pointLight.lookAt(ofVec3f());
}

void lightSet::begin()
{
	ofEnableLighting();
	_directionalLight.enable();
	_spotLight.enable();
	_pointLight.enable();
}

void lightSet::end()
{
	_directionalLight.disable();
	_spotLight.disable();
	_pointLight.disable();
	ofDisableLighting();
}
