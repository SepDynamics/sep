#include "renderer.h"

#include <cmath>
#include <iostream>

// Initialize static member variable
bool sep::workbench::Renderer::startButtonClicked = false;

void sep::workbench::Renderer::init(int width, int height)
{
    this->width = width;
    this->height = height;

    std::cout << "Renderer initialized with dimensions: " << width << "x" << height << std::endl;

    // Default to fallback mode until we confirm OpenGL works
    useOpenGL = false;

    try
    {
        // Get OpenGL version info for debugging
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);

        if (version && vendor && renderer)
        {
            std::cout << "OpenGL Version: " << version << std::endl;
            std::cout << "OpenGL Vendor: " << vendor << std::endl;
            std::cout << "OpenGL Renderer: " << renderer << std::endl;
        }
        else
        {
            std::cerr << "Warning: Unable to get OpenGL version info" << std::endl;
        }

        // Set a bright clear color to make it obvious if rendering is working
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up viewport
        glViewport(0, 0, width, height);

        // Create and bind a vertex array object (VAO)
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create and bind a vertex buffer object (VBO)
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Define the vertices for a simple triangle
        float vertices[] = {
            // Positions         // Colors
            0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Top vertex (red)
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Bottom left (green)
            0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // Bottom right (blue)
        };

        // Upload the vertex data to the GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Setup shaders
        setupShaders();

        // Enable vertex attributes
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO
        glBindVertexArray(0);

        // Test if basic OpenGL functions work by clearing the screen
        glClear(GL_COLOR_BUFFER_BIT);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error during initialization: " << err << std::endl;
            throw std::runtime_error("OpenGL error during initialization");
        }

        // Set flag to indicate successful initialization
        useOpenGL = true;
        std::cout << "OpenGL renderer initialized successfully" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during renderer initialization: " << e.what() << std::endl;
        std::cerr << "Fallback mode enabled - will attempt basic rendering" << std::endl;
        useOpenGL = false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during renderer initialization - fallback mode enabled"
                  << std::endl;
        useOpenGL = false;
    }
}

void sep::workbench::Renderer::setupShaders()
{
    std::cout << "Setting up shaders..." << std::endl;

    // Vertex shader source code
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;
        out vec4 vertexColor;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    // Fragment shader source code
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec4 vertexColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = vertexColor;
        }
    )";

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    // Create shader program and link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    // Delete shaders as they're linked into the program and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Shader program created with ID: " << shaderProgram << std::endl;
}

void sep::workbench::Renderer::render()
{
    // In fallback mode, print a message only once but still try to render
    static bool first_time = true;
    if (!useOpenGL && first_time)
    {
        std::cout << "Using fallback rendering mode - attempting to draw basic shapes" << std::endl;
        first_time = false;
    }

    try
    {
        // Clear the screen with a bright color
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (useOpenGL && shaderProgram != 0 && vao != 0)
        {
            // Use our shader program
            glUseProgram(shaderProgram);

            // Bind the VAO
            glBindVertexArray(vao);

            // Draw the triangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Unbind VAO and shader program
            glBindVertexArray(0);
            glUseProgram(0);

            // Log success on first frame
            static bool success_logged = false;
            if (!success_logged)
            {
                std::cout << "Successfully rendered triangle with modern OpenGL!" << std::endl;
                success_logged = true;
            }
        }
        else
        {
            // Fallback to fixed-function pipeline if shader program or VAO failed
            // Set up orthographic projection for 2D rendering
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

            // Draw a demo pattern in the center
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Draw a bright red square in the center
            glColor4f(1.0f, 0.2f, 0.2f, 0.9f);
            glBegin(GL_QUADS);
            glVertex2f(width / 2 - 150, height / 2 - 150);
            glVertex2f(width / 2 + 150, height / 2 - 150);
            glVertex2f(width / 2 + 150, height / 2 + 150);
            glVertex2f(width / 2 - 150, height / 2 + 150);
            glEnd();

            // Draw a yellow border around the square
            glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
            glLineWidth(5.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(width / 2 - 150, height / 2 - 150);
            glVertex2f(width / 2 + 150, height / 2 - 150);
            glVertex2f(width / 2 + 150, height / 2 + 150);
            glVertex2f(width / 2 - 150, height / 2 + 150);
            glEnd();

            // Draw a white X across the square
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glLineWidth(3.0f);
            glBegin(GL_LINES);
            glVertex2f(width / 2 - 150, height / 2 - 150);
            glVertex2f(width / 2 + 150, height / 2 + 150);
            glVertex2f(width / 2 - 150, height / 2 + 150);
            glVertex2f(width / 2 + 150, height / 2 - 150);
            glEnd();

            // Draw a "START" text above the square
            // Switch to orthographic projection for text rendering
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, width, 0, height, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            // Draw text background for better visibility
            glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
            glBegin(GL_QUADS);
            glVertex2f(width / 2 - 100, height / 2 + 170);
            glVertex2f(width / 2 + 100, height / 2 + 170);
            glVertex2f(width / 2 + 100, height / 2 + 220);
            glVertex2f(width / 2 - 100, height / 2 + 220);
            glEnd();

            // Draw "START" text using simple line segments
            glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            glLineWidth(4.0f);

            // Draw "S"
            glBegin(GL_LINE_STRIP);
            glVertex2f(width / 2 - 80, height / 2 + 210);
            glVertex2f(width / 2 - 60, height / 2 + 220);
            glVertex2f(width / 2 - 40, height / 2 + 210);
            glVertex2f(width / 2 - 60, height / 2 + 200);
            glVertex2f(width / 2 - 40, height / 2 + 190);
            glVertex2f(width / 2 - 60, height / 2 + 180);
            glVertex2f(width / 2 - 80, height / 2 + 190);
            glEnd();

            // Draw "T"
            glBegin(GL_LINES);
            glVertex2f(width / 2 - 20, height / 2 + 220);
            glVertex2f(width / 2 - 20, height / 2 + 180);
            glVertex2f(width / 2 - 35, height / 2 + 220);
            glVertex2f(width / 2 - 5, height / 2 + 220);
            glEnd();

            // Draw "A"
            glBegin(GL_LINE_STRIP);
            glVertex2f(width / 2 + 0, height / 2 + 180);
            glVertex2f(width / 2 + 15, height / 2 + 220);
            glVertex2f(width / 2 + 30, height / 2 + 180);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(width / 2 + 7, height / 2 + 200);
            glVertex2f(width / 2 + 23, height / 2 + 200);
            glEnd();

            // Draw "R"
            glBegin(GL_LINE_STRIP);
            glVertex2f(width / 2 + 40, height / 2 + 180);
            glVertex2f(width / 2 + 40, height / 2 + 220);
            glVertex2f(width / 2 + 60, height / 2 + 220);
            glVertex2f(width / 2 + 70, height / 2 + 210);
            glVertex2f(width / 2 + 60, height / 2 + 200);
            glVertex2f(width / 2 + 40, height / 2 + 200);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(width / 2 + 60, height / 2 + 200);
            glVertex2f(width / 2 + 70, height / 2 + 180);
            glEnd();

            // Draw "T"
            glBegin(GL_LINES);
            glVertex2f(width / 2 + 80, height / 2 + 220);
            glVertex2f(width / 2 + 80, height / 2 + 180);
            glVertex2f(width / 2 + 65, height / 2 + 220);
            glVertex2f(width / 2 + 95, height / 2 + 220);
            glEnd();

            // Restore matrices
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            // Log fallback on first frame
            static bool fallback_logged = false;
            if (!fallback_logged)
            {
                std::cout << "Using fallback rendering with START button" << std::endl;
                std::cout << "Using fallback rendering path with fixed-function pipeline"
                          << std::endl;
                fallback_logged = true;
            }
        }

        // Check for OpenGL errors
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error during rendering: " << err << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        useOpenGL = false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during rendering - falling back to console mode" << std::endl;
        useOpenGL = false;
    }
}

void sep::workbench::Renderer::render(const std::vector<sep::workbench::Pattern>& patterns)
{
    // Fallback to console rendering for patterns, but still try to render
    static bool first_time = true;
    if (!useOpenGL && first_time)
    {
        std::cout << "Rendering " << patterns.size() << " patterns (fallback mode)" << std::endl;
        std::cout << "Attempting basic OpenGL rendering despite initialization issues" << std::endl;
        first_time = false;
    }

    // Even in fallback mode, we'll try to draw something visible
    try
    {
        // Clear the screen with a very bright color to make it obvious if rendering is working
        glClearColor(0.7f, 0.1f, 0.1f, 1.0f);  // Even brighter red background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing for 3D rendering
        glEnable(GL_DEPTH_TEST);

        // Set up perspective projection for 3D rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = (float)width / (float)height;
        gluPerspective(45.0f, aspect, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Position the camera
        gluLookAt(0.0f, 2.0f, 5.0f,  // Eye position
                  0.0f, 0.0f, 0.0f,  // Look at position
                  0.0f, 1.0f, 0.0f   // Up vector
        );

        // Add ambient light - even higher brightness for better visibility
        GLfloat ambientLight[] = {0.9f, 0.9f, 0.9f, 1.0f};  // Increased ambient light
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

        // Add a directional light - maximum brightness
        GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Maximum brightness
        GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Enable lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        // If we have patterns, draw them as 3D objects
        if (!patterns.empty())
        {
            // Calculate layout parameters
            float spacing = 1.5f;
            int patternsPerRow = std::sqrt(patterns.size()) + 1;
            float startX = -((patternsPerRow - 1) * spacing) / 2.0f;
            float startZ = -((patternsPerRow - 1) * spacing) / 2.0f;

            // Draw patterns as 3D objects
            for (size_t i = 0; i < patterns.size() && i < 100; i++)
            {
                const auto& pattern = patterns[i];

                // Calculate position in grid
                int row = i / patternsPerRow;
                int col = i % patternsPerRow;

                // Use pattern's position if available, otherwise use grid layout
                float x =
                    pattern.position.x != 0.0f ? pattern.position.x : (startX + col * spacing);
                float y = pattern.position.y != 0.0f ? pattern.position.y : 0.0f;
                float z =
                    pattern.position.z != 0.0f ? pattern.position.z : (startZ + row * spacing);

                // Get pattern properties for visualization
                float coherence = pattern.quantum_state.coherence;
                float stability = pattern.quantum_state.stability;
                // Make objects even larger for better visibility
                float scale = pattern.scale > 0.0f
                                  ? pattern.scale * 2.5f
                                  : (0.8f + coherence * 1.0f);  // Much larger scale

                // Set color based on pattern properties - extremely vibrant colors with higher base
                // values
                glColor4f(pattern.color.r > 0.0f ? pattern.color.r : (0.9f + coherence * 0.2f),
                          pattern.color.g > 0.0f ? pattern.color.g : (0.8f + stability * 0.3f),
                          pattern.color.b > 0.0f ? pattern.color.b
                                                 : (1.0f + (coherence * stability) * 0.1f),
                          1.0f);  // Full opacity

                // Draw the pattern as a 3D object
                glPushMatrix();
                glTranslatef(x, y, z);
                glScalef(scale, scale, scale);

                // Choose shape based on memory tier or other properties
                int shape = pattern.memory_tier;

                switch (shape)
                {
                    case 0:  // STM - Sphere
                        renderSphere(16, 16);
                        break;
                    case 1:  // MTM - Cube
                        renderCube();
                        break;
                    case 2:  // LTM - Octahedron
                        renderOctahedron();
                        break;
                    default:  // Default - Tetrahedron
                        renderTetrahedron();
                        break;
                }

                glPopMatrix();

                // Draw connections between related patterns (if implemented)
                // This would require additional data about pattern relationships
            }
        }
        else
        {
            // If no patterns, draw multiple placeholder objects with bright colors
            // First object - extra large bright yellow cube
            glColor4f(1.0f, 1.0f, 0.0f, 1.0f);  // Bright yellow
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f);
            glScalef(2.5f, 2.5f, 2.5f);  // Even larger scale
            renderCube();
            glPopMatrix();

            // Second object - bright cyan sphere
            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);  // Bright cyan
            glPushMatrix();
            glTranslatef(2.0f, 0.0f, 0.0f);
            glScalef(2.2f, 2.2f, 2.2f);  // Even larger sphere
            renderSphere(16, 16);
            glPopMatrix();

            // Third object - bright magenta tetrahedron
            glColor4f(1.0f, 0.0f, 1.0f, 1.0f);  // Bright magenta
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, 2.0f);
            glScalef(2.3f, 2.3f, 2.3f);  // Even larger tetrahedron
            renderTetrahedron();
            glPopMatrix();
        }

        // Disable lighting and depth testing when done
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Check for OpenGL errors
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error during pattern rendering: " << err << std::endl;
        }
        else
        {
            // Log successful rendering much more frequently
            static int success_count = 0;
            if (success_count++ % 15 == 0)
            {  // Log more frequently - every 15 frames
                std::cout << "Successfully rendered " << patterns.size() << " patterns with "
                          << "memory tiers (STM=spheres, MTM=cubes, LTM=octahedrons)" << std::endl;

                // Log pattern statistics if available
                if (!patterns.empty())
                {
                    float avg_coherence = calculateAvgCoherence(patterns);
                    std::cout << "  Average coherence: " << avg_coherence
                              << " | Frame count: " << success_count << std::endl;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during pattern rendering: " << e.what() << std::endl;
        useOpenGL = false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during pattern rendering" << std::endl;
        useOpenGL = false;
    }
}

// Helper method to render a sphere
void sep::workbench::Renderer::renderSphere(int latitudes, int longitudes)
{
    float radius = 0.5f;

    for (int i = 0; i < latitudes; i++)
    {
        float lat0 = M_PI * (-0.5f + (float)(i) / latitudes);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / latitudes);
        float y0 = radius * sin(lat0);
        float y1 = radius * sin(lat1);
        float r0 = radius * cos(lat0);
        float r1 = radius * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= longitudes; j++)
        {
            float lng = 2 * M_PI * (float)(j) / longitudes;
            float x = cos(lng);
            float z = sin(lng);

            // Calculate normal for lighting
            float nx = x;
            float ny = sin(lat1);
            float nz = z;
            float norm = sqrt(nx * nx + ny * ny + nz * nz);
            glNormal3f(nx / norm, ny / norm, nz / norm);

            glVertex3f(r1 * x, y1, r1 * z);

            // Calculate normal for lighting
            nx = x;
            ny = sin(lat0);
            nz = z;
            norm = sqrt(nx * nx + ny * ny + nz * nz);
            glNormal3f(nx / norm, ny / norm, nz / norm);

            glVertex3f(r0 * x, y0, r0 * z);
        }
        glEnd();
    }
}

// Helper method to render a cube
void sep::workbench::Renderer::renderCube()
{
    float size = 0.5f;

    // Define the 8 vertices of the cube
    float vertices[8][3] = {{-size, -size, -size}, {size, -size, -size}, {size, size, -size},
                            {-size, size, -size},  {-size, -size, size}, {size, -size, size},
                            {size, size, size},    {-size, size, size}};

    // Define the 6 faces of the cube (each face is a quad with 4 vertices)
    int faces[6][4] = {
        {0, 1, 2, 3},  // Back face
        {4, 5, 6, 7},  // Front face
        {0, 4, 7, 3},  // Left face
        {1, 5, 6, 2},  // Right face
        {0, 1, 5, 4},  // Bottom face
        {3, 2, 6, 7}   // Top face
    };

    // Define normals for each face
    float normals[6][3] = {{0, 0, -1}, {0, 0, 1}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}};

    // Draw each face
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++)
    {
        glNormal3fv(normals[i]);
        for (int j = 0; j < 4; j++)
        {
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();
}

// Helper method to render a tetrahedron
void sep::workbench::Renderer::renderTetrahedron()
{
    float size = 0.6f;

    // Define the 4 vertices of the tetrahedron
    float vertices[4][3] = {
        {0, size, 0},           // Top
        {-size, -size, -size},  // Back left
        {size, -size, -size},   // Back right
        {0, -size, size}        // Front
    };

    // Define the 4 faces of the tetrahedron (each face is a triangle with 3 vertices)
    int faces[4][3] = {
        {0, 1, 2},  // Back face
        {0, 2, 3},  // Right face
        {0, 3, 1},  // Left face
        {1, 3, 2}   // Bottom face
    };

    // Draw each face
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 4; i++)
    {
        // Calculate face normal for lighting
        float v1[3] = {vertices[faces[i][1]][0] - vertices[faces[i][0]][0],
                       vertices[faces[i][1]][1] - vertices[faces[i][0]][1],
                       vertices[faces[i][1]][2] - vertices[faces[i][0]][2]};

        float v2[3] = {vertices[faces[i][2]][0] - vertices[faces[i][0]][0],
                       vertices[faces[i][2]][1] - vertices[faces[i][0]][1],
                       vertices[faces[i][2]][2] - vertices[faces[i][0]][2]};

        // Cross product to get normal
        float normal[3] = {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
                           v1[0] * v2[1] - v1[1] * v2[0]};

        // Normalize
        float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;

        glNormal3fv(normal);

        for (int j = 0; j < 3; j++)
        {
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();
}

// Helper method to render an octahedron
void sep::workbench::Renderer::renderOctahedron()
{
    float size = 0.5f;

    // Define the 6 vertices of the octahedron
    float vertices[6][3] = {
        {0, size, 0},   // Top
        {0, -size, 0},  // Bottom
        {size, 0, 0},   // Right
        {-size, 0, 0},  // Left
        {0, 0, size},   // Front
        {0, 0, -size}   // Back
    };

    // Define the 8 faces of the octahedron (each face is a triangle with 3 vertices)
    int faces[8][3] = {
        {0, 2, 4}, {0, 4, 3}, {0, 3, 5}, {0, 5, 2},  // Top faces
        {1, 4, 2}, {1, 3, 4}, {1, 5, 3}, {1, 2, 5}   // Bottom faces
    };

    // Draw each face
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 8; i++)
    {
        // Calculate face normal for lighting
        float v1[3] = {vertices[faces[i][1]][0] - vertices[faces[i][0]][0],
                       vertices[faces[i][1]][1] - vertices[faces[i][0]][1],
                       vertices[faces[i][1]][2] - vertices[faces[i][0]][2]};

        float v2[3] = {vertices[faces[i][2]][0] - vertices[faces[i][0]][0],
                       vertices[faces[i][2]][1] - vertices[faces[i][0]][1],
                       vertices[faces[i][2]][2] - vertices[faces[i][0]][2]};

        // Cross product to get normal
        float normal[3] = {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
                           v1[0] * v2[1] - v1[1] * v2[0]};

        // Normalize
        float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;

        glNormal3fv(normal);

        for (int j = 0; j < 3; j++)
        {
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();
}

// Non-const version of render that delegates to the const version
sep::workbench::Renderer::~Renderer()
{
    // Call cleanup to release OpenGL resources
    cleanup();
}

void sep::workbench::Renderer::cleanup()
{
    // Only cleanup if OpenGL was successfully initialized
    if (useOpenGL)
    {
        std::cout << "Cleaning up OpenGL resources..." << std::endl;

        // Delete shader program
        if (shaderProgram != 0)
        {
            glDeleteProgram(shaderProgram);
            shaderProgram = 0;
        }

        // Delete VAO
        if (vao != 0)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        // Delete VBO
        if (vbo != 0)
        {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        std::cout << "OpenGL resources cleaned up successfully." << std::endl;
    }
}

// Check if a point is inside the START button area
bool sep::workbench::Renderer::isPointInStartButton(double x, double y) const
{
    // The START text is drawn above the square
    // The square is at (width/2 - 150, height/2 - 150) to (width/2 + 150, height/2 + 150)
    // The START text is at (width/2 - 100, height/2 + 170) to (width/2 + 100, height/2 + 220)

    // Convert screen coordinates to our coordinate system
    // In OpenGL, y=0 is at the bottom, but in screen coordinates, y=0 is at the top
    y = height - y;

    // Check if the point is within the START text area
    return (x >= width / 2 - 100 && x <= width / 2 + 100 && y >= height / 2 + 170 &&
            y <= height / 2 + 220);
}

// Handle mouse click
void sep::workbench::Renderer::handleMouseClick(double x, double y)
{
    if (isPointInStartButton(x, y) && demoManager != nullptr)
    {
        // Instead of casting to a specific type, we'll use a simpler approach
        // The main.cpp file will handle the actual switching to the genesis demo
        std::cout << "START button clicked, requesting switch to genesis demo" << std::endl;

        // Signal that the START button was clicked
        // The main application can check this and take appropriate action
        startButtonClicked = true;
    }
}

// Static method implementations
bool sep::workbench::Renderer::wasStartButtonClicked() { return startButtonClicked; }

void sep::workbench::Renderer::resetStartButtonClicked() { startButtonClicked = false; }

void sep::workbench::Renderer::render(std::vector<sep::workbench::Pattern>& patterns)
{
    // Call the const version to avoid code duplication
    render(static_cast<const std::vector<sep::workbench::Pattern>&>(patterns));

    // Additional logging for debugging
    static bool first_time = true;
    if (first_time)
    {
        std::cout << "Patterns: " << patterns.size()
                  << " | Avg Coherence: " << calculateAvgCoherence(patterns) << std::endl;
        first_time = false;
    }
}

float sep::workbench::Renderer::calculateAvgCoherence(const std::vector<sep::workbench::Pattern>& patterns)
{
    if (patterns.empty()) return 0.0f;

    float total = 0.0f;
    for (const auto& pattern : patterns)
    {
        total += pattern.quantum_state.coherence;
    }
    return total / patterns.size();
}
