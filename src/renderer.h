// Projet Session/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxReflectionRefraction.h"
#include "ofxRaycaster.h"

// énumération des différents types de primitives vectorielles
enum class VectorPrimitiveType {none, pixel, point, line, rectangle, polygone, circle, iso_triangle};

enum class MeshPrimitiveType {cube, icosahedron};

// enum for different modes
enum class ToolMode {vector_draw, mesh_draw, model_draw, select, rotate, scale, translate, none};

enum class DrawMode {normal, selected, zone};

enum class ModelType {banana, big_hero_six, heart, soccer};

enum class ConvolutionKernel {identity, emboss, sharpen, blur};

enum class ShaderType {color_fill, lambert, gouraud, phong, blinn_phong, pbr};

enum class MaterialType {rubber, plastic, chrome, brass, none};

// structure de primitive vectorielle générique
struct VectorPrimitive
{
    VectorPrimitiveType                 type;
    float                               position1[2];
    float                               position2[2];
    float                               stroke_width;
    int                                 sides;
    unsigned char                       stroke_color[4];
    unsigned char                       fill_color[4];
    ofMatrix4x4                         transformation_matrix;
};

struct MeshPrimitive
{
    MeshPrimitiveType                     type;
    float                               position1[3];
    float                               position2[3];
    float                               stroke_width;
    int                                 sides;
    unsigned char                       stroke_color[4];
    unsigned char                       fill_color[4];
};

struct Model
{
    ofxAssimpModelLoader                model;
    float                               position1[3];
    float                               position2[3];
    std::vector<ofPath>                 box;
    ofTexture                           texture;
    string                              name;
};

struct UndoStruct {
    std::vector<int>    indexes;
    ofMatrix4x4         matrix;
};

class Renderer
{
public:
    
    vector<ofImage> pos;
    vector<ofImage> neg;
    vector<ofxReflectionRefraction> Ref;
    ofMesh sphere;
    ofMesh box;
    
    ofxraycaster::Ray ray;
    ofxraycaster::Ray2D ray_2d;
    glm::vec3 ray_origin;
    glm::vec3 ray_direction;
    bool show_ray;
    bool show_ray_2d;
    
    int tessLevelInner;
    int tessLevelOuter;
    bool activate_tessellation;
    
    const int catmull_rom_max_pts = 20;
    float catmull_rom_pts [40];
    int catmull_nb_pts;
    bool show_catmull_curve;
    bool show_catmull_controls;
    int catmull_line_resolution;
    int catmull_line_width_outline;
    int catmull_line_width_curve;
    float catmull_alpha;
    
    ofPolyline catmull_renderer;
    ofVec3f catmull_position;
    
    bool show_coons_patch;
    bool show_coons_controls;
    
    float coons_curve_1 [18] = {89, 175, -250,
                                445, 290, -294,
                                490, 340, -442,
                                654, 505, -500,
                                880, 380, -238,
                                495, 720, -141};
    float coons_curve_2 [18] = {215, 260, -260,
                                304, 500, 31,
                                419, 412, 245,
                                617, 426, 170,
                                528, 405, 255,
                                860, 280, 175};
    float coons_curve_3 [18] = {428, 444, -600,
                                445, 290, -294,
                                260, 334, -300,
                                293, 374, 10,
                                304, 500, 31,
                                260, 500, 290};
    float coons_curve_4 [18] = {642, 175, -600,
                                880, 380, -238,
                                950, 500, -300,
                                517, 541, 10,
                                528, 405, 255,
                                950, 500, 550};
    
    vector<ofVec3f> positions_coons_curve_1;
    vector<ofVec3f> positions_coons_curve_2;
    vector<ofVec3f> positions_coons_curve_3;
    vector<ofVec3f> positions_coons_curve_4;
    
    ofPolyline coons_curve_1_renderer;
    ofPolyline coons_curve_2_renderer;
    ofPolyline coons_curve_3_renderer;
    ofPolyline coons_curve_4_renderer;
    
    ofVboMesh coons_mesh;
    
    ofMaterial material_brass;
    ofMaterial material_chrome;
    ofMaterial material_plastic;
    ofMaterial material_rubber;
    MaterialType material_current;
    
    bool material_shader;
    
    ofShader shader_pbr;
    
    ofColor material_color_ambient;
    ofColor material_color_diffuse;
    ofColor material_color_specular;
    
    float material_metallic;
    float material_roughness;
    float material_occlusion;
    float material_brightness;
    
    glm::vec3 material_fresnel_ior;
    
    ofImage texture_diffuse;
    ofImage texture_metallic;
    ofImage texture_roughness;
    ofImage texture_occlusion;
    
    ofLight light_pbr;
    ofColor light_color_pbr;
    float light_intensity;
    
    float tone_mapping_exposure;
    float tone_mapping_gamma;
    bool tone_mapping_toggle;
    
    ofEasyCam cam;

    ofColor global_ambient_color;
    
    ofColor ambient_light_color;
    float ambient_light_x;
    float ambient_light_y;
    float ambient_light_z;
    
    bool bool_directional_light;
    ofColor directional_light_color;
    float directional_light_x;
    float directional_light_y;
    float directional_light_z;
    float directional_light_orientation_x;
    float directional_light_orientation_y;
    float directional_light_orientation_z;
    
    bool bool_point_light;
    ofColor point_light_color;
    float point_light_x;
    float point_light_y;
    float point_light_z;
    
    bool bool_spot_light;
    ofColor spot_light_color;
    float spot_light_x;
    float spot_light_y;
    float spot_light_z;
    float spot_light_orientation_x;
    float spot_light_orientation_y;
    float spot_light_orientation_z;
    float spot_light_cutoff;
    
    const float pi = 3.1416;
    ofImage in;
    ofImage out;
    bool show_image;
    bool show_histo;
    bool show_cubemap;
    
    bool show_cam;
    bool ortho_mode;
    
    float image_ratio;
    float image_height;
    float image_width;
    ofVec2f image_position;
    
    ConvolutionKernel kernel_type;
    
    ofLight ambient_light;
    ofLight directional_light;
    ofLight point_light;
    ofLight spot_light;
    
    bool gui_show;
    ofPoint gui_position;
    float gui_width;
    float gui_height;
    
    VectorPrimitiveType draw_mode;
    MeshPrimitiveType draw_mode_3d;
    ModelType model_mode;
    
    ofPolyRenderMode render_type;
    ToolMode current_mode;
    
    ofxAssimpModelLoader heart;
    ofxAssimpModelLoader soccer;
    ofxAssimpModelLoader banana;
    ofxAssimpModelLoader big_hero_six;
    
    ofImage banana_tex_img;
    ofTexture banana_texture;
    
    bool show_texture;
    
    std::vector<VectorPrimitive> shapes;
    std::vector<int> selected_shapes_index;
    
    std::vector<MeshPrimitive> mesh_vector;
    
    std::vector<Model> model_vector;
    
    bool show_bounding_box;
    
    // Primitives
    int max_count_2d;
    int max_count_3d;
    int max_count_3d_models;
    
    int index;
    int head_2d;
    int head_3d;
    int head_3d_models;
    
    int undo_count;
    std::stack<UndoStruct> undo_stack;
    std::stack<UndoStruct> redo_stack;
    
    // Mouse
    int mouse_press_x;
    int mouse_press_y;
    
    int mouse_current_x;
    int mouse_current_y;
    
    bool is_mouse_button_pressed;
    
    // From application.gui
    ofColor stroke_color;
    ofColor fill_color;
    ofColor background_color;
    ofColor selected_color;
    float stroke_width;
    float radius;
    int polygone_sides;
    
    float rotation_angle;
    bool rotation_x;
    bool rotation_y;
    bool rotation_z;
    float translation_x;
    float translation_y;
    float translation_z;
    float scale_ratio_x;
    float scale_ratio_y;
    float scale_ratio_z;
    
    ShaderType shader_active;
    
    ofShader shader_color_fill;
    ofShader shader_lambert;
    ofShader shader_gouraud;
    ofShader shader_phong;
    ofShader shader_blinn_phong;
    ofShader shader_tessellation;
    
    ofShader* shader;

    
    // System
    void setup();
    void update();
    void draw();
    
    void reset();
    
    void image_export() const;
    
    // Functions
    void add_vector_shape(VectorPrimitiveType type);
    ofPath create_path(int index, DrawMode mode);
    
    void add_mesh(MeshPrimitiveType type);
    ofVboMesh create_3d_mesh(int index, DrawMode mode);
    
    void add_3d_model(ModelType model);
    
    std::vector<ofPath> get_bounding_box(ofMesh &mesh);
    std::vector<ofPath> get_bounding_box(ofxAssimpModelLoader &model);
    // Mouse
    void draw_zone(float x1, float y1, float x2, float y2) const;
    void draw_cursor(float x, float y) const;
    
    // Selection
    void select_single(float x, float y);
    void select_multiple(float x1, float y1, float x2, float y2);
    
    // Transformations
    std::tuple<float, float> rotate_pair(float x, float y, float rx, float ry, float angle);
    std::tuple<float, float> find_center_selection();
    void apply_rotation();
    void apply_scale();
    void apply_translation();
    
    float find_distance(float x1, float y1, float x2, float y2);
    
    void undo_transformation();
    void redo_transformation();
    
    void delete_shape();
    
    void apply_filter();
    
    float oscillation_amplitude;
    float oscillation_frequency;
    float oscillate(float time, float frequency, float amplitude);
    
    ~Renderer();
};

const std::array<float, 9> convolution_kernel_identity
{
    0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f
};


const std::array<float, 9> convolution_kernel_sharpen
{
    0.0f, -1.0f,  0.0f,
    -1.0f,  5.0f, -1.0f,
    0.0f, -1.0f,  0.0f
};

const std::array<float, 9> convolution_kernel_emboss
{
    -2.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  2.0f
};

const std::array<float, 9> convolution_kernel_blur
{
    1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
    1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
    1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

inline void catmull_rom(int resolution, float alpha,
                        float p0x, float p0y, float p0z,
                        float p1x, float p1y, float p1z,
                        float p2x, float p2y, float p2z,
                        float p3x, float p3y, float p3z,
                        vector<ofVec3f>& positions) {
//    float alpha = 1.0f;
    float t0 = 0.0f;
    float t1 = pow(sqrt(pow(p1x - p0x,2) + pow(p1y - p0y, 2)), alpha) + t0;
    float t2 = pow(sqrt(pow(p2x - p1x,2) + pow(p2y - p1y, 2)), alpha) + t1;
    float t3 = pow(sqrt(pow(p3x - p2x,2) + pow(p3y - p2y, 2)), alpha) + t2;
    
    for (float t = t1; t < t2 ; t += ((t2-t1)/resolution)) {
    
        float a1x = (t1 - t)/(t1-t0)*p0x + (t-t0)/(t1-t0)*p1x;
        float a1y = (t1 - t)/(t1-t0)*p0y + (t-t0)/(t1-t0)*p1y;
        float a1z = (t1 - t)/(t1-t0)*p0z + (t-t0)/(t1-t0)*p1z;
        float a2x = (t2 - t)/(t2-t1)*p1x + (t-t1)/(t2-t1)*p2x;
        float a2y = (t2 - t)/(t2-t1)*p1y + (t-t1)/(t2-t1)*p2y;
        float a2z = (t2 - t)/(t2-t1)*p1z + (t-t1)/(t2-t1)*p2z;
        float a3x = (t3 - t)/(t3-t2)*p2x + (t-t2)/(t3-t2)*p3x;
        float a3y = (t3 - t)/(t3-t2)*p2y + (t-t2)/(t3-t2)*p3y;
        float a3z = (t3 - t)/(t3-t2)*p2z + (t-t2)/(t3-t2)*p3z;
        
        float b1x = (t2 - t)/(t2 - t0)*a1x + (t - t0)/(t2 - t0)*a2x;
        float b1y = (t2 - t)/(t2 - t0)*a1y + (t - t0)/(t2 - t0)*a2y;
        float b1z = (t2 - t)/(t2 - t0)*a1z + (t - t0)/(t2 - t0)*a2z;
        float b2x = (t3 - t)/(t3 - t1)*a2x + (t - t1)/(t3 - t1)*a3x;
        float b2y = (t3 - t)/(t3 - t1)*a2y + (t - t1)/(t3 - t1)*a3y;
        float b2z = (t3 - t)/(t3 - t1)*a2z + (t - t1)/(t3 - t1)*a3z;
        
        float x = (t2 - t)/(t2 - t1)*b1x + (t - t1)/(t2-t1)*b2x;
        float y = (t2 - t)/(t2 - t1)*b1y + (t - t1)/(t2-t1)*b2y;
        float z = (t2 - t)/(t2 - t1)*b1z + (t - t1)/(t2-t1)*b2z;
        positions.push_back(ofVec3f(x, y, z));
    }
};

inline void coons_patch(float resolution,
                        const vector<ofVec3f>& positions_curve_1,
                        const vector<ofVec3f>& positions_curve_2,
                        const vector<ofVec3f>& positions_curve_3,
                        const vector<ofVec3f>& positions_curve_4,
                        vector<ofVec3f>& positions) {
    resolution = resolution * 3;
    for (float u = 0 ; u <= 1; u += 1.0/(resolution+2)){
        for (float v = 0 ; v <= 1; v+= 1.0/(resolution + 2)) {
            ofVec3f lerp_u = (1-v)*positions_curve_1[u*(resolution+2)] + v*positions_curve_2[u*(resolution+2)];
            ofVec3f lerp_v = (1-u)*positions_curve_3[v*(resolution+2)] + u*positions_curve_4[v*(resolution+2)];
            ofVec3f blerp_uv = (1-u)*(1-v)*positions_curve_1[0] + u*(1-v)*positions_curve_4[0] +(1-u)*v*positions_curve_2[0] + u*v*positions_curve_2[positions_curve_2.size()-1];
            positions.push_back(lerp_u + lerp_v - blerp_uv);
        }
    }
};
