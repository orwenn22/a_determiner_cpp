#include "MetricsCamera.h"

#include "Graphics.h"

MetricsCamera::MetricsCamera(int _origin_x, int _origin_y, float _pixels_per_meter) {
    origin_x = (float)_origin_x;
    origin_y = (float)_origin_y;
    pixels_per_meter = _pixels_per_meter;
}

MetricsCamera::~MetricsCamera() {
    if(Metrics::GetGraphicsCam() == this) Metrics::SetGraphicsCam(nullptr);
}

Vector2 MetricsCamera::ConvertAbsoluteToMeters(int absolute_x, int absolute_y) {
    //Compute the position in pixel from the origin of the cam
    int relative_x = absolute_x - (int)origin_x;
    int relative_y = absolute_y - (int)origin_y;

    //Then divide by zoom level to obtain position in meters
    return {(float)relative_x / (float)pixels_per_meter, (float)relative_y / (float)pixels_per_meter};
}

Vector2 MetricsCamera::ConvertMetersToAbsolute(Vector2 position_meters) {
    //Compute the position in pixel from the origin of the cam
    int relative_x = (int)(position_meters.x * (float)pixels_per_meter);
    int relative_y = (int)(position_meters.y * (float)pixels_per_meter);

    //Then add to origin to obtain the absolute position in pixel
    return {(float)((int)origin_x+relative_x), (float)((int)origin_y+relative_y)};
}

Vector2 MetricsCamera::GetCameraTopLeft() {
    return ConvertAbsoluteToMeters(0, 0);

    //Could also be
    // return {-origin_x / pixels_per_meter, -origin_y / pixels_per_meter};
}

Vector2 MetricsCamera::GetCameraCenter() {
    return ConvertAbsoluteToMeters(GetScreenWidth()/2, GetScreenHeight()/2);
}

void MetricsCamera::SetCameraTopLeft(Vector2 worldpos) {
    origin_x = -MetersToPixels(worldpos.x);
    origin_y = -MetersToPixels(worldpos.y);
}

void MetricsCamera::SetCameraCenter(Vector2 worldpos) {
    //First set the top left of the camera
    SetCameraTopLeft(worldpos);

    //We need to add half of the screen size to set the middle
    origin_x += (float)GetScreenWidth()/2.f;
    origin_y += (float)GetScreenHeight()/2.f;
}

void MetricsCamera::SetPixelsPerMeter(float new_zoom) {
    if(new_zoom == pixels_per_meter || new_zoom <= 0) return;

    Vector2 previous_center = GetCameraCenter();
    pixels_per_meter = new_zoom;
    SetCameraCenter(previous_center);
}
