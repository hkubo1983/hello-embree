#include<iostream>

#pragma comment(lib, "embree.lib")
#include <common/sys/platform.h>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

int main(void)
{
	/* create device */
	RTCDevice device = rtcNewDevice(NULL);

	/* create new scene */
	RTCScene scene = rtcDeviceNewScene(device, RTC_SCENE_DYNAMIC, RTC_INTERSECT1);

	/* four vertices on z-plane */
	float vertices[] = {
		0, 0, 0, // x, y, z
		0, 1, 0, // x, y, z
		1, 1, 0, // x, y, z
		1, 0, 0, // x, y, z
	};

	/* index list of two triangles */
	int indices[] = {
		0, 1, 2, // first triangle
		0, 2, 3, // second triangle
	};

	int nvert = 4; // num vertex
	int ntri = 2; // num triangles
	
	/* set geometry info to the scene */
	unsigned int geomId = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, ntri, nvert, 1);
	rtcSetBuffer(scene, geomId, RTC_VERTEX_BUFFER, vertices, 0, sizeof(float)*3);
	rtcSetBuffer(scene, geomId, RTC_INDEX_BUFFER, indices, 0, sizeof(int)*3);
	
	/* commit */
	rtcCommit(scene);

	/* create a ray */
	RTCRay rtcray;
	/* ray origin */
	rtcray.org[0] = 0; // x
	rtcray.org[1] = 0; // y
	rtcray.org[2] = -10; // z
	/* ray direction */
	rtcray.dir[0] = 0; // x
	rtcray.dir[1] = 0; // y
	rtcray.dir[2] = 1; // z

	rtcray.tnear = 1e-5f; // start of ray segment
	rtcray.tfar = 1e8f;  // end of ray segment, set to hit distance after intersection

	/* find intersection */
	rtcIntersect(scene, rtcray);

	if (rtcray.geomID == RTC_INVALID_GEOMETRY_ID)
	{
		std::cout << "Reject." << std::endl;
	}
	else
	{
		std::cout << "Intersect" << std::endl;
	}

	/* delete scene */
	rtcDeleteScene(scene);

	/* delete device */
	rtcDeleteDevice(device);


	return 0;
}