#ifndef UI_FRAMEWORK_METRICSCAMERA_H
#define UI_FRAMEWORK_METRICSCAMERA_H

#include <raylib.h>

class MetricsCamera {
public:
    MetricsCamera(int _origin_x, int _origin_y, int _pixels_per_meter);
    ~MetricsCamera();

    // Convert an absolute position on the window to a world position in meters
    Vector2 ConvertAbsoluteToMeters(int absolute_x, int absolute_y);
    Vector2 ConvertMetersToAbsolute(Vector2 position_meters);

    //Get the world position of the center of the camera
    Vector2 GetCameraTopLeft();
    Vector2 GetCameraCenter();

    //Set the position of the camera's center in the world
    void SetCameraTopLeft(Vector2 worldpos);

    //Set the position of the camera's top left in the world
    void SetCameraCenter(Vector2 worldpos);

    //Set the zoom
    void SetPixelsPerMeter(int new_zoom);

    //Convert units
    inline float PixelsToMeters(int pixels) { return (float)pixels/(float)pixels_per_meter; }
    inline float MetersToPixels(float meters) { return meters*(float)pixels_per_meter; }

    //Get zoom level
    inline int PixelsPerMeter() { return pixels_per_meter; }

    //This corresponds to the absolute position of the origin (0,0) of the world's metric system
    float origin_x, origin_y;


private:
    int pixels_per_meter;
};


#endif //UI_FRAMEWORK_METRICSCAMERA_H
