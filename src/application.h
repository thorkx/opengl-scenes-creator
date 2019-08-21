// Projet Session/application.h
// Classe principale de l'application.

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGrafica.h"
#include "ofxRaycaster.h"

#include "renderer.h"

class Application : public ofBaseApp
{
public:
    
    Renderer renderer;
    
    int max_catmull_pts;
    
    // Mouse
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void keyReleased(int key);
    void dragEvent(ofDragInfo info);
    
    float mouse_tolerance;
    
    int mouse_offset_x;
    int mouse_offset_y;
        
    // GUI
    ofxPanel gui;
    ofxPanel gui2;
    ofxPanel gui3;
    
    ofxGuiGroup group_draw;
    ofxGuiGroup group_3d_draw;
    ofxGuiGroup group_3d_model;
    ofxGuiGroup group_shapes;
    ofxGuiGroup group_tools;
    ofxGuiGroup group_lights;
    
    ofxGuiGroup group_catmull;
    ofxGuiGroup group_coons;
    
    ofxGuiGroup group_ray;
    ofParameter<float> slider_position_ray_x;
    ofParameter<float> slider_position_ray_y;
    ofParameter<float> slider_position_ray_z;
    ofParameter<float> slider_direction_ray_x;
    ofParameter<float> slider_direction_ray_y;
    ofParameter<float> slider_direction_ray_z;
    ofParameter<bool> button_show_ray;
    ofParameter<bool> button_show_ray_2d;
    void button_ray_pressed(bool& bool_val);
    void button_ray_2d_pressed(bool& bool_val);
    
    int current_catmull_pt;
    ofParameter<int> slider_nb_catmull_pts;
    ofEventListener slider_active_pt_listener;
    ofEventListener slider_nb_pts_listener;
    ofEventListener slider_catmull_position_x_listener;
    ofEventListener slider_catmull_position_y_listener;
    ofEventListener slider_catmull_alpha_listener;
    ofParameter<int> slider_active_catmull_pt;
    ofParameter<float> slider_position_catmull_x;
    ofParameter<float> slider_position_catmull_y;
    ofParameter<float> slider_catmull_alpha;
    ofParameter<bool> button_show_catmull_curve;
    ofParameter<bool> button_show_catmull_controls;
    
    void slider_position_catmull_x_changed();
    void slider_position_catmull_y_changed();
    void slider_nb_catmull_pts_changed();
    void slider_catmull_alpha_changed();
    void slider_catmull_changed();
    void button_show_catmull_curve_pressed(bool& bool_val);
    void button_show_catmull_controls_pressed(bool& bool_val);
    
    int current_coons_pt;
    int current_coons_curve;
    ofParameter<int> slider_active_coons_curve;
    ofParameter<int> slider_active_coons_pt;
    ofParameter<float> slider_coons_position_x;
    ofParameter<float> slider_coons_position_y;
    ofParameter<float> slider_coons_position_z;
    ofParameter<bool> button_show_coons_patch;
    ofParameter<bool> button_show_coons_controls;
    
    ofEventListener slider_active_coons_curve_listener;
    ofEventListener slider_active_coons_pt_listener;
    ofEventListener slider_coons_position_x_listener;
    ofEventListener slider_coons_position_y_listener;
    ofEventListener slider_coons_position_z_listener;

    
    void slider_position_coons_x_changed();
    void slider_position_coons_y_changed();
    void slider_position_coons_z_changed();
    
    void slider_coons_active_pt_changed();
    
    void slider_coons_active_curve_changed();

    void button_show_coons_patch_pressed(bool& bool_val);
    void button_show_coons_controls_pressed(bool& bool_val);
    
    
    ofxGuiGroup group_transform;
    ofxGuiGroup group_rotate;
    ofxGuiGroup group_scale;
    ofxGuiGroup group_translate;
    
    ofxGuiGroup group_ambient_light;
    ofxGuiGroup group_directional_light;
    ofxGuiGroup group_point_light;
    ofxGuiGroup group_spot_light;
    
    ofParameter<bool> bool_ambient_light;
    ofParameter<bool> bool_directional_light;
    ofParameter<bool> bool_point_light;
    ofParameter<bool> bool_spot_light;

    ofParameter<ofColor> color_picker_ambient;
    ofParameter<ofColor> color_picker_directional;
    ofParameter<ofColor> color_picker_point;
    ofParameter<ofColor> color_picker_spot;
    
    ofParameter<float> slider_position_ambient_x;
    ofParameter<float> slider_position_ambient_y;
    ofParameter<float> slider_position_ambient_z;
    
    ofParameter<float> slider_position_directional_x;
    ofParameter<float> slider_position_directional_y;
    ofParameter<float> slider_position_directional_z;
    ofParameter<float> slider_orientation_directional_x;
    ofParameter<float> slider_orientation_directional_y;
    ofParameter<float> slider_orientation_directional_z;
    
    ofParameter<float> slider_position_point_x;
    ofParameter<float> slider_position_point_y;
    ofParameter<float> slider_position_point_z;
    
    ofParameter<float> slider_position_spot_x;
    ofParameter<float> slider_position_spot_y;
    ofParameter<float> slider_position_spot_z;
    ofParameter<float> slider_orientation_spot_x;
    ofParameter<float> slider_orientation_spot_y;
    ofParameter<float> slider_orientation_spot_z;
    
    ofParameter<float> slider_spot_cutoff;

    ofxButton button_directional_ok;
    ofxButton button_point_ok;
    ofxButton button_spot_ok;
    
    std::vector<ofxGuiGroup*> group_vector;
    
    ofParameter<float> slider_rotation_angle;
    ofParameter<float> slider_translate_x;
    ofParameter<float> slider_translate_y;
    ofParameter<float> slider_translate_z;
    ofParameter<float> slider_scale_x;
    ofParameter<float> slider_scale_y;
    ofParameter<float> slider_scale_z;
    
    ofxButton button_rotate_ok;
    ofxButton button_translate_ok;
    ofxButton button_scale_ok;
    
    ofParameter<ofColor> color_picker_background;
    ofParameter<ofColor> color_picker_stroke;
    ofParameter<ofColor> color_picker_fill;
    
    ofParameter<float> slider_stroke_weight;
    
    ofParameter<bool> gui_checkbox;
    ofParameter<bool> grid_checkbox;
    ofParameter<bool> img_checkbox;
    ofParameter<bool> histo_checkbox;
    ofParameter<bool> light_checkbox;
    
    ofParameter<bool> bool_iso;
    ofParameter<bool> bool_line;
    ofParameter<bool> bool_rectangle;
    ofParameter<bool> bool_circle;
    ofParameter<bool> bool_polygone;
    ofParameter<int> slider_polygone;
    
    ofParameter<bool> bool_cube;
    ofParameter<bool> bool_icosahedron;
    
    ofParameter<bool> bool_heart;
    ofParameter<bool> bool_big;
    ofParameter<bool> bool_banana;
    ofParameter<bool> bool_soccer;
    
    ofParameter<bool> bool_select;
    ofParameter<bool> bool_2D_draw;
    ofParameter<bool> bool_3D_draw;
    ofParameter<bool> bool_3D_model;
    ofParameter<bool> bool_rotate;
    ofParameter<bool> bool_scale;
    ofParameter<bool> bool_translate;
    
    std::vector<ofParameter<bool>> bool_vector;
    
    ofParameter<bool> activate_cam;
    
    void button_draw_pressed(bool& bool_val);
    void button_select_pressed(bool& bool_val);
    void button_rotate_pressed(bool& bool_val);
    void button_scale_pressed(bool& bool_val);
    void button_translate_pressed(bool& bool_val);
    
    void button_iso_pressed(bool& bool_val);
    void button_line_pressed(bool& bool_val);
    void button_circle_pressed(bool& bool_val);
    void button_rectangle_pressed(bool& bool_val);
    void button_polygone_pressed(bool& bool_val);
    
    void button_draw_3d_pressed(bool& bool_val);
    void button_3d_model_pressed(bool& bool_val);

    void button_cube_pressed(bool& bool_val);
    void button_icosahedron_pressed(bool& bool_val);
    void button_heart_pressed(bool& bool_val);
    void button_banana_pressed(bool& bool_val);
    void button_big_pressed(bool& bool_val);
    void button_soccer_pressed(bool& bool_val);
    
    void button_rotate_ok_pressed();
    void button_translate_ok_pressed();
    void button_scale_ok_pressed();
    
    void button_directional_pressed();
    void button_point_pressed();
    void button_spot_pressed();
    
    // System
    void setup();
    
    void draw();
    void update();

    void exit();
    
    ofxGPlot histogram;
    vector<ofxGPoint> redHistPoints, greenHistPoints, blueHistPoints;
    ofRectangle histo_rect;
    void calculateHistograms();
    void draw_histogram();
};
