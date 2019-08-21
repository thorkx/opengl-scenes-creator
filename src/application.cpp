// Projet Session/application.cpp
// Classe principale de l'application.

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup()
{
    ofSetWindowTitle("Projet de session");
    
    ofLog() << "<app::setup>";
    
    renderer.setup();
    
    max_catmull_pts = 20;
    
    histo_rect.set(renderer.image_position, 20, 20);
    
    calculateHistograms();
    
    // Color histogram setup
    histogram.setPos(ofGetWindowWidth() - 25.0f - 300, ofGetWindowHeight() - 25.0f - 180);
    histogram.setDim(200, 120);
    histogram.setXLim(0, 124);
    histogram.setYLim(0, 50);
    histogram.setTitleText("Color histogram");

    histogram.setPoints(redHistPoints);
    histogram.getMainLayer().setLineColor({ ofColor(255, 0, 0, 75) });
    
    histogram.addLayer("layer g", greenHistPoints);
    histogram.getLayer("layer g").setLineColor({ ofColor(0, 255, 0, 75) });
    
    histogram.addLayer("layer b", blueHistPoints);
    histogram.getLayer("layer b").setLineColor({ ofColor(0, 0, 255, 75) });
    
    histogram.activateReset();
    
    mouse_tolerance = 5;
    
    // Initialisation GUI
    gui.setup("UI");
    gui2.setup("UI+");
    gui2.setPosition(gui.getPosition().x + gui.getWidth() + 10, gui.getPosition().y);
    
    // Tools
    group_tools.setup("Tools");
    
    bool_2D_draw.setName("2D Draw");
    bool_3D_draw.setName("3D Draw");
    bool_3D_model.setName("3D Models");
    bool_select.setName("Selection");
    bool_rotate.setName("Rotate");
    bool_scale.setName("Scale");
    bool_translate.setName("Translate");
    
    bool_vector.push_back(bool_2D_draw);
    bool_vector.push_back(bool_3D_draw);
    bool_vector.push_back(bool_3D_model);
    bool_vector.push_back(bool_select);
    bool_vector.push_back(bool_rotate);
    bool_vector.push_back(bool_scale);
    bool_vector.push_back(bool_translate);
    
    bool_2D_draw.addListener(this, &Application::button_draw_pressed);
    bool_3D_draw.addListener(this, &Application::button_draw_3d_pressed);
    bool_3D_model.addListener(this, &Application::button_3d_model_pressed);
    bool_select.addListener(this, &Application::button_select_pressed);
    bool_rotate.addListener(this, &Application::button_rotate_pressed);
    bool_scale.addListener(this, &Application::button_scale_pressed);
    bool_translate.addListener(this, &Application::button_translate_pressed);
    
    bool_2D_draw = true;
    
    // Draw tools
    group_draw.setup("Draw tools");
    
    color_picker_background.set("Background Color", ofColor(31), ofColor(0, 0), ofColor(255, 255));
    color_picker_stroke.set("Stroke Color", (int) ofRandom(0, 256));
    color_picker_fill.set("Fill Color", (int) ofRandom(0, 256));
    
    slider_stroke_weight.set("Border Width", 4.0f, 0.0f, 10.0f);
    
    group_draw.add(color_picker_background);
    group_draw.add(color_picker_stroke);
    group_draw.add(color_picker_fill);
    group_draw.add(slider_stroke_weight);
    
    // Vectorial shapes
    bool_line.setName("Line");
    bool_line.addListener(this, &Application::button_line_pressed);
    bool_rectangle.setName("Rectangle");
    bool_rectangle.addListener(this, &Application::button_rectangle_pressed);
    bool_iso.setName("Isoscele triangle");
    bool_iso.addListener(this, &Application::button_iso_pressed);
    bool_circle.setName("Circle");
    bool_circle.addListener(this, &Application::button_circle_pressed);
    bool_polygone.setName("Regular Polygon");
    bool_polygone.addListener(this, &Application::button_polygone_pressed);
    slider_polygone.set("Polygon sides", 3, 3, 20);
    
    bool_vector.push_back(bool_line);
    bool_vector.push_back(bool_rectangle);
    bool_vector.push_back(bool_iso);
    bool_vector.push_back(bool_circle);
    bool_vector.push_back(bool_polygone);
    
    group_shapes.setup("Shapes");
    group_shapes.add(bool_line);
    group_shapes.add(bool_iso);
    group_shapes.add(bool_rectangle);
    group_shapes.add(bool_circle);
    group_shapes.add(bool_polygone);
    group_shapes.add(slider_polygone);
    
    bool_rectangle = true;
    
    // 3D
    
    bool_cube.setName("Cube");
    bool_cube.addListener(this, &Application::button_cube_pressed);
    
    bool_icosahedron.setName("Icosahedron");
    bool_icosahedron.addListener(this, &Application::button_icosahedron_pressed);
    
    group_3d_draw.setup("3D shapes");
    group_3d_draw.add(bool_cube);
    group_3d_draw.add(bool_icosahedron);
    
    bool_vector.push_back(bool_cube);
    bool_vector.push_back(bool_icosahedron);
    
    // 3D model
    
    bool_heart.setName("Heart");
    bool_heart.addListener(this, &Application::button_heart_pressed);
    bool_big.setName("Baymax");
    bool_big.addListener(this, &Application::button_big_pressed);
    bool_banana.setName("Banana");
    bool_banana.addListener(this, &Application::button_banana_pressed);
    bool_soccer.setName("Soccer");
    bool_soccer.addListener(this, &Application::button_soccer_pressed);
    
    group_3d_model.setup("3D models");
    group_3d_model.add(bool_heart);
    group_3d_model.add(bool_big);
    group_3d_model.add(bool_banana);
    group_3d_model.add(bool_soccer);
    
    bool_vector.push_back(bool_heart);
    bool_vector.push_back(bool_big);
    bool_vector.push_back(bool_banana);
    bool_vector.push_back(bool_soccer);

    // Transformation tools
    group_transform.setup("Transformation tools");
    group_rotate.setup("Rotate");
    group_scale.setup("Scale");
    group_translate.setup("Translate");
    
    slider_rotation_angle.set("Rotation degree (deg)", 0.0f, -180.0f, 180.0f);

    button_rotate_ok.setup("OK");
    button_rotate_ok.addListener(this, &Application::button_rotate_ok_pressed);

    slider_scale_x.set("Scale X", 1.0f, 0.01f, 2.0);
    slider_scale_y.set("Scale Y", 1.0f, 0.01f, 2.0);
    slider_scale_z.set("Scale Z", 1.0f, 0.01f, 2.0);
    
    button_scale_ok.setup("OK");
    button_scale_ok.addListener(this, &Application::button_scale_ok_pressed);
    
    float max_x = ofGetWidth();
    float max_y = ofGetHeight();
    float max_z = 1000.0f;
    
    slider_translate_x.set("Translation X", 0.0f, -1.0f * max_x, max_x);
    slider_translate_y.set("Translation Y", 0.0f, -1.0f * max_y, max_y);
    slider_translate_z.set("Translation Z", 0.0f, -1.0f * max_y, max_y);
    
    button_translate_ok.setup("OK");
    button_translate_ok.addListener(this, &Application::button_translate_ok_pressed);
    
    // Light tools
    group_lights.setup("Lights");
    
    group_ambient_light.setup("Ambient Light");
    bool_ambient_light.setName("Ambient Light");
    
    color_picker_ambient.set("Light Color", ofColor(127), ofColor(0, 0), ofColor(255, 255));
    
    slider_position_ambient_x.set("Position X", 0.0f, -max_x, max_x);
    slider_position_ambient_y.set("Position Y", 0.0f, -max_y, max_y);
    slider_position_ambient_z.set("Position Z", 200.0f, -max_z, max_z);
    
    group_directional_light.setup("Directional Light");
    bool_directional_light.setName("Directional Light");
    
    color_picker_directional.set("Light Color", ofColor(191), ofColor(0, 0), ofColor(255, 255));
    
    slider_position_directional_x.set("Position X", 0.0f, -max_x, max_x);
    slider_position_directional_y.set("Position Y", 0.0f, -max_y, max_y);
    slider_position_directional_z.set("Position Z", 200.0f, -max_z, max_z);
    slider_orientation_directional_x.set("Orientation X", 0.0f, -max_x, max_x);
    slider_orientation_directional_y.set("Orientation Y", 0.0f, -max_y, max_y);
    slider_orientation_directional_z.set("Orientation Z", 0.0f, -max_z, max_z);
    button_directional_ok.setup("On/Off");
    button_directional_ok.addListener(this, &Application::button_directional_pressed);

    group_point_light.setup("Point Light");
    bool_point_light.setName("Point Light");
    
    color_picker_point.set("Light Color", ofColor(191), ofColor(0, 0), ofColor(255, 255));
    
    slider_position_point_x.set("Position X", 0.0f, -max_x, max_x);
    slider_position_point_y.set("Position Y", 0.0f, -max_y, max_y);
    slider_position_point_z.set("Position Z", 200.0f, -max_z, max_z);
    button_point_ok.setup("On/Off");
    button_point_ok.addListener(this, &Application::button_point_pressed);
    
    group_spot_light.setup("Spot Light");
    bool_spot_light.setName("Spot Light");
    color_picker_spot.set("Spot Color", ofColor(191), ofColor(0, 0), ofColor(255, 255));
    
    slider_position_spot_x.set("Position X", 0.0f, -max_x, max_x);
    slider_position_spot_y.set("Position Y", 0.0f, -max_y, max_y);
    slider_position_spot_z.set("Position Z", 200.0f, -max_z, max_z);
    slider_orientation_spot_x.set("Orientation X", 0.0f, -max_x, max_x);
    slider_orientation_spot_y.set("Orientation Y", 0.0f, -max_y, max_y);
    slider_orientation_spot_z.set("Orientation Z", 0.0f, -max_z, max_z);
    slider_spot_cutoff.set("Cutoff", 500.0f, 0.0f, max_z);
    button_spot_ok.setup("On/Off");
    button_spot_ok.addListener(this, &Application::button_spot_pressed);
    
    // Catmull-Rom Curve

    group_catmull.setup("Catmull-Rom Curve");
    slider_nb_catmull_pts.set("Nb of points", 4, 4,max_catmull_pts);
    slider_active_catmull_pt.set("Point to modify", 1, 1, 4);
    
    slider_nb_pts_listener = slider_nb_catmull_pts.newListener([&](int){this->slider_nb_catmull_pts_changed();});
    slider_active_pt_listener = slider_active_catmull_pt.newListener([&](float){this->slider_catmull_changed();});
    
    slider_catmull_position_x_listener = slider_position_catmull_x.newListener([&](float){this->slider_position_catmull_x_changed();});
    slider_catmull_position_y_listener = slider_position_catmull_y.newListener([&](float){this->slider_position_catmull_y_changed();});
    
    slider_position_catmull_x.set("Position X", ofGetWidth()/2, 0, ofGetWidth());
    slider_position_catmull_y.set("Position Y", ofGetHeight()/2.0, 0, ofGetHeight());
    
    slider_catmull_alpha.set("Alpha", 0.0f, 0.0f, 1.0f);
    slider_catmull_alpha_listener = slider_catmull_alpha.newListener([&](float){this->slider_catmull_alpha_changed();});

    button_show_catmull_curve.setName("Show curve");
    button_show_catmull_curve.addListener(this, &Application::button_show_catmull_curve_pressed);
    
    button_show_catmull_controls.setName("Show control points");
    button_show_catmull_controls.addListener(this, &Application::button_show_catmull_controls_pressed);
    
    // Coons patch
    current_coons_pt = 1;
    current_coons_curve = 1;
    
    group_coons.setup("Coons Patch");
    slider_active_coons_curve.set("Curve to modify", 1, 1, 4);
    slider_active_coons_curve_listener = slider_active_coons_curve.newListener([&](int){this->slider_coons_active_curve_changed();});
    
    slider_active_coons_pt.set("Point to modify", 1, 1, 6);
    slider_active_coons_pt_listener = slider_active_coons_pt.newListener([&](int){this->slider_coons_active_pt_changed();});
    
    slider_coons_position_x.set("Position X", ofGetWidth()/2.0, 0, ofGetWidth());
    slider_coons_position_y.set("Position Y", ofGetHeight()/2.0, 0, ofGetHeight());
    slider_coons_position_z.set("Position Z", 0, -500, 500);
    
    slider_coons_position_x_listener = slider_coons_position_x.newListener([&](float){this->slider_position_coons_x_changed();});
    slider_coons_position_y_listener = slider_coons_position_y.newListener([&](float){this->slider_position_coons_y_changed();});
    slider_coons_position_z_listener = slider_coons_position_z.newListener([&](float){this->slider_position_coons_z_changed();});
    
    button_show_coons_patch.setName("Show patch");
    button_show_coons_patch.addListener(this, &Application::button_show_coons_patch_pressed);
    
    button_show_coons_controls.setName("Show curves");
    button_show_coons_controls.addListener(this, &Application::button_show_coons_controls_pressed);
    
    // Ray
    group_ray.setup("Ray casting");
    slider_position_ray_x.set("Position X", ofGetWidth()/2.0, 0, ofGetWidth());
    slider_position_ray_y.set("Position Y", ofGetHeight()/2.0, 0, ofGetHeight());
    slider_position_ray_z.set("Position Z", 0, -500, 500);
    slider_direction_ray_x.set("Direction X", 1, -1, 1);
    slider_direction_ray_y.set("Direction Y", 1, -1, 1);
    slider_direction_ray_z.set("Direction Z", 0, -1, 1);
    button_show_ray.setName("Show ray");
    button_show_ray.addListener(this, &Application::button_ray_pressed);
    button_show_ray_2d.setName("Show 2D ray");
    button_show_ray_2d.addListener(this, &Application::button_ray_2d_pressed);
    
    // Adding groups to gui
    group_tools.add(bool_select);
    group_tools.add(bool_rotate);
    group_tools.add(bool_scale);
    group_tools.add(bool_translate);
    group_tools.add(bool_2D_draw);
    group_tools.add(bool_3D_draw);
    group_tools.add(bool_3D_model);
    
    group_catmull.add(slider_nb_catmull_pts);
    group_catmull.add(slider_active_catmull_pt);
    group_catmull.add(slider_position_catmull_x);
    group_catmull.add(slider_position_catmull_y);
    group_catmull.add(slider_catmull_alpha);
    group_catmull.add(button_show_catmull_curve);
    group_catmull.add(button_show_catmull_controls);
    
    group_coons.add(slider_active_coons_curve);
    group_coons.add(slider_active_coons_pt);
    group_coons.add(slider_coons_position_x);
    group_coons.add(slider_coons_position_y);
    group_coons.add(slider_coons_position_z);
    group_coons.add(button_show_coons_patch);
    group_coons.add(button_show_coons_controls);
    
    group_ray.add(slider_position_ray_x);
    group_ray.add(slider_position_ray_y);
    group_ray.add(slider_position_ray_z);
    group_ray.add(slider_direction_ray_x);
    group_ray.add(slider_direction_ray_y);
    group_ray.add(slider_direction_ray_z);
    group_ray.add(button_show_ray);
    group_ray.add(button_show_ray_2d);
    
    group_ambient_light.add(color_picker_ambient);
    group_ambient_light.add(slider_position_ambient_x);
    group_ambient_light.add(slider_position_ambient_y);
    group_ambient_light.add(slider_position_ambient_z);
    group_lights.add(&group_ambient_light);
    
    group_directional_light.add(color_picker_directional);
    group_directional_light.add(slider_position_directional_x);
    group_directional_light.add(slider_position_directional_y);
    group_directional_light.add(slider_position_directional_z);
    group_directional_light.add(slider_orientation_directional_x);
    group_directional_light.add(slider_orientation_directional_y);
    group_directional_light.add(slider_orientation_directional_z);
    group_directional_light.add(&button_directional_ok);
    group_lights.add(&group_directional_light);
    
    group_point_light.add(color_picker_point);
    group_point_light.add(slider_position_point_x);
    group_point_light.add(slider_position_point_y);
    group_point_light.add(slider_position_point_z);
    group_point_light.add(&button_point_ok);
    group_lights.add(&group_point_light);
    
    group_spot_light.add(color_picker_spot);
    group_spot_light.add(slider_position_spot_x);
    group_spot_light.add(slider_position_spot_y);
    group_spot_light.add(slider_position_spot_z);
    group_spot_light.add(slider_orientation_spot_x);
    group_spot_light.add(slider_orientation_spot_y);
    group_spot_light.add(slider_orientation_spot_z);
    group_spot_light.add(slider_spot_cutoff);
    group_spot_light.add(&button_spot_ok);
    group_lights.add(&group_spot_light);
    
    group_draw.add(&group_shapes);
    
        group_rotate.add(slider_rotation_angle);
        group_rotate.add(&button_rotate_ok);
    group_transform.add(&group_rotate);
    
        group_scale.add(slider_scale_x);
        group_scale.add(slider_scale_y);
        group_scale.add(slider_scale_z);
        group_scale.add(&button_scale_ok);
    group_transform.add(&group_scale);
    
        group_translate.add(slider_translate_x);
        group_translate.add(slider_translate_y);
        group_translate.add(slider_translate_z);
        group_translate.add(&button_translate_ok);
    group_transform.add(&group_translate);
  
    gui.add(&group_tools);
    gui.add(&group_draw);
    gui.add(&group_transform);
    gui.add(&group_3d_draw);
    gui.add(&group_3d_model);
    gui2.add(&group_catmull);
    gui2.add(&group_coons);
    gui2.add(&group_ray);
    gui2.add(&group_lights);
        
    group_lights.minimize();
    
    group_vector.push_back(&group_tools);
    group_vector.push_back(&group_transform);
    group_vector.push_back(&group_draw);
    group_vector.push_back(&group_scale);
    group_vector.push_back(&group_rotate);
    group_vector.push_back(&group_translate);
    group_vector.push_back(&group_shapes);
    group_vector.push_back(&group_3d_draw);
    group_vector.push_back(&group_3d_model);
    
    // Hide/show grid
    grid_checkbox.setName("Show/Hide grid (g)");
    gui.add(grid_checkbox);
    
    // Hide/show image
    img_checkbox.setName("Show/Hide image (i)");
    gui.add(img_checkbox);
    
    // Hide/show histo
    histo_checkbox.setName("Show/Hide histogram (h)");
    gui.add(histo_checkbox);
    
    // Hide/show control
    gui_checkbox.setName("Show/Hide (u)");
    gui.add(gui_checkbox);
    
    grid_checkbox = false;
    gui_checkbox = true;
    img_checkbox = false;
    histo_checkbox = false;
    light_checkbox = false;
    
    // Cam
    activate_cam = false;
    mouse_offset_x = 0;
    mouse_offset_y = 0;
    
    // init catmull points at center
    for (int i = 0 ; i < max_catmull_pts; i++) {
        renderer.catmull_rom_pts[2*i] = ofGetWidth()/2.0f;
        renderer.catmull_rom_pts[2*i + 1] = ofGetHeight()/2.0f;
    }
        
    current_catmull_pt = 1;
    current_coons_pt = 1;
    current_coons_curve = 1;
}

void Application::update()
{
    if (histo_checkbox && !img_checkbox)
        img_checkbox = true;

    renderer.ray.setOrigin(glm::vec3(slider_position_ray_x, slider_position_ray_y, slider_position_ray_z));
    renderer.ray.setDirection(glm::vec3(slider_direction_ray_x, slider_direction_ray_y, slider_direction_ray_z));
    renderer.ray_2d.setOrigin(glm::vec2(slider_position_ray_x, slider_position_ray_y));
    renderer.ray_2d.setDirection(glm::vec2(slider_direction_ray_x, slider_direction_ray_y));

    renderer.catmull_nb_pts = slider_nb_catmull_pts;
    renderer.catmull_alpha = slider_catmull_alpha;
    
    renderer.ambient_light_color = color_picker_ambient;
    renderer.ambient_light_x = slider_position_ambient_x;
    renderer.ambient_light_y = slider_position_ambient_y;
    renderer.ambient_light_z = slider_position_ambient_z;
    
    renderer.bool_directional_light = bool_directional_light;
    renderer.directional_light_color = color_picker_directional;
    renderer.directional_light_x = slider_position_directional_x;
    renderer.directional_light_y = slider_position_directional_y;
    renderer.directional_light_z = slider_position_directional_z;
    renderer.directional_light_orientation_x = slider_orientation_directional_x;
    renderer.directional_light_orientation_y = slider_orientation_directional_y;
    renderer.directional_light_orientation_z = slider_orientation_directional_z;
    
    renderer.bool_point_light = bool_point_light;
    renderer.point_light_color = color_picker_point;
    renderer.point_light_x = slider_position_point_x;
    renderer.point_light_y = slider_position_point_y;
    renderer.point_light_z = slider_position_point_z;
    
    renderer.bool_spot_light = bool_spot_light;
    renderer.spot_light_color = color_picker_spot;
    renderer.spot_light_x = slider_position_spot_x;
    renderer.spot_light_y = slider_position_spot_y;
    renderer.spot_light_z = slider_position_spot_z;
    renderer.spot_light_orientation_x = slider_orientation_spot_x;
    renderer.spot_light_orientation_y = slider_orientation_spot_y;
    renderer.spot_light_orientation_z = slider_orientation_spot_z;
    renderer.spot_light_cutoff = slider_spot_cutoff;
    
    renderer.background_color = color_picker_background;
    renderer.stroke_color = color_picker_stroke;
    renderer.stroke_width = slider_stroke_weight;
    renderer.fill_color = color_picker_fill;
    
    renderer.show_image = img_checkbox;
    renderer.show_histo = histo_checkbox;
    
    renderer.show_cam = activate_cam;
    
    renderer.polygone_sides = slider_polygone;
    
    renderer.gui_show = gui_checkbox;
    
    renderer.update();
}

void Application::draw()
{
    if (grid_checkbox)
        ofDrawGrid(max(ofGetWidth(), ofGetHeight()) / 40.0f, 40, false, false, false, true);
    
    if (!light_checkbox) {
        ofDisableLighting();
        ofDisableDepthTest();
    } else {
        ofEnableLighting();
        ofEnableDepthTest();
    }
    
    renderer.draw();
    
    if (histo_checkbox) {
        draw_histogram();
        ofPushStyle();
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofDrawRectangle(histo_rect);
        ofPopStyle();
    }

    if (gui_checkbox) {
        gui.draw();
        gui2.draw();
    }


}

void Application::dragEvent(ofDragInfo info) {
    if (info.files.size() > 0) {
        renderer.in.load(info.files[0]);
        renderer.image_ratio = renderer.image_height / renderer.in.getHeight() ;
        renderer.image_width = renderer.in.getWidth() * renderer.image_ratio;
        renderer.in.resize(renderer.image_width, renderer.image_height);
        
        renderer.out.allocate(renderer.image_width, renderer.image_height, OF_IMAGE_COLOR);
        renderer.image_position.x = ofGetWidth() - renderer.out.getWidth() - 5.0f;
        
        renderer.kernel_type = ConvolutionKernel::identity;
        renderer.apply_filter();
    }
}

void Application::mouseMoved(int x, int y)
{
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
}

void Application::mouseDragged(int x, int y, int button)
{
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
    
    if (histo_checkbox) {
        // Keep rectangle in image
        histo_rect.setX(ofClamp(x - histo_rect.getWidth() / 2, renderer.image_position.x, renderer.image_position.x + renderer.out.getWidth() - histo_rect.getWidth()));
        histo_rect.setY(ofClamp(y - histo_rect.getHeight() / 2, renderer.image_position.y + histo_rect.getHeight(), renderer.image_position.y + renderer.out.getHeight()) - histo_rect.getWidth());
  
        // Calculate the color histograms
        calculateHistograms();
        
        // Update the plots
        histogram.activateReset();
        histogram.getMainLayer().setPoints(redHistPoints);
        histogram.getLayer("layer g").setPoints(greenHistPoints);
        histogram.getLayer("layer b").setPoints(blueHistPoints);
    }
}

void Application::mousePressed(int x, int y, int button)
{
    renderer.is_mouse_button_pressed = true;
    
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
    
    renderer.mouse_press_x = x + mouse_offset_x;
    renderer.mouse_press_y = y + mouse_offset_y;
    
    ofLog() << "<app::mouse pressed  at: (" << x << ", " << y << ")>";
}

void Application::mouseReleased(int x, int y, int button)
{
    renderer.is_mouse_button_pressed = false;
    
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
    
    if (histo_checkbox || activate_cam)
        return;
    
    switch (renderer.current_mode){
        case ToolMode::vector_draw:
            renderer.add_vector_shape(renderer.draw_mode);
            break;
        case ToolMode::mesh_draw:
            renderer.add_mesh(renderer.draw_mode_3d);
            break;
        case ToolMode::model_draw:
            renderer.add_3d_model(renderer.model_mode);
            break;
        case ToolMode::select:
            if (std::abs(renderer.mouse_press_x - renderer.mouse_current_x) > mouse_tolerance ||
                std::abs(renderer.mouse_press_y - renderer.mouse_current_y) > mouse_tolerance)
                renderer.select_multiple(renderer.mouse_press_x, renderer.mouse_press_y, renderer.mouse_current_x, renderer.mouse_current_y);
            else
                renderer.select_single(renderer.mouse_press_x, renderer.mouse_press_y);
            break;
        default:
            break;
    }
    
    ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
}

void Application::mouseEntered(int x, int y)
{
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
    
    ofLog() << "<app::mouse entered  at: (" << x << ", " << y << ")>";
}

void Application::mouseExited(int x, int y)
{
    renderer.mouse_current_x = x + mouse_offset_x;
    renderer.mouse_current_y = y + mouse_offset_y;
    
    ofLog() << "<app::mouse exited   at: (" << x << ", " << y << ")>";
}

void Application::keyReleased(int key)
{
    switch (key)
    {
        case 48: // 0 key
            renderer.ortho_mode = !renderer.ortho_mode;
            ofLog() << "<ortho mode: " << renderer.ortho_mode << ">";
            break;
            
        case 49: // 1 key
            renderer.image_export();
            break;
            
        case 55: //7
            bool_directional_light = !bool_directional_light;
            break;
            
        case 56: //8
            bool_point_light = !bool_point_light;
            break;
            
        case 57: //9
            bool_spot_light = !bool_spot_light;
            break;
            
        case 97: // a key
            renderer.activate_tessellation = !renderer.activate_tessellation;
            ofLog() << "<toggle activate tessellation: " << renderer.activate_tessellation << ">";
            break;
        
        case 98: // b key
            renderer.show_bounding_box = !renderer.show_bounding_box;
            ofLog() << "<toggle bounding box: " << renderer.show_bounding_box << ">";
            break;
        
        case 99: // c key
            renderer.show_cubemap = !renderer.show_cubemap;
            ofLog() << "<toggle cubemap: " << renderer.show_cubemap << ">";
            break;
            
        case 102: // f key
            if (renderer.kernel_type == ConvolutionKernel::identity){
                renderer.kernel_type = ConvolutionKernel::emboss;
                ofLog() << "<toggle kernel type: emboss>";
            }
            else if (renderer.kernel_type == ConvolutionKernel::emboss){
                renderer.kernel_type = ConvolutionKernel::sharpen;
                ofLog() << "<toggle kernel type: sharpen>";
            }
            else if (renderer.kernel_type == ConvolutionKernel::sharpen){
                renderer.kernel_type = ConvolutionKernel::blur;
                ofLog() << "<toggle kernel type: blur>";
            }
            else if (renderer.kernel_type == ConvolutionKernel::blur){
                renderer.kernel_type = ConvolutionKernel::identity;
                ofLog() << "<toggle kernel type: identity>";
            }
            renderer.apply_filter();
            renderer.out.update();
            break;
            
        case 103: // g key
            grid_checkbox = !grid_checkbox;
            ofLog() << "<toggle grid: " << grid_checkbox << ">";
            break;
            
        case 104: // h key
            histo_checkbox = !histo_checkbox;
            ofLog() << "<toggle histogram: " << histo_checkbox << ">";
            break;
            
        case 105: // i key
            img_checkbox = !img_checkbox;
            ofLog() << "<toggle image: " << img_checkbox << ">";
            break;
        
        case 108: // l key
            light_checkbox = !light_checkbox;
            if (light_checkbox)
                gui_checkbox = false;
            else
                gui_checkbox = true;
            ofLog() << "<toggle lights: " << light_checkbox << ">";
            ofLog() << "<toggle ui: " << gui_checkbox << ">";
            break;
            
        case 109: // m key
            if (renderer.render_type == OF_MESH_WIREFRAME)
                renderer.render_type = OF_MESH_POINTS;
            else if (renderer.render_type == OF_MESH_POINTS)
                renderer.render_type = OF_MESH_FILL;
            else if (renderer.render_type == OF_MESH_FILL)
                renderer.render_type = OF_MESH_WIREFRAME;
            ofLog() << "<toggle render mode: " << renderer.render_type << ">";
            break;
            
        case 114: // key r
            renderer.reset();
            img_checkbox = false;
            histo_checkbox = false;
            grid_checkbox = false;
            gui_checkbox = true;
            ofLog() << "<reset>";
            break;
            
        case 116: // t key
            renderer.show_texture = !renderer.show_texture;
            ofLog() << "<show texture: " << renderer.show_texture << ">";
            break;
            
        case 117: // u key
            gui_checkbox = !gui_checkbox;
            ofLog() << "<toggle ui: " << gui_checkbox << ">";
            break;
            
        case 118: // v key
            activate_cam = !activate_cam;
            if (activate_cam) {
                light_checkbox = true;
                gui_checkbox = false;
            }
            else {
                light_checkbox = false;
                gui_checkbox = true;
            }
            ofLog() << "<toggle cam: " << activate_cam << ">";
            ofLog() << "<toggle ui: " << gui_checkbox << ">";
            ofLog() << "<toggle lights: " << light_checkbox << ">";
            break;
            
        case 120: { // x key
            string material_name;
            if (renderer.material_current == MaterialType::none) {
                material_name = "brass";
                renderer.material_current = MaterialType::brass;
            } else if (renderer.material_current == MaterialType::brass) {
                material_name = "chrome";
                renderer.material_current = MaterialType::chrome;
            } else if (renderer.material_current == MaterialType::chrome) {
                material_name = "plastic";
                renderer.material_current = MaterialType::plastic;
            } else if (renderer.material_current == MaterialType::plastic) {
                material_name = "rubber";
                renderer.material_current = MaterialType::rubber;
            } else {
                material_name = "none";
                renderer.material_current = MaterialType::none;
            }
            
            ofLog() << "<toggle material : " << material_name << ">";
            break;
        }
            
        case 121: { // y key
            renderer.material_shader = !renderer.material_shader;
            if (renderer.material_shader)
                ofLog() << "<toggle material/shader : " << "material" << ">";
            else
                ofLog() << "<toggle material/shader : " << "shader" << ">";
            break;
        }
            
        case 122: { // z key
            string shader_name;
            if (renderer.shader_active == ShaderType::color_fill) {
                renderer.shader_active = ShaderType::pbr;
                shader_name = "PBR";
            } else if (renderer.shader_active == ShaderType::pbr) {
                renderer.shader_active = ShaderType::lambert;
                shader_name = "Lambert";
            }
            else if (renderer.shader_active == ShaderType::lambert) {
                renderer.shader_active = ShaderType::gouraud;
                shader_name = "Gouraud";
            }
            else if (renderer.shader_active == ShaderType::gouraud) {
                renderer.shader_active = ShaderType::phong;
                shader_name = "Phong";
            }
            else if (renderer.shader_active == ShaderType::phong){
                renderer.shader_active = ShaderType::blinn_phong;
                shader_name = "Blinn-Phong";
            }
            else {
                renderer.shader_active = ShaderType::color_fill;
                shader_name = "Color Fill";
            }
            
            ofLog() << "<toggle illumination shader: " << shader_name << ">";
            break;
        }
            
        case 8: // backspace
            renderer.delete_shape();
            break;
            
        default:
            break;
    }
}

void Application::button_line_pressed(bool& bool_val)
{
    if (bool_val) {
        renderer.draw_mode = VectorPrimitiveType::line;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
        bool_line = true;
    } else {
        bool_line = false;
    }
    ofLog() << "<button line shape pressed>";
}

void Application::button_iso_pressed(bool& bool_val)
{
    if (bool_val) {
        renderer.draw_mode = VectorPrimitiveType::iso_triangle;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
        bool_iso = true;
    } else {
        bool_iso = false;
    }
    ofLog() << "<button isoscele triangle shape pressed>";
}

void Application::button_rectangle_pressed(bool& bool_val)
{
    if (bool_val) {
        renderer.draw_mode = VectorPrimitiveType::rectangle;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
        bool_rectangle = true;
    } else {
        bool_rectangle = false;
    }
    ofLog() << "<button rectangle shape pressed>";
}

void Application::button_circle_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.draw_mode = VectorPrimitiveType::circle;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
        bool_circle = true;
    } else {
        bool_circle = false;
    }
    ofLog() << "<button circle shape pressed>";
}

void Application::button_polygone_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.draw_mode = VectorPrimitiveType::polygone;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
        bool_polygone = true;
    } else {
        bool_polygone = false;
    }
    ofLog() << "<button ellipsis shape pressed>";
}

void Application::button_cube_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.draw_mode_3d = MeshPrimitiveType::cube;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_draw = true;
        bool_cube = true;
    } else {
        bool_cube = false;
    }
    ofLog() << "<button cube shape pressed>";
}

void Application::button_icosahedron_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.draw_mode_3d = MeshPrimitiveType::icosahedron;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_draw = true;
        bool_icosahedron = true;
    } else {
        bool_icosahedron = false;
    }
    ofLog() << "<button icosahedron shape pressed>";
}

void Application::button_heart_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.model_mode = ModelType::heart;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_model = true;
        bool_heart = true;
    } else {
        bool_heart = false;
    }
    ofLog() << "<button heart model pressed>";
}

void Application::button_banana_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.model_mode = ModelType::banana;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_model = true;
        bool_banana = true;
    } else {
        bool_banana = false;
    }
    ofLog() << "<button banana model pressed>";
}

void Application::button_big_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.model_mode = ModelType::big_hero_six;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_model = true;
        bool_big = true;
    } else {
        bool_big = false;
    }
    ofLog() << "<button big_hero_six model pressed>";
}

void Application::button_soccer_pressed(bool& bool_val)
{
    if (bool_val){
        renderer.model_mode = ModelType::soccer;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_model = true;
        bool_soccer = true;
    } else {
        bool_soccer = false;
    }
    ofLog() << "<button soccer model pressed>";
}

void Application::button_3d_model_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        group_tools.maximize();
        group_3d_model.maximize();
        
        renderer.current_mode = ToolMode::model_draw;
        renderer.selected_shapes_index.clear();
        
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_model = true;
    } else {
        if (!group_3d_model.isMinimized())
            group_3d_model.minimize();
        bool_3D_model = false;
    }
    ofLog() << "<button 3d model tool pressed>";
}

void Application::button_draw_3d_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        group_tools.maximize();
        group_3d_draw.maximize();
        
        renderer.current_mode = ToolMode::mesh_draw;
        renderer.selected_shapes_index.clear();
        
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_3D_draw = true;
    } else {
        if (!group_3d_draw.isMinimized())
            group_3d_draw.minimize();
        bool_3D_draw = false;
    }
    ofLog() << "<button 3d draw tool pressed>";
}

void Application::button_draw_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        group_tools.maximize();
        group_draw.maximize();
        group_shapes.maximize();
        
        renderer.current_mode = ToolMode::vector_draw;
        renderer.selected_shapes_index.clear();
        
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_2D_draw = true;
    } else {
        if (!group_draw.isMinimized())
            group_draw.minimize();
        bool_2D_draw = false;
    }
    ofLog() << "<button draw tool pressed>";
}

void Application::button_select_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        group_tools.maximize();

        renderer.current_mode = ToolMode::select;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_select = true;
    } else {
        bool_select = false;
    }
    ofLog() << "<button selection tool pressed>";
}

void Application::button_rotate_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        group_tools.maximize();
        group_transform.maximize();
        group_rotate.maximize();
        
        renderer.current_mode = ToolMode::rotate;
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_rotate = true;

    } else {
        if (!group_rotate.isMinimized())
            group_rotate.minimize();
        bool_rotate = false;
    }
    ofLog() << "<button selection tool pressed>";
}

void Application::button_translate_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_translate = true;
        
        group_tools.maximize();
        group_transform.maximize();
        group_translate.maximize();
        
        renderer.current_mode = ToolMode::translate;

        } else {
            if (!group_translate.isMinimized())
                group_translate.minimize();
            bool_translate = false;
        }
    ofLog() << "<button translate tool pressed>";
}

void Application::button_directional_pressed() {
    bool_directional_light = !bool_directional_light;
}

void Application::button_point_pressed() {
    bool_point_light = !bool_point_light;
}

void Application::button_spot_pressed() {
    bool_spot_light = !bool_spot_light;
}

void Application::button_scale_pressed(bool& bool_val) {
    if (bool_val) {
        for (int i = 0 ; i < group_vector.size() ; i++) {
            ofxGuiGroup* group = group_vector[i];
            if (!group->isMinimized()) {
                group->minimize();
            }
        }
        for_each(bool_vector.begin(), bool_vector.end(), [](ofParameter<bool> &b){ b = false;});
        bool_scale = true;
        
        group_tools.maximize();
        group_transform.maximize();
        group_scale.maximize();
        
        renderer.current_mode = ToolMode::scale;
        
    } else {
        if (!group_scale.isMinimized())
            group_scale.minimize();
        bool_scale = false;
    }
    ofLog() << "<button scale tool pressed>";
}

void Application::button_rotate_ok_pressed(){
    renderer.rotation_angle = slider_rotation_angle;
    renderer.apply_rotation();
    slider_rotation_angle.set(0.0f);
}

void Application::button_scale_ok_pressed(){
    renderer.scale_ratio_x = slider_scale_x;
    renderer.scale_ratio_y = slider_scale_y;
    renderer.scale_ratio_z = slider_scale_z;
    renderer.apply_scale();
    slider_scale_x.set(1.0f);
    slider_scale_y.set(1.0f);
    slider_scale_z.set(1.0f);
}

void Application::button_translate_ok_pressed(){
    renderer.translation_x = slider_translate_x;
    renderer.translation_y = slider_translate_y;
    renderer.translation_z = slider_translate_z;
    renderer.apply_translation();
    slider_translate_x.set(0.0f);
    slider_translate_y.set(0.0f);
    slider_translate_z.set(0.0f);
}

void Application::exit()
{
    ofLog() << "<app::exit>";
}

void Application::calculateHistograms() {
    int delta = 2;
    int nBins = 255 / delta;
    vector<float> redHist(nBins, 0);
    vector<float> greenHist(nBins, 0);
    vector<float> blueHist(nBins, 0);
    
    int xStart = ofClamp(histo_rect.getX() - renderer.image_position.x, 0, renderer.out.getWidth());
    int yStart = ofClamp(histo_rect.getY() - renderer.image_position.y, 0, renderer.out.getHeight());
    int xEnd = ofClamp(histo_rect.getX() + histo_rect.getWidth() - renderer.image_position.x, 0, renderer.out.getWidth());
    int yEnd = ofClamp(histo_rect.getY() + histo_rect.getHeight() - renderer.image_position.y, 0, renderer.out.getHeight());

    for (int x = xStart; x < xEnd; ++x) {
        for (int y = yStart; y < yEnd; ++y) {
            ofColor c = renderer.out.getColor(x, y);
            ++redHist[c.r / delta];
            ++greenHist[c.g / delta];
            ++blueHist[c.b / delta];
        }
    }
    
    // Calculate the plot points
    redHistPoints.clear();
    greenHistPoints.clear();
    blueHistPoints.clear();
    
    for (int i = 0; i < nBins; ++i) {
        redHistPoints.emplace_back(i, redHist[i]);
        greenHistPoints.emplace_back(i, greenHist[i]);
        blueHistPoints.emplace_back(i, blueHist[i]);
    }
}

void Application::draw_histogram() {
    histogram.beginDraw();
    histogram.drawBox();
    histogram.drawTitle();
//    histogram.drawXAxis();
    histogram.getMainLayer().drawFilledContour(GRAFICA_HORIZONTAL_CONTOUR, 0.05);
    histogram.getLayer("layer g").drawFilledContour(GRAFICA_HORIZONTAL_CONTOUR, 0.05);
    histogram.getLayer("layer b").drawFilledContour(GRAFICA_HORIZONTAL_CONTOUR, 0.05);
    histogram.endDraw();
}

void Application::slider_catmull_changed() {
    renderer.catmull_rom_pts[2*(current_catmull_pt -1)] = slider_position_catmull_x;
    renderer.catmull_rom_pts[2*(current_catmull_pt-1)+1] = slider_position_catmull_y;
    current_catmull_pt = slider_active_catmull_pt;
    
    slider_position_catmull_x = renderer.catmull_rom_pts[2*(slider_active_catmull_pt -1)];
    slider_position_catmull_y = renderer.catmull_rom_pts[2*(slider_active_catmull_pt -1)+1];
}

void Application::slider_nb_catmull_pts_changed() {
    slider_active_catmull_pt.setMax(slider_nb_catmull_pts);
}

void Application::button_show_catmull_curve_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_catmull_curve = true;
        button_show_catmull_curve = true;
    } else {
        renderer.show_catmull_curve = false;
        button_show_catmull_curve = false;
        renderer.show_catmull_controls = false;
        button_show_catmull_controls = false;
    }
}

void Application::button_show_catmull_controls_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_catmull_controls = true;
        button_show_catmull_controls = true;
    } else {
        renderer.show_catmull_controls = false;
        button_show_catmull_controls = false;
    }
    
}

void Application::slider_position_catmull_x_changed() {
    renderer.catmull_rom_pts[2*(current_catmull_pt -1)] = slider_position_catmull_x;
}

void Application::slider_position_catmull_y_changed() {
    renderer.catmull_rom_pts[2*(current_catmull_pt -1)+1] = slider_position_catmull_y;
}

void Application::slider_catmull_alpha_changed() {
    renderer.catmull_alpha = slider_catmull_alpha;
}

void Application::slider_position_coons_x_changed() {
    switch (current_coons_curve) {
        case 1:
            renderer.coons_curve_1[3*(current_coons_pt-1)] = slider_coons_position_x;
            if (current_coons_pt == 2) {
                // C1(0) = C3(0)
                renderer.coons_curve_3[3] = slider_coons_position_x;
            } else if (current_coons_pt == 5) {
                // C1(1) = C4(0)
                renderer.coons_curve_4[3] = slider_coons_position_x;
            }
            break;
        case 2:
            renderer.coons_curve_2[3*(current_coons_pt -1)] = slider_coons_position_x;
            if (current_coons_pt == 2) {
                // C2(0) = C3(1)
                renderer.coons_curve_3[12] = slider_coons_position_x;
            } else if (current_coons_pt == 5) {
                // C2(1) = C4(1)
                renderer.coons_curve_4[12] = slider_coons_position_x;
            }
            break;
        case 3:
            renderer.coons_curve_3[3*(current_coons_pt -1)] = slider_coons_position_x;
            if (current_coons_pt == 2) {
                // C3(0) = C1(0)
                renderer.coons_curve_1[3] = slider_coons_position_x;
            } else if (current_coons_pt == 5) {
                // C3(1) = C2(0)
                renderer.coons_curve_2[3] = slider_coons_position_x;
            }
            break;
        case 4:
            renderer.coons_curve_4[3*(current_coons_pt -1)] = slider_coons_position_x;
            if (current_coons_pt == 2) {
                // C4(0) = C1(1)
                renderer.coons_curve_1[12] = slider_coons_position_x;
            } else if (current_coons_pt == 5) {
                // C4(1) = C2(1)
                renderer.coons_curve_2[12] = slider_coons_position_x;
            }
            break;
        default:
            break;
    }

}
void Application::slider_position_coons_y_changed() {
    switch (current_coons_curve) {
        case 1:
            renderer.coons_curve_1[3*(current_coons_pt -1)+1] = slider_coons_position_y;
            if (current_coons_pt == 2) {
                // C1(0) = C3(0)
                renderer.coons_curve_3[4] = slider_coons_position_y;
            } else if (current_coons_pt == 5) {
                // C1(1) = C4(0)
                renderer.coons_curve_4[4] = slider_coons_position_y;
            }
            break;
        case 2:
            renderer.coons_curve_2[3*(current_coons_pt -1)+1] = slider_coons_position_y;
            if (current_coons_pt == 2) {
                // C2(0) = C3(1)
                renderer.coons_curve_3[13] = slider_coons_position_y;
            } else if (current_coons_pt == 5) {
                // C2(1) = C4(1)
                renderer.coons_curve_4[13] = slider_coons_position_y;
            }
            break;
        case 3:
            renderer.coons_curve_3[3*(current_coons_pt -1)+1] = slider_coons_position_y;
            if (current_coons_pt == 2) {
                // C3(0) = C1(0)
                renderer.coons_curve_1[4] = slider_coons_position_y;
            } else if (current_coons_pt == 5) {
                // C3(1) = C2(0)
                renderer.coons_curve_2[4] = slider_coons_position_y;
            }
            break;
        case 4:
            renderer.coons_curve_4[3*(current_coons_pt -1)+1] = slider_coons_position_y;
            if (current_coons_pt == 2) {
                // C4(0) = C1(1)
                renderer.coons_curve_1[13] = slider_coons_position_y;
            } else if (current_coons_pt == 5) {
                // C4(1) = C2(1)
                renderer.coons_curve_2[13] = slider_coons_position_y;
            }
            break;
        default:
            break;
    }
}
void Application::slider_position_coons_z_changed() {
    switch (current_coons_curve) {
        case 1:
            renderer.coons_curve_1[3*(current_coons_pt -1)+2] = slider_coons_position_z;
            if (current_coons_pt == 2) {
                // C1(0) = C3(0)
                renderer.coons_curve_3[5] = slider_coons_position_z;
            } else if (current_coons_pt == 5) {
                // C1(1) = C4(0)
                renderer.coons_curve_4[5] = slider_coons_position_z;
            }
            break;
        case 2:
            renderer.coons_curve_2[3*(current_coons_pt -1)+2] = slider_coons_position_z;
            if (current_coons_pt == 2) {
                // C2(0) = C3(1)
                renderer.coons_curve_3[14] = slider_coons_position_z;
            } else if (current_coons_pt == 5) {
                // C2(1) = C4(1)
                renderer.coons_curve_4[14] = slider_coons_position_z;
            }
            break;
        case 3:
            renderer.coons_curve_3[3*(current_coons_pt -1)+2] = slider_coons_position_z;
            if (current_coons_pt == 2) {
                // C3(0) = C1(0)
                renderer.coons_curve_1[5] = slider_coons_position_z;
            } else if (current_coons_pt == 5) {
                // C3(1) = C2(0)
                renderer.coons_curve_2[5] = slider_coons_position_z;
            }
            break;
        case 4:
            renderer.coons_curve_4[3*(current_coons_pt -1)+2] = slider_coons_position_z;
            if (current_coons_pt == 2) {
                // C4(0) = C1(1)
                renderer.coons_curve_1[14] = slider_coons_position_z;
            } else if (current_coons_pt == 5) {
                // C4(1) = C2(1)
                renderer.coons_curve_2[14] = slider_coons_position_z;
            }
            break;
        default:
            break;
    }
}

void Application::slider_coons_active_pt_changed() {
//    slider_position_coons_x_changed();
//    slider_position_coons_y_changed();
//    slider_position_coons_z_changed();

    current_coons_pt = slider_active_coons_pt;
    
    switch (current_coons_curve) {
        case 1:
            slider_coons_position_x = renderer.coons_curve_1[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_1[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_1[3*(current_coons_pt -1)+2];
            break;
        case 2:
            slider_coons_position_x = renderer.coons_curve_2[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_2[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_2[3*(current_coons_pt -1)+2];
            break;
        case 3:
            slider_coons_position_x = renderer.coons_curve_3[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_3[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_3[3*(current_coons_pt -1)+2];
            break;
        case 4:
            slider_coons_position_x = renderer.coons_curve_4[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_4[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_4[3*(current_coons_pt -1)+2];
            break;
        default:
            break;
    }
}

void Application::slider_coons_active_curve_changed() {
//    slider_position_coons_x_changed();
//    slider_position_coons_y_changed();
//    slider_position_coons_z_changed();
    
    current_coons_curve = slider_active_coons_curve;
    current_coons_pt = 1;
    slider_active_coons_pt = current_coons_pt;
    
    switch (current_coons_curve) {
        case 1:
            slider_coons_position_x = renderer.coons_curve_1[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_1[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_1[3*(current_coons_pt -1)+2];
            break;
        case 2:
            slider_coons_position_x = renderer.coons_curve_2[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_2[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_2[3*(current_coons_pt -1)+2];
            break;
        case 3:
            slider_coons_position_x = renderer.coons_curve_3[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_3[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_3[3*(current_coons_pt -1)+2];
            break;
        case 4:
            slider_coons_position_x = renderer.coons_curve_4[3*(current_coons_pt -1)];
            slider_coons_position_y = renderer.coons_curve_4[3*(current_coons_pt -1)+1];
            slider_coons_position_z = renderer.coons_curve_4[3*(current_coons_pt -1)+2];
            break;
        default:
            break;
    }
}

void Application::button_show_coons_patch_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_coons_patch = true;
        button_show_coons_patch = true;
    } else {
        renderer.show_coons_patch = false;
        button_show_catmull_curve = false;
        renderer.show_coons_controls = false;
        button_show_coons_controls = false;
    }
    
}
void Application::button_show_coons_controls_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_coons_controls = true;
        button_show_coons_controls = true;
    } else {
        renderer.show_coons_controls = false;
        button_show_coons_controls = false;
    }
}

void Application::button_ray_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_ray = true;
        renderer.show_ray_2d = false;
        button_show_ray_2d = false;
    }
    else
        renderer.show_ray = false;
}

void Application::button_ray_2d_pressed(bool& bool_val) {
    if (bool_val) {
        renderer.show_ray_2d = true;
        renderer.show_ray = false;
        button_show_ray = false;
    }
    else
        renderer.show_ray_2d = false;
}




