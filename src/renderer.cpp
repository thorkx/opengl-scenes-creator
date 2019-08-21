// Projet Session/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"

void Renderer::setup()
{
    ofSetFrameRate(60);
    ofDisableArbTex();
    ofSetBackgroundColor(191);
    
    // Max number of allowed shapes
    max_count_2d = 50;
    max_count_3d = 10;
    max_count_3d_models = 3;
    
    // Max number of undos
    undo_count = 10;
    
    // Index of next shape
    head_2d = 0;
    head_3d = 0;
    head_3d_models = 0;
    
    // Vector containing the currently selected shapes indexes
    selected_shapes_index = {};
    
    // Default shape mode
    draw_mode = VectorPrimitiveType::rectangle;
    draw_mode_3d = MeshPrimitiveType::cube;
    model_mode = ModelType::heart;
    
    // Default active mode
    current_mode = ToolMode::vector_draw;
    
    mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
    
    is_mouse_button_pressed = false;
    
    // Color for selected items highlight
    selected_color = ofColor(255, 255, 0, 70);
    
    radius = 4.0f;
    
    show_bounding_box = false;
    show_cubemap = false;
    
    // Load cubemap
    sphere = ofMesh::sphere(100,20,OF_PRIMITIVE_TRIANGLES);
    box = ofMesh::box(200,200,200,1,1,1);

    ofImage px("cubemap/rainbow/posx.jpg"); pos.push_back(px);
    ofImage py("cubemap/rainbow/posy.jpg"); pos.push_back(py);
    ofImage pz("cubemap/rainbow/posz.jpg"); pos.push_back(pz);
    ofImage nx("cubemap/rainbow/negx.jpg"); neg.push_back(nx);
    ofImage ny("cubemap/rainbow/negy.jpg"); neg.push_back(ny);
    ofImage nz("cubemap/rainbow/negz.jpg"); neg.push_back(nz);
    
    ofxReflectionRefraction r1;
    r1.setup(sphere,pos,neg,true);
    Ref.push_back(r1);
    ofxReflectionRefraction r2;
    r2.setup(box,pos,neg,false);
    Ref.push_back(r2);
    
    // Default 3d render mode
    render_type = OF_MESH_WIREFRAME;
    
    // Load external 3d models
    heart.loadModel("3d_models/heart.obj");
    big_hero_six.loadModel("3d_models/baymax.obj");
    banana.loadModel("3d_models/banana.obj");
    soccer.loadModel("3d_models/football.obj");
    
    // Load texture
    ofDisableArbTex();
    
    banana_tex_img.load("3d_models/banana_texture.png");
    banana_texture.loadData(banana_tex_img);
    
    // Source : http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
    material_brass.setAmbientColor(ofFloatColor(0.329412f, 0.223529f, 0.027451f, 1.0f));
    material_brass.setDiffuseColor(ofFloatColor(0.780392f, 0.568627f, 0.113725f, 1.0f));
    material_brass.setSpecularColor(ofFloatColor(0.992157f, 0.941176f, 0.807843f, 1.0f));
    material_brass.setShininess(27.8974f);
    
    material_chrome.setAmbientColor(ofFloatColor(0.25f, 0.25f, 0.25f, 1.0f));
    material_chrome.setDiffuseColor(ofFloatColor(0.4f, 0.4f, 0.4f, 1.0f));
    material_chrome.setSpecularColor(ofFloatColor(0.774597f, 0.774597f, 0.774597f, 1.0f));
    material_chrome.setShininess(76.8f);
    
    material_plastic.setAmbientColor(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    material_plastic.setDiffuseColor(ofFloatColor(0.5f, 0.0f, 0.0f, 1.0f));
    material_plastic.setSpecularColor(ofFloatColor(0.7f, 0.6f, 0.6f, 1.0f));
    material_plastic.setShininess(32.0f);
    
    material_rubber.setAmbientColor(ofFloatColor(0.05f, 0.05f, 0.05f, 1.0f));
    material_rubber.setDiffuseColor(ofFloatColor(0.5f, 0.5f, 0.5f, 1.0f));
    material_rubber.setSpecularColor(ofFloatColor(0.7f, 0.7f, 0.7f, 1.0f));
    material_rubber.setShininess(10.0f);
    
    material_current = MaterialType::none;

    // Load image
    in.load("pexels-photo-1981945.jpeg");
    image_height = ofGetHeight() / 2.0f;
    image_ratio = image_height / in.getHeight() ;
    image_width = in.getWidth() * image_ratio;

    in.resize(image_width, image_height);
    out.allocate(image_width, image_height, OF_IMAGE_COLOR);
    
    image_position.x = ofGetWidth() - out.getWidth() - 5.0f;
    image_position.y = 5.0f;
    
    kernel_type = ConvolutionKernel::identity;
    apply_filter();
    
    ofEnableLighting();
    
    global_ambient_color = ofColor(255,255,231);
    
    // Ambient light
    ambient_light.setAmbientColor(global_ambient_color);
    ambient_light.setDiffuseColor(ambient_light_color);
    ambient_light.setPosition(ambient_light_x, ambient_light_y, ambient_light_z);
    ambient_light.enable();
    
    // Lights
    directional_light.setDirectional();
    point_light.setPointLight();
    spot_light.setSpotlight();
    spot_light.setSpotConcentration(2);
    
    // Camera
    ortho_mode = false;
    ofEnableDepthTest();
    ofSetVerticalSync(true);
    cam.setScale(1,-1,1);
    
    // Illumination shaders
    shader_color_fill.load(
                           "shader/color_fill_330_vs.glsl",
                           "shader/color_fill_330_fs.glsl");

    shader_lambert.load(
                        "shader/lambert_330_vs.glsl",
                        "shader/lambert_330_fs.glsl");

    shader_gouraud.load(
                        "shader/gouraud_330_vs.glsl",
                        "shader/gouraud_330_fs.glsl");

    shader_phong.load(
                      "shader/phong_330_vs.glsl",
                      "shader/phong_330_fs.glsl");
    
    shader_blinn_phong.load(
                            "shader/blinn_phong_330_vs.glsl",
                            "shader/blinn_phong_330_fs.glsl");
    
    shader_pbr.load(
                    "shader/pbr_330_vs.glsl",
                    "shader/pbr_330_fs.glsl");
    
    texture_diffuse.load("metal_plate_diffuse_1k.jpg");
    texture_metallic.load("metal_plate_metallic_1k.jpg");
    texture_roughness.load("metal_plate_roughness_1k.jpg");
    texture_occlusion.load("metal_plate_ao_1k.jpg");
    
    texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    // paramètres du matériau
    material_color_ambient = ofColor(63, 63, 63);
    material_color_diffuse = ofColor(255, 255, 255);
    material_color_specular = ofColor(255, 255, 255);
    
    material_metallic = 1.0f;
    material_roughness = 0.5f;
    material_occlusion = 1.5f;
    material_brightness = 5.0f;
    
    material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);
    
    light_intensity = 10.0f;
    
    // paramètres de mappage tonal
    tone_mapping_exposure = 1.0f;
    tone_mapping_toggle = true;
    tone_mapping_gamma = 1.5f;
    
    
    // Tessellation shader
    // https://github.com/leozimmerman/ShadersLibrary
    
    shader_tessellation.setGeometryInputType(GL_TRIANGLES);

    shader_tessellation.setGeometryOutputType(GL_LINE_STRIP);
    shader_tessellation.setGeometryOutputCount(4);
    
    shader_tessellation.setupShaderFromFile(GL_VERTEX_SHADER, "shader/Tessellation/tessQuads.vert");
    shader_tessellation.setupShaderFromFile(GL_FRAGMENT_SHADER, "shader/Tessellation/tessQuads.frag");
    shader_tessellation.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "shader/Tessellation/tessQuads.geom");
    shader_tessellation.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "shader/Tessellation/tessQuads.cont");
    shader_tessellation.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "shader/Tessellation/tessQuads.eval");
    shader_tessellation.linkProgram();
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    
    activate_tessellation = false;
    tessLevelInner = 2.0f;
    tessLevelOuter = 2.0f;
    
    // shader actif au lancement de la scène
    shader_active = ShaderType::blinn_phong;
    
    material_shader = true;
    
    oscillation_amplitude = 32.0f;
    oscillation_frequency = 7500.0f;
    
    show_catmull_curve = false;
    show_catmull_controls = false;
    catmull_line_resolution = 10;
    catmull_line_width_outline = 4.0f;
    catmull_line_width_curve = 8.0f;
    catmull_alpha = 1.0f;
    
    for (int index = 0; index <= catmull_line_resolution; ++index)
        catmull_renderer.addVertex(ofPoint());
    
    // Raycaster setup
    ray_origin = glm::vec3(50, 50, -50);
    ray_direction = glm::vec3(1, 1, 0);
    ray.setup(ray_origin, ray_direction);
    ray_2d.setup(glm::vec2(50, 50), glm::vec2(1, 1));
    show_ray = false;
}

void Renderer::update()
{
    ofSetBackgroundColor(background_color);
    ofSetColor(stroke_color);
    ofSetLineWidth(stroke_width);
    
    ambient_light.setDiffuseColor(ambient_light_color);
    ambient_light.setPosition(ambient_light_x, ambient_light_y, ambient_light_z);
    
    directional_light.setDiffuseColor(directional_light_color);
    directional_light.setPosition(directional_light_x, directional_light_y, directional_light_z);
    directional_light.setOrientation(ofVec3f(directional_light_orientation_x, directional_light_orientation_y, directional_light_orientation_z));
    
    if (bool_directional_light)
        directional_light.enable();
    else
        directional_light.disable();
    
    point_light.setDiffuseColor(point_light_color);
    point_light.setSpecularColor(point_light_color);
    point_light.setPosition(point_light_x, point_light_y, point_light_z);
    
    if (bool_point_light)
        point_light.enable();
    else
        point_light.disable();
    
    spot_light.setDiffuseColor(spot_light_color);
    spot_light.setSpecularColor(spot_light_color);
    spot_light.setPosition(spot_light_x, spot_light_y, spot_light_z);
    spot_light.setOrientation(ofVec3f(spot_light_orientation_x, spot_light_orientation_y, spot_light_orientation_z));
    spot_light.setSpotlightCutOff(spot_light_cutoff);
    
    if (bool_spot_light)
        spot_light.enable();
    else
        spot_light.disable();
    
    if (show_catmull_curve){
        catmull_renderer.clear();
        int current_pt = 4;
        ofVec3f ctrl_pt_1 = ofVec3f(catmull_rom_pts[0], catmull_rom_pts[1], 0);
        ofVec3f ctrl_pt_2 = ofVec3f(catmull_rom_pts[2], catmull_rom_pts[3], 0);
        ofVec3f ctrl_pt_3 = ofVec3f(catmull_rom_pts[4], catmull_rom_pts[5], 0);
        ofVec3f ctrl_pt_4 = ofVec3f(catmull_rom_pts[6], catmull_rom_pts[7], 0);
        vector<ofVec3f> positions;
        positions.push_back(ofVec3f(ctrl_pt_2.x, ctrl_pt_2.y, ctrl_pt_2.z));
        while (current_pt <= catmull_nb_pts) {
            catmull_rom(catmull_line_resolution, catmull_alpha, ctrl_pt_1.x, ctrl_pt_1.y, ctrl_pt_1.z, ctrl_pt_2.x, ctrl_pt_2.y, ctrl_pt_2.z, ctrl_pt_3.x, ctrl_pt_3.y, ctrl_pt_3.z, ctrl_pt_4.x, ctrl_pt_4.y, ctrl_pt_4.z, positions);
            positions.push_back(ofVec3f(ctrl_pt_3.x, ctrl_pt_3.y, ctrl_pt_3.z));
            ctrl_pt_1 = ctrl_pt_2;
            ctrl_pt_2 = ctrl_pt_3;
            ctrl_pt_3 = ctrl_pt_4;
            ctrl_pt_4 = ofVec3f(catmull_rom_pts[2*current_pt], catmull_rom_pts[2*current_pt+1], 0);
            current_pt += 1;
        }
        for (int i = 0 ; i < positions.size() ; i++) {
            catmull_renderer.addVertex(positions[i].x, positions[i].y, positions[i].z);
        }
    }
    
    if (show_coons_patch){
        coons_mesh.clear();
        for (int c = 1; c <= 4 ; c++) {
            ofVec3f ctrl_pt_1;
            ofVec3f ctrl_pt_2;
            ofVec3f ctrl_pt_3;
            ofVec3f ctrl_pt_4;
            switch (c) {
                case 1:
                    coons_curve_1_renderer.clear();
                    ctrl_pt_1 = ofVec3f(coons_curve_1[0], coons_curve_1[1], coons_curve_1[2]);
                    ctrl_pt_2 = ofVec3f(coons_curve_1[3], coons_curve_1[4], coons_curve_1[5]);
                    ctrl_pt_3 = ofVec3f(coons_curve_1[6], coons_curve_1[7], coons_curve_1[8]);
                    ctrl_pt_4 = ofVec3f(coons_curve_1[10], coons_curve_1[10], coons_curve_1[11]);
                    break;
                case 2:
                    coons_curve_2_renderer.clear();
                    ctrl_pt_1 = ofVec3f(coons_curve_2[0], coons_curve_2[1], coons_curve_2[2]);
                    ctrl_pt_2 = ofVec3f(coons_curve_2[3], coons_curve_2[4], coons_curve_2[5]);
                    ctrl_pt_3 = ofVec3f(coons_curve_2[6], coons_curve_2[7], coons_curve_2[8]);
                    ctrl_pt_4 = ofVec3f(coons_curve_2[10], coons_curve_2[10], coons_curve_2[11]);
                    break;
                case 3:
                    coons_curve_3_renderer.clear();
                    ctrl_pt_1 = ofVec3f(coons_curve_3[0], coons_curve_3[1], coons_curve_3[2]);
                    ctrl_pt_2 = ofVec3f(coons_curve_3[3], coons_curve_3[4], coons_curve_3[5]);
                    ctrl_pt_3 = ofVec3f(coons_curve_3[6], coons_curve_3[7], coons_curve_3[8]);
                    ctrl_pt_4 = ofVec3f(coons_curve_3[10], coons_curve_3[10], coons_curve_3[11]);
                    break;
                case 4:
                    coons_curve_4_renderer.clear();
                    ctrl_pt_1 = ofVec3f(coons_curve_4[0], coons_curve_4[1], coons_curve_4[2]);
                    ctrl_pt_2 = ofVec3f(coons_curve_4[3], coons_curve_4[4], coons_curve_4[5]);
                    ctrl_pt_3 = ofVec3f(coons_curve_4[6], coons_curve_4[7], coons_curve_4[8]);
                    ctrl_pt_4 = ofVec3f(coons_curve_4[10], coons_curve_4[10], coons_curve_4[11]);
                    break;
                default:
                    break;
            }
        
            int current_pt = 4;
            vector<ofVec3f> positions;
                        
            while (current_pt <= 6) {
                catmull_rom(catmull_line_resolution, 0.5f, ctrl_pt_1.x, ctrl_pt_1.y, ctrl_pt_1.z, ctrl_pt_2.x, ctrl_pt_2.y, ctrl_pt_2.z, ctrl_pt_3.x, ctrl_pt_3.y, ctrl_pt_3.z, ctrl_pt_4.x, ctrl_pt_4.y, ctrl_pt_4.z, positions);
                positions.push_back(ofVec3f(ctrl_pt_3.x, ctrl_pt_3.y, ctrl_pt_3.z));
                ctrl_pt_1 = ctrl_pt_2;
                ctrl_pt_2 = ctrl_pt_3;
                ctrl_pt_3 = ctrl_pt_4;
                switch (c) {
                    case 1:
                        ctrl_pt_4 = ofVec3f(coons_curve_1[3*current_pt], coons_curve_1[3*current_pt + 1], coons_curve_1[3*current_pt + 2]);
                        break;
                    case 2:
                        ctrl_pt_4 = ofVec3f(coons_curve_2[3*current_pt], coons_curve_2[3*current_pt + 1], coons_curve_2[3*current_pt + 2]);
                        break;
                    case 3:
                        ctrl_pt_4 = ofVec3f(coons_curve_3[3*current_pt], coons_curve_3[3*current_pt + 1], coons_curve_3[3*current_pt + 2]);
                        break;
                    case 4:
                        ctrl_pt_4 = ofVec3f(coons_curve_4[3*current_pt], coons_curve_4[3*current_pt + 1], coons_curve_4[3*current_pt + 2]);
                        break;
                    default:
                        break;
                }
                current_pt += 1;
            }
            for (int i = 0 ; i < positions.size() ; i++) {
                switch (c) {
                    case 1:
                        coons_curve_1_renderer.addVertex(positions[i].x, positions[i].y, positions[i].z);
                        break;
                    case 2:
                        coons_curve_2_renderer.addVertex(positions[i].x, positions[i].y, positions[i].z);
                        break;
                    case 3:
                        coons_curve_3_renderer.addVertex(positions[i].x, positions[i].y, positions[i].z);
                        break;
                    case 4:
                        coons_curve_4_renderer.addVertex(positions[i].x, positions[i].y, positions[i].z);
                        break;
                    default:
                        break;
                }
            }
            switch (c) {
                case 1:
                    positions_coons_curve_1 = positions;
                    break;
                case 2:
                    positions_coons_curve_2 = positions;
                    break;
                case 3:
                    positions_coons_curve_3 = positions;
                    break;
                case 4:
                    positions_coons_curve_4 = positions;
                    break;
            }
        }
        vector<ofVec3f> positions;
        coons_patch(10.0, positions_coons_curve_1, positions_coons_curve_2, positions_coons_curve_3, positions_coons_curve_4, positions);
        
        for (int i = 0 ; i < positions.size() ; i++) {
            coons_mesh.addVertex(ofPoint(positions[i]));
        }

        int dim = sqrt(coons_mesh.getNumVertices());
        for (int j = 0 ; j < dim-1 ; j++) {
            for (int i = 0 ; i < dim - 1 ; i++) {
                coons_mesh.addTriangle(i+(j*(dim)), i+(j+1)*(dim), i+(j+1)*(dim) +1);
                coons_mesh.addTriangle(i+(j*(dim)), i+j*(dim)+1, i+(j+1)*(dim) +1);
            }
        }
    }
    
    if (show_cam) {
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
        if (ortho_mode)
            cam.enableOrtho();
        else
            cam.disableOrtho();
    } else {
        cam.disableMouseInput();
        cam.disableMouseMiddleButton();
    }
    
    float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;

    
    switch (shader_active)
    {
        case ShaderType::pbr:
            shader = &shader_pbr;

            shader->begin();
            shader->setUniform3f("material_color_ambient", material_color_ambient.r/255.0f, material_color_ambient.g/255.0f, material_color_ambient.b/255.0f);
            shader->setUniform3f("material_color_diffuse", material_color_diffuse.r/255.0f, material_color_diffuse.g/255.0f, material_color_diffuse.b/255.0f);
            shader->setUniform3f("material_color_specular", material_color_specular.r/255.0f, material_color_specular.g/255.0f, material_color_specular.b/255.0f);
            
            shader->setUniform1f("material_brightness", material_brightness);
            shader->setUniform1f("material_metallic", material_metallic);
            shader->setUniform1f("material_roughness", material_roughness);
            shader->setUniform1f("material_occlusion", material_occlusion);
            
            shader->setUniform3f("material_fresnel_ior", material_fresnel_ior);
            
            shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
            shader->setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
            shader->setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
            shader->setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);
            
            shader->setUniform1f("light_intensity", light_intensity);
            shader->setUniform3f("light_color", ambient_light_color.r/255.0f, ambient_light_color.g/255.0f, ambient_light_color.b/255.0f);
            shader->setUniform3f("light_position", glm::vec4(ambient_light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            
            shader->setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
            shader->setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
            shader->setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
            
            shader->end();
            break;
        case ShaderType::color_fill:
            shader = &shader_color_fill;
            shader->begin();
            shader->setUniform3f("color", 1.0f, 1.0f, 0.0f);
            shader->end();
            break;
            
        case ShaderType::lambert:
            shader = &shader_lambert;
            shader->begin();
            shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
            shader->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
            shader->setUniform3f("light_position", glm::vec4(ambient_light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            shader->end();
            break;

        case ShaderType::gouraud:
            shader = &shader_gouraud;
            shader->begin();
            shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
            shader->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
            shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
            shader->setUniform1f("brightness", oscillation);
            shader->setUniform3f("light_position", glm::vec4(ambient_light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            shader->end();
            break;

        case ShaderType::phong:
            shader = &shader_phong;
            shader->begin();
            shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
            shader->setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
            shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
            shader->setUniform1f("brightness", oscillation);
            shader->setUniform3f("light_position", glm::vec4(ambient_light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            shader->end();
            break;
            
        case ShaderType::blinn_phong:
            shader = &shader_blinn_phong;
            shader->begin();
            shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
            shader->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
            shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
            shader->setUniform1f("brightness", oscillation);
            shader->setUniform3f("light_position", glm::vec4(ambient_light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            shader->end();
            break;
            
        default:
            break;
    }
}

void Renderer::image_export() const
{
    ofImage image;
    string extension = "png";
    string name = "render";
    
    string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
    
    string file_name = name + time_stamp + "." + extension;
    
    image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    image.save(file_name);
    
    ofLog() << "<export image: " << file_name << ">";
}

// Draw all
void Renderer::draw()
{

    if (show_image)
        out.draw(image_position.x, image_position.y, out.getWidth(), out.getHeight());
    
    if (show_cam) {
        if (show_cubemap) {
            ofPushMatrix();
            ofSetVerticalSync(false);
            ofBackgroundGradient(ofColor(160),ofColor(0));
            cam.setNearClip(.01);
            cam.setFarClip(100000);
            cam.begin();
            ofEnableDepthTest();

            Ref[0].drawBackground(cam);
            Ref[0].drawMeshGlass(cam,ofVec3f(0));
            Ref[1].drawMeshGlass(cam, ofVec3f(200, 0, 100));
            
            ofDisableDepthTest();
            cam.end();
            ofPopMatrix();
            return;
        }
        else {
            cam.begin();
            ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
            if (!material_shader) {
//                ambient_light.enable();
                shader->begin();
            }
        }
    }

    
    
    if (show_ray)
        ray.draw();
    if (show_ray_2d)
        ray_2d.draw();
    
    if (show_catmull_curve) {
        if (show_catmull_controls) {
            for (int i = 0 ; i < catmull_nb_pts ; i++) {
                ofSetColor(61,135,255);
                ofDrawEllipse(catmull_rom_pts[2*i], catmull_rom_pts[2*i +1], 8.0f, 8.0f);
                ofSetColor(109,165,255);
                ofDrawBitmapString(i+1, catmull_rom_pts[2*i] - 4.0f, catmull_rom_pts[2*i +1] - 20.0f);
            }
            for (int i = 1 ; i < catmull_nb_pts ; i++) {
                ofSetColor(0, 0, 255);
                ofSetLineWidth(catmull_line_width_outline);
                ofDrawLine(catmull_rom_pts[2*(i-1)], catmull_rom_pts[2*(i-1) +1], catmull_rom_pts[2*i], catmull_rom_pts[2*i +1]);
            }
        }
        ofSetColor(255, 255, 255);
        ofSetLineWidth(catmull_line_width_curve);
        catmull_renderer.draw();
        
    }
    
    if (show_coons_patch) {
        ofSetColor(255, 255, 255);
        ofSetLineWidth(catmull_line_width_curve);
        if (show_coons_controls) {
            ofSetColor(61,135,255);
                    coons_curve_1_renderer.draw();
            ofSetColor(255, 58, 58);
                    coons_curve_2_renderer.draw();
            ofSetColor(238, 0, 255);
                    coons_curve_3_renderer.draw();
            ofSetColor(19, 219, 92);
                    coons_curve_4_renderer.draw();

        }
        ofSetColor(255, 255, 255);
        coons_mesh.draw(render_type);

        if (show_ray) {
            glm::vec3 baricentric_coordinates;
            glm::vec3 surface_normal;
            bool intersects = ray.intersectsMesh(coons_mesh, baricentric_coordinates, surface_normal);
            if (intersects) {
                auto intersection = ray.getOrigin() + ray.getDirection() * baricentric_coordinates.z;
                ofDrawLine(ray.getOrigin(), intersection);
                ofDrawSphere(intersection, 5);
                
                auto reflLight = glm::reflect(ray.getDirection(), surface_normal);
                ofDrawLine(intersection, intersection + 100 * reflLight);

            }
        }
    }
    
    // Draw 2d shapes
    for (index = 0 ; index < shapes.size() ; index++) {
        ofPushMatrix();
        
            ofMultMatrix(shapes[index].transformation_matrix);
            ofPath path = create_path(index, DrawMode::normal);
            path.draw();
        
        if (show_ray_2d) {
            vector<ofPolyline> polys = path.getOutline();
            for (int i = 0 ; i < polys.size() ; i++) {
                ofPolyline poly = polys[i];
                glm::vec2 surface_normal;
                float distance;
                if (ray_2d.intersectsPolyline(poly, distance, surface_normal)) {
                    glm::vec2 intersection = ray_2d.getOrigin() + ray_2d.getDirection() * distance;
                    ofDrawLine(ray_2d.getOrigin(), intersection);
                    
                    auto reflLight = glm::reflect(ray_2d.getDirection(), surface_normal);
                    ofSetColor(ofColor(238, 238, 153, 75));
                    ofDrawLine(intersection, intersection + 100 * reflLight);
                }
            }
        }

        
        ofPopMatrix();
    }
    
    // Draw selected 2d shapes
    for (index = 0 ; index < selected_shapes_index.size() ; index++) {
        ofPushMatrix();
        
            ofMultMatrix(shapes[selected_shapes_index[index]].transformation_matrix);
            ofPath path = create_path(selected_shapes_index[index], DrawMode::selected);
            path.draw();
        
        ofPopMatrix();
        
    }
    
    // Draw 3d shapes
    for (index = 0 ; index < mesh_vector.size() ; index ++) {
        ofVboMesh mesh = create_3d_mesh(index, DrawMode::normal);
        if (show_bounding_box) {
            std::vector<ofPath> box = get_bounding_box(mesh);
            for (int i = 0 ; i < box.size() ; i++) {
                box[i].draw();
            }
        }
        if (activate_tessellation) {
            shader_tessellation.begin();
            shader_tessellation.setUniform1f("u_tessLevelInner", tessLevelInner);
            shader_tessellation.setUniform1f("u_tessLevelOuter", tessLevelOuter);
            mesh.getVbo().drawElements(GL_PATCHES, mesh.getNumIndices());
            shader_tessellation.end();
        } else
            if (material_shader) {
                switch (material_current) {
                case (MaterialType::chrome):
                    material_chrome.begin();
                    mesh.draw(render_type);
                    material_chrome.end();
                    break;
                case (MaterialType::plastic):
                    material_plastic.begin();
                    mesh.draw(render_type);
                    material_plastic.end();
                    break;
                case (MaterialType::rubber):
                    material_rubber.begin();
                    mesh.draw(render_type);
                    material_rubber.end();
                    break;
                case (MaterialType::brass):
                    material_brass.begin();
                    mesh.draw(render_type);
                    material_brass.end();
                    break;
                case (MaterialType::none):
                    mesh.draw(render_type);
                    break;
                }
                mesh.draw(render_type);
            } else {
                mesh.disableTextures();
                mesh.draw(render_type);
            }
        if (show_ray) {
            glm::vec3 baricentric_coordinates;
            glm::vec3 surface_normal;
            bool intersects = ray.intersectsMesh(mesh, baricentric_coordinates, surface_normal);
            if (intersects) {
                auto intersection = ray.getOrigin() + ray.getDirection() * baricentric_coordinates.z;
                ofDrawLine(ray.getOrigin(), intersection);
                ofDrawSphere(intersection, 2);
                
                auto reflLight = glm::reflect(ray.getDirection(), surface_normal);
                ofSetColor(ofColor(238, 238, 153, 75));
                ofDrawLine(intersection, intersection + 100 * reflLight);
            }
        }

    }
    
    // Draw 3d models
    for (index = 0 ; index < model_vector.size() ; index ++) {
        if (show_bounding_box) {
            std::vector<ofPath> box = model_vector[index].box;
            for (int i = 0 ; i < box.size() ; i++) {
                box[i].draw();
            }
        }
        if (show_texture and model_vector[index].texture.isAllocated()) {
            model_vector[index].texture.bind();
            model_vector[index].model.draw(render_type);
            model_vector[index].texture.unbind();
        } else {
            if ((model_vector[index].name == "heart" || model_vector[index].name == "football" || model_vector[index].name == "big_hero_six" ) && material_shader) {
                switch (material_current) {
                    case (MaterialType::chrome):
                        material_chrome.begin();
                        model_vector[index].model.draw(render_type);
                        material_chrome.end();
                        break;
                    case (MaterialType::plastic):
                        material_plastic.begin();
                        model_vector[index].model.draw(render_type);
                        material_plastic.end();
                        break;
                    case (MaterialType::rubber):
                        material_rubber.begin();
                        model_vector[index].model.draw(render_type);
                        material_rubber.end();
                        break;
                    case (MaterialType::brass):
                        material_brass.begin();
                        model_vector[index].model.draw(render_type);
                        material_brass.end();
                        break;
                    case (MaterialType::none):
                        model_vector[index].model.draw(render_type);
                        break;
                };
            } else {
                model_vector[index].model.disableMaterials();
                model_vector[index].model.draw(render_type);
            }
        }
    }
    
    // Show drag zone / selection
    if (is_mouse_button_pressed && !show_histo && !show_cam)
    {
        draw_zone(
                  mouse_press_x,
                  mouse_press_y,
                  mouse_current_x,
                  mouse_current_y);
    }
    
    if (!show_cam)
        draw_cursor(mouse_current_x, mouse_current_y);
    
    if (show_cam) {
        if (material_shader)
            shader->end();
        cam.end();
    }
}

// Clear 2d shapes vector
void Renderer::reset()
{
    shapes.clear();
    selected_shapes_index.clear();
    mesh_vector.clear();
    model_vector.clear();
    
    head_2d = 0;
    head_3d = 0;
    head_3d_models = 0;
}

Renderer::~Renderer()
{
    
}

// --------- Cursor --------- //

// Render cursor shape
void Renderer::draw_cursor(float x, float y) const
{
    if (show_histo)
        return;
    
    ofSetLineWidth(2);
    ofSetColor(31);
    float offset = 5.0f;
    float length = 10.0f;
    switch (current_mode) {
        case ToolMode::vector_draw:
            ofDrawLine(x + offset, y - offset, x + offset + length, y - offset);
            ofDrawLine(x + offset + length,  y - offset, x + offset + length, y - offset + length);
            ofDrawLine(x + offset, y + offset, x + offset + length, y + offset);
            ofDrawLine(x + offset, y + offset, x + offset, y - offset);
            break;
        case ToolMode::mesh_draw:
            ofDrawLine(x + offset, y - offset, x + offset + length, y - offset);
            ofDrawLine(x + offset + length, y - offset, x + offset + length, y - offset + length);
            ofDrawLine(x + offset, y + offset, x + offset + length, y + offset);
            ofDrawLine(x + offset, y + offset, x + offset, y - offset);
            ofDrawLine(x + 2*offset, y - 2*offset, x + 2*offset + length, y - 2*offset);
            ofDrawLine(x + 2*offset + length, y - 2*offset, x + 2*offset + length, y);
            ofDrawLine(x + 2*offset, y, x + 2*offset + length, y);
            ofDrawLine(x + 2*offset, y, x + 2*offset, y - 2*offset);
            break;
        case ToolMode::model_draw:
            ofDrawLine(x + offset, y - offset, x + offset + length, y - offset);
            ofDrawLine(x + offset + length, y - offset, x + offset + length, y - offset + length);
            ofDrawLine(x + offset, y + offset, x + offset + length, y + offset);
            ofDrawLine(x + offset, y + offset, x + offset, y - offset);
            ofDrawLine(x + 2*offset, y - 2*offset, x + 2*offset + length, y - 2*offset);
            ofDrawLine(x + 2*offset + length, y - 2*offset, x + 2*offset + length, y);
            ofDrawLine(x + 2*offset, y, x + 2*offset + length, y);
            ofDrawLine(x + 2*offset, y, x + 2*offset, y - 2*offset);
            break;
        case ToolMode::select:
            ofDrawLine(x + offset, y - offset, x + length, y - offset);
            ofDrawLine(x + length + offset, y - offset, x + 2*length, y - offset);
            
            ofDrawLine(x + offset, y - 0.5*offset, x+ offset, y + 0.5*offset);
            ofDrawLine(x + 2*length, y - 0.5*offset, x+ 2*length, y + 0.5*offset);
            
            ofDrawLine(x + offset, y + offset, x + length, y + offset);
            ofDrawLine(x + length + offset, y + offset, x + 2*length, y + offset);
            break;
        case ToolMode::rotate:
            ofNoFill();
            ofDrawCircle(x + length, y, length);
            ofDrawLine(x + length + 2*offset, y, x + length + offset, y - offset);
            ofDrawLine(x + length + 2*offset, y, x + 2*length + offset, y - offset);
            break;
        case ToolMode::scale:
            ofDrawLine(x + offset, y - offset, x + offset + length, y - offset);
            ofDrawLine(x + offset + 0.5 * length, y - offset - 0.5 * length, x + offset + 0.5 * length, y - offset + 0.5 * length);
            ofDrawLine(x + length, y + offset, x + offset + 1.5*length, y - offset);
            ofDrawLine(x + length + offset, y + offset, x + length + offset + length, y + offset);
            break;
        case ToolMode::translate:
            ofDrawLine(x + offset, y - offset, x + offset + length, y - offset);
            ofDrawLine(x + offset + length, y - offset, x + length, y - length);
            ofDrawLine(x + offset + length, y - offset, x + length, y);
            break;
        default:
            ofDrawLine(x + offset, y, x + offset + length, y);
            ofDrawLine(x - offset, y, x - offset - length, y);
            ofDrawLine(x, y + offset, x, y + offset + length);
            ofDrawLine(x, y - offset, x, y - offset - length);
            break;
    }
}

// Draw drag zone for current shape
void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float) ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float) ofGetHeight());
    
    float diameter = max(x2 - x1, y2 - y1);
    float diameter_x = x2 - x1;
    float diameter_y = y2 - y1;
    
    switch (current_mode) {
        case ToolMode::vector_draw:
            
            // 2D Draw tool drag zone
            switch (draw_mode){
                case VectorPrimitiveType::iso_triangle: {
                    ofPath path;
                    
                    float dx = 2 * mouse_current_x - mouse_press_x;
                    path.moveTo(mouse_press_x, mouse_press_y);
                    path.lineTo(mouse_current_x, mouse_current_y);
                    path.lineTo(dx, mouse_press_y);
                    path.close();
                    
                    path.setStrokeColor(stroke_color);
                    path.setStrokeWidth(stroke_width);
                    path.setFillColor(fill_color);
                    path.setFilled(false);
                    
                    path.draw();
                    break;
                }
                case VectorPrimitiveType::line: {
                    ofPath path;
                    path.moveTo(mouse_press_x, mouse_press_y);
                    path.lineTo(mouse_current_x, mouse_current_y);
                    path.close();
                    
                    path.setStrokeColor(stroke_color);
                    path.setStrokeWidth(stroke_width);
                    path.setFilled(false);
                    
                    path.draw();
                    break;
                }
                case VectorPrimitiveType::rectangle:
                {
                    ofPath path;
                    path.moveTo(mouse_press_x, mouse_press_y);
                    path.lineTo(mouse_current_x, mouse_press_y);
                    path.lineTo(mouse_current_x, mouse_current_y);
                    path.lineTo(mouse_press_x, mouse_current_y);
                    path.close();
                    
                    path.setStrokeColor(stroke_color);
                    path.setStrokeWidth(stroke_width);
                    path.setFillColor(fill_color);
                    path.setFilled(false);
                    
                    path.draw();
                    break;
                }
                case VectorPrimitiveType::circle:
                {
                    ofPath path;
                    float distance = sqrt(pow((mouse_current_x - mouse_press_x), 2.0f) + pow((mouse_current_y - mouse_press_y), 2.0f));
                    path.moveTo(mouse_press_x, mouse_press_y);
                    path.setCircleResolution(200);
                    path.circle(mouse_press_x, mouse_press_y, distance);
                    path.close();
                    
                    path.setStrokeColor(stroke_color);
                    path.setStrokeWidth(stroke_width);
                    path.setFillColor(fill_color);
                    path.setFilled(false);
                    
                    path.draw();
                    break;
                }
                case VectorPrimitiveType::polygone:
                {
                    ofPath path;
                    float angle = 2 * pi / polygone_sides;
                    float radius = sqrt(pow((mouse_current_x - mouse_press_x), 2.0f) + pow((mouse_current_y - mouse_press_y), 2.0f));
                    
                    for (int n = 0 ; n < polygone_sides ; n++) {
                        if (n == 0) {
                            path.moveTo(mouse_press_x + radius * cos(n * angle), mouse_press_y + radius * sin(n * angle));
                        } else {
                            path.lineTo(mouse_press_x + radius * cos(n * angle), mouse_press_y + radius * sin(n * angle));
                        }
                    }
                    path.close();
                    
                    path.setStrokeColor(stroke_color);
                    path.setStrokeWidth(stroke_width);
                    path.setFillColor(fill_color);
                    path.setFilled(false);
                    
                    path.draw();
                    break;
                }
                default:
                    break;
            }
            break;
        case ToolMode::mesh_draw:
            ofSetLineWidth(5.0f);
            ofSetColor(stroke_color);
            ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
            ofFill();
            ofDrawEllipse(x1, y1, radius, radius);
            ofDrawEllipse(x1, y2_clamp, radius, radius);
            ofDrawEllipse(x2_clamp, y1, radius, radius);
            ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
            break;
            
            // Selection tool drag zone
        case ToolMode::select:
            ofSetLineWidth(5.0f);
            ofSetColor(selected_color);
            ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
            ofFill();
            ofDrawEllipse(x1, y1, radius, radius);
            ofDrawEllipse(x1, y2_clamp, radius, radius);
            ofDrawEllipse(x2_clamp, y1, radius, radius);
            ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
            break;
            
        default:
            break;
    }
}

// Select the topmost shape if multiple shapes are stacked at (x,y)
void Renderer::select_single(float x, float y) {
    selected_shapes_index.clear();
    
    int selected_index;
    bool is_selected;
    for (index = 0 ; index < shapes.size() ; index++) {
        is_selected = false;
        ofPath path = create_path(index, DrawMode::normal);
        std::vector<ofPolyline> polyline_vector = path.getOutline();
        
//        for (auto i = 0 ; i < polyline_vector.size() ; i++) {
            ofRectangle bounding_box = polyline_vector[0].getBoundingBox();
            float x1 = bounding_box.getMinX();
            float y1 = bounding_box.getMinY();
            float x2 = bounding_box.getMaxX();
            float y2 = bounding_box.getMaxY();
            
            ofVec3f point1(x1, y1, 0);
            ofVec3f point2(x2, y2, 0);
            
            // Tranform points to account for transformation matrix during selection
            ofVec3f t_point1 = point1 * shapes[index].transformation_matrix;
            ofVec3f t_point2 = point2 * shapes[index].transformation_matrix;
            
            if (x >= t_point1.x and x <= t_point2.x and y >= t_point1.y and y <= t_point2.y)
                is_selected = true;
//        }
        if (is_selected) {
            selected_shapes_index.push_back(index);
        }
    }
}

// Select all shapes completely contained within the rectangle formed by passed coordinates
void Renderer::select_multiple(float sx1, float sy1, float sx2, float sy2)
{
    selected_shapes_index.clear();
    
    for (index = 0 ; index < shapes.size() ; index++) {
        ofPath path = create_path(index, DrawMode::normal);
        std::vector<ofPolyline> polyline_vector = path.getOutline();
        bool is_selected = true;
        
        for (auto i = 0 ; i < polyline_vector.size() ; i++) {
            ofRectangle bounding_box = polyline_vector[i].getBoundingBox();
            
            float x1 = bounding_box.getMinX();
            float y1 = bounding_box.getMinY();
            float x2 = bounding_box.getMaxX();
            float y2 = bounding_box.getMaxY();
            
            ofVec3f point1(x1, y1, 0);
            ofVec3f point2(x2, y2, 0);
            
            // Tranform points to account for transformation matrix during selection
            ofVec3f t_point1 = point1 * shapes[index].transformation_matrix;
            ofVec3f t_point2 = point2 * shapes[index].transformation_matrix;
            
            // Top to bottom drag
            if ((t_point1.x >= sx1 && t_point1.x <= sx2 && t_point1.y >= sy1 && t_point1.y <= sy2) && (t_point2.x >= sx1 && t_point2.x <= sx2 && t_point2.y >= sy1 && t_point2.y <= sy2)) {
                is_selected = is_selected && true;
                // Bottom to top drag
            } else if ((t_point1.x <= sx1 && t_point1.x >= sx2 && t_point1.y <= sy1 && t_point1.y >= sy2) && (t_point2.x <= sx1 && t_point2.x >= sx2 && t_point2.y <= sy1 && t_point2.y >= sy2)) {
                is_selected = is_selected && true;
            } else {
                is_selected = is_selected && false;
            }
        }
        
        if (is_selected) {
            selected_shapes_index.push_back(index);
        }
    };
}

// --------- 2d Shapes --------- //

// Add 2d shape to vector
void Renderer::add_vector_shape(VectorPrimitiveType type)
{
    VectorPrimitive new_shape;
    
    new_shape.type = type;
    if (type == VectorPrimitiveType::polygone)
        new_shape.sides = polygone_sides;
    new_shape.position1[0] = mouse_press_x;
    new_shape.position1[1] = mouse_press_y;
    new_shape.position2[0] = mouse_current_x;
    new_shape.position2[1] = mouse_current_y;

    new_shape.fill_color[0] = fill_color.r;
    new_shape.fill_color[1] = fill_color.g;
    new_shape.fill_color[2] = fill_color.b;
    new_shape.fill_color[3] = fill_color.a;
    new_shape.stroke_color[0] = stroke_color.r;
    new_shape.stroke_color[1] = stroke_color.g;
    new_shape.stroke_color[2] = stroke_color.b;
    new_shape.stroke_color[3] = stroke_color.a;
    new_shape.stroke_width = stroke_width;
    
    // Keep number of shapes lower than set max count
    if (shapes.size() == max_count_2d) {
        shapes.at(head_2d) = new_shape;
    } else {
        shapes.push_back(new_shape);
    }
    
    ofLog() << "<new primitive at index: " << head_2d << ">";
    
    head_2d = ++head_2d >= max_count_2d ? 0: head_2d; // boucler sur le tableau si plein
}

// Create a ofPath for a given index, referring to a shape from the shapes vector
ofPath Renderer::create_path(int index, DrawMode mode) {
    
    ofPath path;
    path.setMode(ofPath::POLYLINES);
    switch (shapes[index].type) {
        case VectorPrimitiveType::line:
        {
            path.moveTo(shapes[index].position1[0], shapes[index].position1[1]);
            path.lineTo(shapes[index].position2[0], shapes[index].position2[1]);
            path.close();
            
            if (mode == DrawMode::selected) {
                path.setStrokeColor(selected_color);
            } else {
                path.setStrokeColor(ofColor(
                                            shapes[index].stroke_color[0],
                                            shapes[index].stroke_color[1],
                                            shapes[index].stroke_color[2],
                                            shapes[index].stroke_color[3]));
            }
            
            path.setStrokeWidth(shapes[index].stroke_width);
            path.setFilled(false);
            
            break;
        }
        case VectorPrimitiveType::iso_triangle:
        {
            float dx = 2 * shapes[index].position2[0] - shapes[index].position1[0];
            path.moveTo(shapes[index].position1[0], shapes[index].position1[1]);
            path.lineTo(shapes[index].position2[0], shapes[index].position2[1]);
            path.lineTo(dx, shapes[index].position1[1]);
            path.close();
            
            if (mode == DrawMode::selected) {
                path.setStrokeColor(selected_color);
                path.setFillColor(selected_color);
            } else {
                path.setStrokeColor(ofColor(
                                            shapes[index].stroke_color[0],
                                            shapes[index].stroke_color[1],
                                            shapes[index].stroke_color[2],
                                            shapes[index].stroke_color[3]));
                path.setFillColor(ofColor(
                                          shapes[index].fill_color[0],
                                          shapes[index].fill_color[1],
                                          shapes[index].fill_color[2],
                                          shapes[index].fill_color[3]));
            }
            path.setStrokeWidth(shapes[index].stroke_width);
            path.setFilled(true);
            
            break;
        }
        case VectorPrimitiveType::rectangle:
        {
            path.moveTo(shapes[index].position1[0], shapes[index].position1[1]);
            path.lineTo(shapes[index].position1[0], shapes[index].position2[1]);
            path.lineTo(shapes[index].position2[0], shapes[index].position2[1]);
            path.lineTo(shapes[index].position2[0], shapes[index].position1[1]);
            path.close();
            
            if (mode == DrawMode::selected) {
                path.setStrokeColor(selected_color);
                path.setFillColor(selected_color);
            } else {
                path.setStrokeColor(ofColor(
                                            shapes[index].stroke_color[0],
                                            shapes[index].stroke_color[1],
                                            shapes[index].stroke_color[2],
                                            shapes[index].stroke_color[3]));
                path.setFillColor(ofColor(
                                          shapes[index].fill_color[0],
                                          shapes[index].fill_color[1],
                                          shapes[index].fill_color[2],
                                          shapes[index].fill_color[3]));
            }
            
            path.setStrokeWidth(shapes[index].stroke_width);
            path.setFilled(true);
            
            break;
        }
        case VectorPrimitiveType::polygone:
        {
            float angle = 2 * pi / shapes[index].sides;
            float radius = sqrt(pow((shapes[index].position2[0] - shapes[index].position1[0]), 2.0f) + pow((shapes[index].position2[1] - shapes[index].position1[1]), 2.0f));
            
            for (int n = 0 ; n < shapes[index].sides ; n++) {
                if (n == 0) {
                    path.moveTo(shapes[index].position1[0] + radius * cos(n * angle), shapes[index].position1[1] + radius * sin(n * angle));
                } else {
                    path.lineTo(shapes[index].position1[0] + radius * cos(n * angle), shapes[index].position1[1] + radius * sin(n * angle));
                }
            }
            path.close();
            
            if (mode == DrawMode::selected) {
                path.setStrokeColor(selected_color);
                path.setFillColor(selected_color);
            } else {
                path.setStrokeColor(ofColor(
                                            shapes[index].stroke_color[0],
                                            shapes[index].stroke_color[1],
                                            shapes[index].stroke_color[2],
                                            shapes[index].stroke_color[3]));
                path.setFillColor(ofColor(
                                          shapes[index].fill_color[0],
                                          shapes[index].fill_color[1],
                                          shapes[index].fill_color[2],
                                          shapes[index].fill_color[3]));
            }
            
            path.setStrokeWidth(shapes[index].stroke_width);
            path.setFilled(true);
            
            break;
        }
        case VectorPrimitiveType::circle:
        {
            path.setCircleResolution(200);
            path.moveTo(shapes[index].position1[0], shapes[index].position1[1]);
            path.circle(shapes[index].position1[0], shapes[index].position1[1], find_distance(shapes[index].position1[0], shapes[index].position1[1], shapes[index].position2[0], shapes[index].position2[1]));
            path.close();
            
            if (mode == DrawMode::selected) {
                path.setStrokeColor(selected_color);
                path.setFillColor(selected_color);
            } else {
                path.setStrokeColor(ofColor(
                                            shapes[index].stroke_color[0],
                                            shapes[index].stroke_color[1],
                                            shapes[index].stroke_color[2],
                                            shapes[index].stroke_color[3]));
                path.setFillColor(ofColor(
                                          shapes[index].fill_color[0],
                                          shapes[index].fill_color[1],
                                          shapes[index].fill_color[2],
                                          shapes[index].fill_color[3]));
            }
            
            path.setStrokeWidth(shapes[index].stroke_width);
            path.setFilled(true);
            
            break;
        }
        default:
            break;
    }
    return path;
}

// Scale all shapes currently selected
void Renderer::apply_scale() {
    std::tuple<float, float> center = find_center_selection();
    
    VectorPrimitive& shape = shapes[selected_shapes_index[index]];
    
    ofMatrix4x4 pre_translation;
    pre_translation.makeTranslationMatrix(get<0>(center), get<1>(center), 0);
    
    ofMatrix4x4 scale;
    scale.makeScaleMatrix(scale_ratio_x, scale_ratio_y, 0);
    
    ofMatrix4x4 post_translation;
    post_translation.makeTranslationMatrix(-get<0>(center), -get<1>(center), 0);
    
    ofMatrix4x4 transformation;
    transformation.preMult(pre_translation);
    transformation.preMult(scale);
    transformation.preMult(post_translation);
    
    UndoStruct undo_info;
    
    for (index = 0 ; index < selected_shapes_index.size() ; index++){
        VectorPrimitive& shape = shapes[selected_shapes_index[index]];
        shape.transformation_matrix.preMult(transformation);
        undo_info.indexes.push_back(index);
    }
    
    undo_info.matrix = transformation;
    undo_stack.push(undo_info);
}

// Translate all selected shapes
void Renderer::apply_translation() {
    for (index = 0 ; index < selected_shapes_index.size() ; index++){
        VectorPrimitive& shape = shapes[selected_shapes_index[index]];
        
        // Decompose current transformation matrix to extract rotation
        ofVec3f c_translation;
        ofQuaternion c_rotation;
        ofVec3f c_scale;
        ofQuaternion c_scale_orientation;
        shape.transformation_matrix.decompose(c_translation, c_rotation, c_scale, c_scale_orientation);
        
        ofMatrix4x4 m_rotation;
        m_rotation.makeRotationMatrix(c_rotation);
        
        // Adjust translation to current rotation
        ofVec3f v_translation(translation_x, translation_y, translation_z);
        v_translation = v_translation * m_rotation.getInverse();
        
        // Translate
        ofMatrix4x4 translation;
        translation.makeTranslationMatrix(v_translation.x, v_translation.y, v_translation.z);
        shape.transformation_matrix.preMult(translation);
    }
}

// Rotate all shapes currently selected
void Renderer::apply_rotation() {
    std::tuple<float, float> center = find_center_selection();
    
    ofMatrix4x4 pre_translation;
    pre_translation.makeTranslationMatrix(get<0>(center), get<1>(center), 0);
    
    ofMatrix4x4 rotation;
    rotation.makeRotationMatrix(rotation_angle, ofVec3f(0, 0, 1));
    
    ofMatrix4x4 post_translation;
    post_translation.makeTranslationMatrix(-get<0>(center), -get<1>(center), 0);
    
    ofMatrix4x4 transformation;
    transformation.preMult(pre_translation);
    transformation.preMult(rotation);
    transformation.preMult(post_translation);
    
    UndoStruct undo_info;
    
    for (index = 0 ; index < selected_shapes_index.size() ; index++){
        VectorPrimitive& shape = shapes[selected_shapes_index[index]];
        shape.transformation_matrix.preMult(transformation);
        undo_info.indexes.push_back(index);
    }
    undo_info.matrix = transformation;
    undo_stack.push(undo_info);
}

// Find center of currently selected group
std::tuple<float, float> Renderer::find_center_selection() {
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::min();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::min();
    
    float center_x;
    float center_y;
    
    for (index = 0 ; index < selected_shapes_index.size() ; index++){
        ofPath path = create_path(selected_shapes_index[index], DrawMode::normal);
        
        std::vector<ofPolyline> polyline_vector = path.getOutline();
        for (auto i = 0 ; i < polyline_vector.size() ; i++) {
            ofRectangle bounding_box = polyline_vector[i].getBoundingBox();
            
            min_x = min(min_x, bounding_box.getMinX());
            max_x = max(max_x, bounding_box.getMaxX());
            min_y = min(min_y, bounding_box.getMinY());
            max_y = max(max_y, bounding_box.getMaxY());
        }
    }
    
    center_x = (min_x + max_x) / 2.0f;
    center_y = (min_y + max_y) / 2.0f;
    
    return make_tuple(center_x, center_y);
}

// Find distance between to pairs of coordinates x,y
float Renderer::find_distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow((x2 - x1), 2.0f) + pow((y2 - y1), 2.0f));
}

void Renderer::undo_transformation() {
    if (undo_stack.size() >= 1) {
        UndoStruct undo_info = undo_stack.top();
        for (index = 0 ; index < undo_info.indexes.size() ; index++) {
            shapes[undo_info.indexes[index]].transformation_matrix.preMult(undo_info.matrix.getInverse());
        }
        redo_stack.push(undo_info);
        undo_stack.pop();
    }
}

void Renderer::redo_transformation() {

}

// Delete all shapes currently selected
void Renderer::delete_shape() {
    std::sort(selected_shapes_index.begin(), selected_shapes_index.end());
    std::reverse(selected_shapes_index.begin(), selected_shapes_index.end());
    
    for (index = 0 ; index < selected_shapes_index.size() ; index++) {
        shapes.erase(shapes.begin() + selected_shapes_index[index]);
        if (head_2d >= selected_shapes_index[index] && head_2d > 0) {
            head_2d--;
        }
    }
    selected_shapes_index.clear();
}

void Renderer::apply_filter() {

    // résolution du kernel de convolution
    const int kernel_size = 3;
    
    // décalage à partir du centre du kernel
    const int kernel_offset = kernel_size / 2;
    
    // nombre de composantes de couleur (RGB)
    const int color_component_count = 3;
    
    // indices de l'image
    int x, y;
    
    // indices du kernel
    int i, j;
    
    // index des composantes de couleur
    int c;
    
    // index du pixel de l'image source utilisé pour le filtrage
    int pixel_index_img_src;
    
    // index du pixel de l'image de destination en cours de filtrage
    int pixel_index_img_dst;
    
    // index du pixel de l'image de destination en cours de filtrage
    int kernel_index;
    
    // valeur à un des indices du kernel de convolution
    float kernel_value;
    
    // extraire les pixels de l'image source
    ofPixels pixel_array_src = in.getPixels();
    
    // extraire les pixels de l'image de destination
    ofPixels pixel_array_dst = out.getPixels();
    
    // couleur du pixel lu dans l'image source
    ofColor pixel_color_src;
    
    // couleur du pixel à écrire dans l'image de destination
    ofColor pixel_color_dst;
    
    // somme du kernel appliquée à chaque composante de couleur d'un pixel
    float sum[color_component_count];
    
    // itération sur les rangées des pixels de l'image source
    for (y = 0; y < image_height; ++y)
    {
        // itération sur les colonnes des pixels de l'image source
        for (x = 0; x < image_width; ++x)
        {
            // initialiser le tableau où les valeurs de filtrage sont accumulées
            for (c = 0; c < color_component_count; ++c)
                sum[c] = 0;
            
            // déterminer l'index du pixel de l'image de destination
            pixel_index_img_dst = (image_width * y + x) * color_component_count;
            
            // itération sur les colonnes du kernel de convolution
            for (j = -kernel_offset; j <= kernel_offset; ++j)
            {
                // itération sur les rangées du kernel de convolution
                for (i = -kernel_offset; i <= kernel_offset; ++i)
                {
                    // déterminer l'index du pixel de l'image source à lire
                    pixel_index_img_src = (image_width * (y-j) + (x-i)) * color_component_count;
                    
                    // lire la couleur du pixel de l'image source
                    pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);
                    
                    // déterminer l'indice du facteur à lire dans le kernel de convolution
                    kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);
                    
                    // extraction de la valeur à cet index du kernel
                    switch (kernel_type)
                    {
                        case ConvolutionKernel::identity:
                            kernel_value = convolution_kernel_identity.at(kernel_index);
                            break;
                            
                        case ConvolutionKernel::emboss:
                            kernel_value = convolution_kernel_emboss.at(kernel_index);
                            break;
                            
                        case ConvolutionKernel::sharpen:
                            kernel_value = convolution_kernel_sharpen.at(kernel_index);
                            break;
//
//                        case ConvolutionKernel::edge_detect:
//                            kernel_value = convolution_kernel_edge_detect.at(kernel_index);
//                            break;
                            
                        case ConvolutionKernel::blur:
                            kernel_value = convolution_kernel_blur.at(kernel_index);
                            break;
                            
                        default:
                            kernel_value = convolution_kernel_identity.at(kernel_index);
                            break;
                    }
                    
                    // itération sur les composantes de couleur
                    for (c = 0; c < color_component_count; ++c)
                    {
                        // accumuler les valeurs de filtrage en fonction du kernel de convolution
                        sum[c] = sum[c] + kernel_value * pixel_color_src[c];
                    }
                }
            }
            
            // déterminer la couleur du pixel à partir des valeurs de filtrage accumulées pour chaque composante
            for (c = 0; c < color_component_count; ++c)
            {
                // conversion vers entier et validation des bornes de l'espace de couleur
                pixel_color_dst[c] = (int) ofClamp(sum[c], 0, 255);
            }
            
            // écrire la couleur à l'index du pixel en cours de filtrage
            pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
        }
    }
    
    // écrire les pixels dans l'image de destination
    out.setFromPixels(pixel_array_dst);
    
    ofLog() << "<convolution filter done>";
}

// --------- 3d Shapes --------- //

// Add 3d mesh to vector
void Renderer::add_mesh(MeshPrimitiveType type)
{
    MeshPrimitive new_mesh;
    
    new_mesh.type = type;
    new_mesh.position1[0] = mouse_press_x;
    new_mesh.position1[1] = mouse_press_y;
    new_mesh.position1[2] = 0;
    new_mesh.position2[0] = mouse_current_x;
    new_mesh.position2[1] = mouse_current_y;
    new_mesh.position2[2] = 0;
    
    new_mesh.stroke_width = stroke_width;
    new_mesh.stroke_color[0] = stroke_color.r;
    new_mesh.stroke_color[1] = stroke_color.g;
    new_mesh.stroke_color[2] = stroke_color.b;
    new_mesh.stroke_color[3] = stroke_color.a;
    new_mesh.fill_color[0] = fill_color.r;
    new_mesh.fill_color[1] = fill_color.g;
    new_mesh.fill_color[2] = fill_color.b;
    new_mesh.fill_color[3] = fill_color.a;
    
    // Keep number of meshes lower than set max count
    if (mesh_vector.size() == max_count_3d) {
        mesh_vector.at(head_3d) = new_mesh;
    } else {
        mesh_vector.push_back(new_mesh);
    }
    
    ofLog() << "<new primitive at index: " << head_3d << ">";
    
    head_3d = ++head_3d >= max_count_3d ? 0: head_3d; // restart at 0 if max count reached
}

ofVboMesh Renderer::create_3d_mesh(int index, DrawMode mode) {
    ofVboMesh mesh;
    
    switch (mesh_vector[index].type) {
        case MeshPrimitiveType::cube:
        {
            
            ofFill();
            ofSetLineWidth(2.0f);
            ofSetColor(127);
            int x1 = mesh_vector[index].position1[0];
            int y1 = mesh_vector[index].position1[1];
            int z1 = mesh_vector[index].position1[2];
            int x2 = mesh_vector[index].position2[0];
            int y2 = mesh_vector[index].position2[1];
            int z2 = mesh_vector[index].position2[2];

            // Caculate position of cube
            int side_length = max(abs(x2-x1), abs(y2-y1));
            x1 > x2 ? x2 = x1 - side_length : x2 = x1 + side_length;
            y1 > y2 ? y2 = y1 - side_length : y2 = y1 + side_length;
            z1 > z2 ? z2 = z1 - side_length : z2 = z1 + side_length;
            

//            // Algorithm inspired by
//            // http://ilkinulas.github.io/development/unity/2016/04/30/cube-mesh-in-unity3d.html

            ofPoint points[8] = {
                ofPoint(x1, y1, z1),
                ofPoint(x2, y1, z1),
                ofPoint(x2, y2, z1),
                ofPoint(x1, y2, z1),
                ofPoint(x1, y2, z2),
                ofPoint(x2, y2, z2),
                ofPoint(x2, y1, z2),
                ofPoint(x1, y1, z2)
            };

            for (int i = 0 ; i < 8 ; i++) {
                mesh.addVertex(points[i]);
            }

            mesh.addTriangle(0, 2, 1);
            mesh.addTriangle(0, 3, 2);
            mesh.addTriangle(2, 3, 4);
            mesh.addTriangle(2, 4, 5);
            mesh.addTriangle(1, 2, 5);
            mesh.addTriangle(1, 5, 6);
            mesh.addTriangle(0, 7, 4);
            mesh.addTriangle(0, 4, 3);
            mesh.addTriangle(5, 4, 7);
            mesh.addTriangle(5, 7, 6);
            mesh.addTriangle(0, 6, 7);
            mesh.addTriangle(0, 1, 6);

            break;
        }
        case MeshPrimitiveType::icosahedron:
        {
            ofFill();
            ofSetLineWidth(2.0f);
            ofSetColor(127);
            int x1 = mesh_vector[index].position1[0];
            int y1 = mesh_vector[index].position1[1];
            int z1 = mesh_vector[index].position1[2];
            int x2 = mesh_vector[index].position2[0];
            int y2 = mesh_vector[index].position2[1];
            int z2 = mesh_vector[index].position2[2];
            
            // Caculate
            int radius = max(abs(x2-x1), abs(y2-y1)) / 2.0;
            float phi = (1.0 + sqrt(5.0)) / 2.0f;
            float golden_ratio = sqrt(10.0 + (2.0f * sqrt(5.0f))) / (4.0f * phi);
            int a = (radius / golden_ratio) / 2.0f;
            int b = (radius / golden_ratio) / (2.0f * phi);
            
            // Algorithm inspired by
            // https://wiki.mcneel.com/developer/scriptsamples/icosahedron
            
            ofPoint points[12] = {
                ofPoint(x1, y1 + b, z1 - a),
                ofPoint(x1 + b, y1 + a, z1),
                ofPoint(x1 - b, y1 + a, z1),
                ofPoint(x1, y1 + b, z1 + a),
                
                ofPoint(x1, y1 - b, z1 + a),
                ofPoint(x1 - a, y1, z1 + b),
                ofPoint(x1, y1 - b, z1 - a),
                ofPoint(x1 + a, y1, z1 - b),
                
                ofPoint(x1 + a, y1, z1 + b),
                ofPoint(x1 - a, y1, z1 - b),
                ofPoint(x1 + b, y1 - a, z1),
                ofPoint(x1 - b, y1 - a, z1)
            };
            
            for (int i = 0 ; i < 12 ; i++) {
                mesh.addVertex(points[i]);
            }
            
            mesh.addTriangle(0, 1, 2);
            mesh.addTriangle(3, 2, 1);
            mesh.addTriangle(3, 4, 5);
            mesh.addTriangle(3, 8, 4);
            mesh.addTriangle(0, 6, 7);
            
            mesh.addTriangle(0, 9, 6);
            mesh.addTriangle(4, 10, 11);
            mesh.addTriangle(6, 11, 10);
            mesh.addTriangle(2, 5, 9);
            mesh.addTriangle(11, 9, 5);
            
            mesh.addTriangle(1, 7, 8);
            mesh.addTriangle(10, 8, 7);
            mesh.addTriangle(3, 5, 2);
            mesh.addTriangle(3, 1, 8);
            mesh.addTriangle(0, 2, 9);
            
            mesh.addTriangle(0, 7, 1);
            mesh.addTriangle(6, 9, 11);
            mesh.addTriangle(6, 10, 7);
            mesh.addTriangle(4, 11, 5);
            mesh.addTriangle(4, 8, 10);
        
            break;
        }
        default:
            break;
    }
    return mesh;
}

// -------- 3d Model -------- //

void Renderer::add_3d_model(ModelType model) {
    Model model_struct;
    model_struct.position1[0] = mouse_press_x;
    model_struct.position1[1] = mouse_press_y;
    model_struct.position1[2] = 0;
    model_struct.position2[0] = mouse_current_x;
    model_struct.position2[1] = mouse_current_y;
    model_struct.position2[2] = 0;
    switch (model) {
        case ModelType::big_hero_six:
            model_struct.model = big_hero_six;
            model_struct.model.setScale(0.5, 0.5, 0.5);
            model_struct.name = "big_hero_six";
            break;
        case ModelType::heart:
            model_struct.model = heart;
            model_struct.model.setScale(0.5, 0.5, 0.5);
            model_struct.name = "heart";
            break;
        case ModelType::banana:
            model_struct.model = banana;
            model_struct.texture = banana_texture;
//            model_struct.model.setScale(0.5, 0.5, 0.5);
            model_struct.name = "banana";
            break;
        case ModelType::soccer:
            model_struct.model = soccer;
            model_struct.position1[1] = model_struct.position1[1] + 100.0f;
            model_struct.model.setScale(0.5, 0.5, 0.5);
            model_struct.name = "football";
            break;
        default:
            break;
    }
//    model_struct.box = get_bounding_box(model_struct.model);
    model_struct.model.setPosition(model_struct.position1[0], model_struct.position1[1], model_struct.position1[2]);
    
    // Keep number of models lower than set max count
    if (model_vector.size() == max_count_3d_models) {
        model_vector.at(head_3d_models) = model_struct;
    } else {
        model_vector.push_back(model_struct);
    }
    
    ofLog() << "<new model at index: " << head_3d_models << ">";
    
    head_3d_models = ++head_3d_models >= max_count_3d_models ? 0: head_3d_models; // restart at 0 if max count reached

}

std::vector<ofPath> Renderer::get_bounding_box(ofMesh &mesh)
{
    std::vector<ofPath> sides;
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::min();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::min();
    float min_z = std::numeric_limits<float>::max();
    float max_z = std::numeric_limits<float>::min();

    for (int i = 0 ; i < mesh.getVertices().size() ; i++)
    {
        ofVec3f vertex = mesh.getVertex(i);
        min_x = min(min_x, vertex.x);
        max_x = max(max_x, vertex.x);
        min_y = min(min_y, vertex.y);
        max_y = max(max_y, vertex.y);
        min_z = min(min_z, vertex.z);
        max_z = max(max_z, vertex.z);
    }

    ofPath rect1;
    rect1.moveTo(min_x, min_y, min_z);
    rect1.lineTo(min_x, max_y, min_z);
    rect1.lineTo(max_x, max_y, min_z);
    rect1.moveTo(max_x, min_y, min_z);
    rect1.close();
    rect1.setFilled(false);
    rect1.setStrokeColor(ofColor(211, 50));
    rect1.setStrokeWidth(1.0f);
    sides.push_back(rect1);
    
    ofPath rect2;
    rect2.moveTo(min_x, min_y, max_z);
    rect2.lineTo(min_x, max_y, max_z);
    rect2.lineTo(max_x, max_y, max_z);
    rect2.moveTo(max_x, min_y, max_z);
    rect2.close();
    rect2.setFilled(false);
    rect2.setStrokeColor(ofColor(211, 50));
    rect2.setStrokeWidth(1.0f);
    sides.push_back(rect2);
    
    ofPath rect3;
    rect3.moveTo(min_x, min_y, min_z);
    rect3.lineTo(min_x, min_y, max_z);
    rect3.lineTo(min_x, max_y, max_z);
    rect3.moveTo(min_x, max_y, min_z);
    rect3.close();
    rect3.setFilled(false);
    rect3.setStrokeColor(ofColor(211, 50));
    rect3.setStrokeWidth(1.0f);
    sides.push_back(rect3);
    
    ofPath rect4;
    rect4.moveTo(max_x, min_y, min_z);
    rect4.lineTo(max_x, min_y, max_z);
    rect4.lineTo(max_x, max_y, max_z);
    rect4.moveTo(max_x, max_y, min_z);
    rect4.close();
    rect4.setFilled(false);
    rect4.setStrokeColor(ofColor(211, 50));
    rect4.setStrokeWidth(1.0f);
    sides.push_back(rect4);
    
    ofPath rect5;
    rect5.moveTo(min_x, min_y, min_z);
    rect5.lineTo(max_x, min_y, min_z);
    rect5.lineTo(max_x, min_y, max_z);
    rect5.moveTo(min_x, min_y, max_z);
    rect5.close();
    rect5.setFilled(false);
    rect5.setStrokeColor(ofColor(211, 50));
    rect5.setStrokeWidth(1.0f);
    sides.push_back(rect5);
    
    ofPath rect6;
    rect6.moveTo(min_x, max_y, min_z);
    rect6.lineTo(max_x, max_y, min_z);
    rect6.lineTo(max_x, max_y, max_z);
    rect6.moveTo(min_x, max_y, max_z);
    rect6.close();
    rect6.setFilled(false);
    rect6.setStrokeColor(ofColor(211, 50));
    rect6.setStrokeWidth(1.0f);
    sides.push_back(rect6);
    
    return sides;
}

std::vector<ofPath> Renderer::get_bounding_box(ofxAssimpModelLoader &model)
{
    std::vector<ofPath> sides;
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::min();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::min();
    float min_z = std::numeric_limits<float>::max();
    float max_z = std::numeric_limits<float>::min();
    
    for (int j = 0 ; j < model.getMeshCount() ; j++)
    {
        for (int i = 0 ; i < model.getMesh(j).getVertices().size() ; i++)
        {
            ofVec3f vertex = model.getMesh(j).getVertex(i);
            min_x = min(min_x, vertex.x);
            max_x = max(max_x, vertex.x);
            min_y = min(min_y, vertex.y);
            max_y = max(max_y, vertex.y);
            min_z = min(min_z, vertex.z);
            max_z = max(max_z, vertex.z);
        }
    }
    
    ofPath rect1;
    rect1.moveTo(min_x, min_y, min_z);
    rect1.lineTo(min_x, max_y, min_z);
    rect1.lineTo(max_x, max_y, min_z);
    rect1.moveTo(max_x, min_y, min_z);
    rect1.close();
    rect1.setFilled(false);
    rect1.setStrokeColor(ofColor(211, 50));
    rect1.setStrokeWidth(1.0f);
    sides.push_back(rect1);
    
    ofPath rect2;
    rect2.moveTo(min_x, min_y, max_z);
    rect2.lineTo(min_x, max_y, max_z);
    rect2.lineTo(max_x, max_y, max_z);
    rect2.moveTo(max_x, min_y, max_z);
    rect2.close();
    rect2.setFilled(false);
    rect2.setStrokeColor(ofColor(211, 50));
    rect2.setStrokeWidth(1.0f);
    sides.push_back(rect2);
    
    ofPath rect3;
    rect3.moveTo(min_x, min_y, min_z);
    rect3.lineTo(min_x, min_y, max_z);
    rect3.lineTo(min_x, max_y, max_z);
    rect3.moveTo(min_x, max_y, min_z);
    rect3.close();
    rect3.setFilled(false);
    rect3.setStrokeColor(ofColor(211, 50));
    rect3.setStrokeWidth(1.0f);
    sides.push_back(rect3);
    
    ofPath rect4;
    rect4.moveTo(max_x, min_y, min_z);
    rect4.lineTo(max_x, min_y, max_z);
    rect4.lineTo(max_x, max_y, max_z);
    rect4.moveTo(max_x, max_y, min_z);
    rect4.close();
    rect4.setFilled(false);
    rect4.setStrokeColor(ofColor(211, 50));
    rect4.setStrokeWidth(1.0f);
    sides.push_back(rect4);
    
    ofPath rect5;
    rect5.moveTo(min_x, min_y, min_z);
    rect5.lineTo(max_x, min_y, min_z);
    rect5.lineTo(max_x, min_y, max_z);
    rect5.moveTo(min_x, min_y, max_z);
    rect5.close();
    rect5.setFilled(false);
    rect5.setStrokeColor(ofColor(211, 50));
    rect5.setStrokeWidth(1.0f);
    sides.push_back(rect5);
    
    ofPath rect6;
    rect6.moveTo(min_x, max_y, min_z);
    rect6.lineTo(max_x, max_y, min_z);
    rect6.lineTo(max_x, max_y, max_z);
    rect6.moveTo(min_x, max_y, max_z);
    rect6.close();
    rect6.setFilled(false);
    rect6.setStrokeColor(ofColor(211, 50));
    rect6.setStrokeWidth(1.0f);
    sides.push_back(rect6);
    
    return sides;
}

// fonction d'oscillation
float Renderer::oscillate(float time, float frequency, float amplitude)
{
    return sinf(time * 2.0f * PI / frequency) * amplitude;
}


