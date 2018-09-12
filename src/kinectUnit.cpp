#include "kinectUnit.h"

//-------------------------
void kinectUnit::setup(int kid, int x, int y, int width, int height, int port)
{
	_kinectId = kid;
	_range.set(x, y, width, height);
	_udp.Create();
	_udp.Bind(port);
	_udp.SetNonBlocking(true);
	_timer = cKinectUDPUpdateTime;
}

//-------------------------
bool kinectUnit::update(float delta)
{
	_timer -= delta;
	if (_timer <= 0.0f)
	{
		_timer = cKinectUDPUpdateTime;

		kinectPackage package;
		char udpMessage[cKinectUDPLength] = { 0 };
		if (_udp.Receive(udpMessage, cKinectUDPLength) != 0)
		{
			memcpy(&package, udpMessage, sizeof(kinectPackage));

			if (package._kid == _kinectId)
			{
				CopyMemory(_blobList, package._blobData, sizeof(blobData) * cKMaxBlobNumEach);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------------
void kinectUnit::draw()
{
	for (auto& iter : _blobList)
	{
		ofDrawRectangle(iter.x, iter.y, iter.width, iter.height);
	}
}
