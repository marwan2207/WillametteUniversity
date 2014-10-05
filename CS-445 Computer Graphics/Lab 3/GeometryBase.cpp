// This is the base class for all the geometry classes: Cube, Disk, etc

#include "GeometryBase.h"

// ---------------------------------------------------------------------- default constructor
GeometryBase::GeometryBase() : mode(GL_TRIANGLES), numVertices(0)
{}

// ---------------------------------------------------------------------- copy constructor
GeometryBase::GeometryBase(const GeometryBase& other)
{
    numVertices = other.numVertices;
    for (int i=0; i < numVertices; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
        normals[i]=other.normals[i];
        tex_coords[i]=other.tex_coords[i];
    }
    vao = other.vao;
    mode = other.mode;
}

// ---------------------------------------------------------------------- assignment operator
GeometryBase&
GeometryBase::operator=(const GeometryBase& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
    numVertices = rhs.numVertices;
    for (int i=0; i < numVertices; i++)
    {
        points[i]=rhs.points[i];
        colors[i]=rhs.colors[i];
        normals[i]=rhs.normals[i];
        tex_coords[i]=rhs.tex_coords[i];
    }
    vao = rhs.vao;
    mode = rhs.mode;
}

// ---------------------------------------------------------------------- destructor
GeometryBase::~GeometryBase()
{
    if (numVertices > 0)
    {
        delete[] points;
        delete[] colors;
        delete[] normals;
        delete[] tex_coords;
    }
}


// ---------------------------------------------------------------------- createBuffers
// This is where all of the data is passed to buffers in the GPU.  Once we do this, we
//  don't really need to keep the points, colors, etc arrays around.
void
GeometryBase::createBuffers(GLint program )
{
    glGenVertexArrays( 1, &vao );  // generate index reference for vertex array buffer. Note, we need vao later to
    // draw the object so we make vao a member variable.
    glBindVertexArray( vao );       // bind the vertex array object
    GLuint buffer;                   // generate index reference for data buffer which will contain the points, colors, etc.
    // This buffer is contained in the vao so we don't need to save its reference.
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );  // bind the buffer object

    int csize =  numVertices * sizeof(*points) ;  // size of the points array
    // Note: points array is same size as normals and colors array
    int tsize = sizeof(*tex_coords) * numVertices; // size of the texture coordinates array

    glBufferData( GL_ARRAY_BUFFER, // target
                  3*csize + tsize,  // size
                  NULL,             // no data just yet (see below)
                  GL_STATIC_DRAW ); // usage

    glBufferSubData( GL_ARRAY_BUFFER,   //target
                     0,                  // offset in buffer where points data starts
                     csize,              // size of points array
                     *points );          // now we add the points data!
    glBufferSubData( GL_ARRAY_BUFFER,   //target
                     csize,              // offset in buffer where color data starts
                     csize,              // size of color array
                     *colors );          // now we add the color  data!
    glBufferSubData( GL_ARRAY_BUFFER, 2*csize, csize, *normals ); // see comments above
    glBufferSubData( GL_ARRAY_BUFFER, 3*csize, tsize, *tex_coords ); // see comments above

    // St up shader attribute variables.  "vPosition" is the name of the uniform variable as it appears in the
    //  vertex shader. E.g. see vertex.glsl
    GLint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition,       // index of the vertex attribute to be modified
                           4,                // size of the vertex attribute
                           GL_FLOAT,         // data type of each component of the array
                           GL_FALSE,         // Specifies whether fixed-point data values should be normalized (GL_TRUE)
                           0,                // stride: the byte offset between consecutive vertex attributes.
                           0 ); //  Specifies a pointer to the first component of the first vertex attribute in the array.

    GLint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,        // see glVertexAttribPointer comments above
                           (GLvoid*) csize );

    GLint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,       // see glVertexAttribPointer comments above
                           (GLvoid*) (2*csize) );

    GLint vTexture = glGetAttribLocation( program, "vTexture" );
    glEnableVertexAttribArray( vTexture );
    glVertexAttribPointer( vTexture, 2, GL_FLOAT, GL_FALSE, 0,      // see glVertexAttribPointer comments above
                           (GLvoid*) (3*csize) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );  // unbind buffer object
    glBindVertexArray( 0 );  // unbind vertex array object
}

// ---------------------------------------------------------------------- draw
void GeometryBase::draw()
{
    glBindVertexArray( vao );
    glDrawArrays( mode, 0, numVertices );
    glBindVertexArray( 0 );
}

// ---------------------------------------------------------------------- printArrays
void
GeometryBase::printArrays()
{
    cout << "points:\n";
    for (int i = 0; i < numVertices; i++)
    {
        cout<< i << ":  " <<  points[i] << "\n";
    }
    cout << "colors:\n";
    for (int i = 0; i < numVertices; i++)
    {
        cout<< i << ":  " <<  colors[i] << "\n";
    }
    cout << "normals:\n";
    for (int i = 0; i < numVertices; i++)
    {
        cout<< i << ":  " <<  normals[i] << "\n";
    }
    cout << "texture coords:\n";
    for (int i = 0; i < numVertices; i++)
    {
        cout<< i << ":  " <<  tex_coords[i] << "\t";
        if (i%3==2) cout << "\n";
    }
}






