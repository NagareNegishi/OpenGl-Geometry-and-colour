# 3D Model Loader and Renderer

A comprehensive 3D model loading and rendering system that parses Wavefront OBJ files and displays them with interactive color control and dynamic lighting. The implementation demonstrates fundamental computer graphics concepts including file parsing, OpenGL rendering, and shader programming.

## Features

### OBJ File Loading
- **Wavefront OBJ Parser**: Complete implementation supporting vertices, normals, and face data
- **Robust File Processing**: Line-by-line parsing with proper error handling
- **Index Management**: Automatic conversion between 1-based OBJ and 0-based OpenGL indexing
- **Memory Management**: Efficient storage and organization of mesh data

### Interactive Rendering
- **Real-Time Display**: Smooth 3D model rendering using OpenGL
- **Dynamic Color Control**: RGB color picker with slider and direct input support
- **Lighting System**: Directional lighting with interactive 3-axis control
- **Debug Output**: Console mesh data printing for development and verification

### User Interface
- **ImGui Integration**: Clean, responsive interface for all controls
- **File Path Input**: Configurable OBJ file loading with default teapot model
- **Visual Feedback**: Real-time updates reflecting parameter changes
- **Intuitive Controls**: Organized layout with clear parameter grouping

## Technical Implementation

### OBJ File Parser
- **Data Structures**: Separate storage for vertices, normals, and face indices
- **Helper Functions**: Dedicated parsers for face and vertex data processing
- **Format Compliance**: Full Wavefront OBJ specification support for triangular meshes
- **Memory Efficiency**: Optimized data reorganization for GPU rendering

### OpenGL Rendering Pipeline
- **Vertex Array Objects**: Efficient geometry data management
- **Buffer Organization**: Structured vertex and index buffer implementation
- **Shader Integration**: Custom vertex and fragment shaders for lighting
- **Resource Management**: Proper OpenGL object lifecycle handling

### Lighting Model
- **Directional Lighting**: Basic Lambertian diffuse lighting implementation
- **Normal-Based Shading**: Surface normal and light direction dot product calculation
- **Interactive Control**: Real-time light direction adjustment via GUI sliders
- **Shader Uniforms**: Dynamic parameter passing to GPU shaders

## Project Structure

```
work/
├── src/
│   ├── objfile.h/.cpp     # OBJ file parser implementation
│   ├── application.cpp    # Main rendering and GUI logic
│   └── ...               # Framework files
├── res/shaders/
│   ├── vertex.glsl       # Vertex shader with lighting
│   └── fragment.glsl     # Fragment shader with color control
└── Files_added_or_modified/  # Modified files for submission
```

## Core Functionality

### File Loading System
- **loadOBJ()**: Main parsing function with filepath input and boolean return
- **Data Clearing**: Automatic cleanup before loading new files
- **Error Handling**: Robust file opening and format validation
- **Progress Tracking**: Clear success/failure indication

### Rendering Pipeline
- **build()**: OpenGL buffer setup and vertex array configuration
- **draw()**: Efficient mesh rendering with indexed drawing
- **destroy()**: Proper resource cleanup and memory management
- **printMeshData()**: Debug output in original OBJ format

### GUI Controls
- **File Management**: Load, unload, and print mesh data buttons
- **Color Selection**: RGB sliders and direct value input
- **Light Direction**: Three-axis directional light control
- **Visual Updates**: Real-time parameter application

## Usage Instructions

1. **Load Model**: Use the default teapot path or specify custom OBJ file
2. **Adjust Color**: Modify RGB values using color picker controls
3. **Control Lighting**: Adjust X, Y, Z light direction components
4. **Debug Data**: Print mesh information to console for verification
5. **Manage Files**: Load different models or unload current geometry

## Implementation Details

### Parser Architecture
- **Line Processing**: Character-by-character file reading with type identification
- **Data Validation**: Format checking and error recovery
- **Index Conversion**: Seamless 1-based to 0-based index translation
- **Geometry Organization**: Efficient vertex and face data structuring

### Shader System
- **Uniform Variables**: Dynamic color and lighting parameter passing
- **Vertex Processing**: Position transformation and normal calculation
- **Fragment Shading**: Per-pixel color computation with lighting
- **Pipeline Integration**: Seamless CPU-GPU data communication

### Memory Management
- **C++ I/O Streaming**: Standard library file handling
- **Dynamic Arrays**: STL vectors for flexible data storage
- **OpenGL Buffers**: Efficient GPU memory utilization
- **Resource Lifecycle**: Proper allocation and deallocation

## Technical Notes

### File Format Support
- Triangular faces only (simplified implementation)
- Vertex positions and normals required
- Standard OBJ format compliance

### Performance Considerations
- Indexed rendering for memory efficiency
- Single-pass geometry processing
- Optimized buffer organization
