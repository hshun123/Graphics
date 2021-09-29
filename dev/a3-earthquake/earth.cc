/** CSci-4611 Assignment 3:  Earthquake
* modified by Eric Hwang
 */

#include "earth.h"
#include "config.h"

#include <vector>

 // for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


const int nslices = 20;
const int nstacks = 15;

Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string>& search_path) {
    // init shader program
    shader_.Init();

    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.
    
    // Eric
    std::vector<unsigned int> indices;
    std::vector<Point2> tex_coords;

    for (int y = 0; y <= nstacks; y++) {
        for (int x = 0; x <= nslices; x++) {
            // convert to (x,y,z) point
            // x -pi to pi, y -pi/2 to pi/2
            Point3 convert = LatLongToPlane(y * 180 / nstacks - 90, x * 360 / nslices - 180); 
            vertices.push_back(convert);

            // vertices.push_back(Point3((float)((float)y / nslices * 2 * M_PI - M_PI), (float)((float)j / nstacks * M_PI - M_PI_2), 0));

            // text coordinate
            tex_coords.push_back(Point2((float)x / nslices, 1.0 - (float)y / nstacks));

            if (y != nstacks && x != nslices) {
                // first triangle
                indices.push_back(x + y * nslices + y);
                indices.push_back(x + y * nslices + y + 1);
                indices.push_back(x + y * nslices + y + nslices + 1);
               
                // second triangle
                indices.push_back(x + y * nslices + y + nslices + 1);
                indices.push_back(x + y * nslices + y + 1);
                indices.push_back(x + y * nslices + y + nslices + 2);
                
            }
        }
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    //----
    earth_mesh_.SetTexCoords(0, tex_coords);
    //----
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4& model_matrix, const Matrix4& view_matrix, const Matrix4& proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10, 10, 10);
    light.ambient_intensity = Color(1, 1, 1);
    light.diffuse_intensity = Color(1, 1, 1);
    light.specular_intensity = Color(1, 1, 1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}

void Earth::Updae_global() {
    
    std::vector<Point3> vertices_sphere;
    std::vector<Vector3> normals;

    float latitude, longitude;
    
    // globebtn is pressed
    globe_btn = !globe_btn;

    for (int y = 0; y <= nstacks; y++) {
        for (int x = 0; x <= nslices; x++) {
            latitude = y * 180 / nstacks - 90;
            longitude = x * 360 / nslices - 180;

            if (globe_btn) {
                vertices_sphere.push_back(LatLongToSphere(latitude, longitude));
                // cartesian coordinates for normal
                Vector3 normal = Vector3(cos(GfxMath::ToRadians(latitude)) * sin(GfxMath::ToRadians(longitude)), 
                    sin(GfxMath::ToRadians(latitude)), cos(GfxMath::ToRadians(latitude)) * cos(GfxMath::ToRadians(longitude)));

                normals.push_back(normal);
            }
            else {
                vertices_sphere.push_back(LatLongToPlane(latitude, longitude));
            }
        }
     }
  
    earth_mesh_.SetVertices(vertices_sphere);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
    //return Point3(0,0,0);

    // USE GIVEN FORMULA
    return Point3(cos(GfxMath::ToRadians(latitude)) * sin(GfxMath::ToRadians(longitude)), sin(GfxMath::ToRadians(latitude)),
        cos(GfxMath::ToRadians(latitude)) * cos(GfxMath::ToRadians(longitude)));
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    // return Point3(0,0,0);

    // convert lat/lon to radians value
    return Point3(GfxMath::ToRadians(longitude), GfxMath::ToRadians(latitude), 0);
}



void Earth::DrawDebugInfo(const Matrix4& model_matrix, const Matrix4& view_matrix, const Matrix4& proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t = 0; t < earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_verticesReadOnly(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertexReadOnly(indices[0]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[1]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1, 1, 0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

