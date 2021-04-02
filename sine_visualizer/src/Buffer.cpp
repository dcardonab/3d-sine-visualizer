#include "Buffer.h"
#include <iostream>


Buffer::Buffer(){
	std::cout << "(Buffer.cpp) Constructor Called\n";
}

Buffer::~Buffer(){
    glDeleteVertexArrays(1, &m_VAOid);
    glDeleteBuffers(1, &m_vertexPositionBuffer);
    glDeleteBuffers(1, &m_indexBufferObject);
}


void Buffer::Bind(){
    // Bind to our vertex array
    glBindVertexArray(m_VAOid);
    // Bind to our vertex information
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    // Bind to the elements we are drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}

// Note: Calling Unbind is rarely done, if you need
// to draw something else then just bind to new buffer.
void Buffer::Unbind(){
    // Bind to our vertex array
    glBindVertexArray(0);
    // Bind to our vertex information
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Bind to the elements we are drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Buffer::CreateBufferLayout(unsigned int stride, unsigned int vcount, unsigned int icount, float* vdata, unsigned int* idata ){
    std::cout << "Created new buffer layout.\n";

    m_stride = stride;
    
    static_assert(sizeof(GLfloat) == sizeof(float), "GLFloat and float are not the same size on this architecture");
    static_assert(sizeof(GLuint) == sizeof(unsigned int), "Gluint and unsigned int are not same size on this architecture");
    
    // Vertex Arrays Object
    glGenVertexArrays(1, &m_VAOid); 
    glBindVertexArray(m_VAOid);     // Select buffer by binding

    // Vertex Buffer Object (VBO)
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    // Attribute 0 has three float for position coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(  0,  // Attribute number, which matches
                                // layout in shader
                            3,  // Num components in attribute
                            GL_FLOAT, // Type of data
                            GL_FALSE, // Is the data normalized
                            sizeof(float)*m_stride, // Stride
                            0   // Offset for the attribute's
                                // corresponding data.
    );

    // Vertex Buffer Object (VBO) for the index buffer.
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);
}

void Buffer::CreateBufferTextureLayout(unsigned int stride,unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata ) {
    std::cout << "Created new texture buffer layout.\n";

    m_stride = stride;
    
    static_assert(sizeof(GLfloat) == sizeof(float), "GLFloat and gloat are not the same size on this architecture");
    static_assert(sizeof(GLuint) == sizeof(unsigned int), "Gluint and unsigned int are not same size on this architecture");
    
    // VertexArrays
    glGenVertexArrays(1, &m_VAOid);
    glBindVertexArray(m_VAOid);

    // Vertex Buffer Object (VBO)
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    // Three floats for position coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, 0);

    // Two floats for texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(float) * m_stride, (char*)(sizeof(float) * 3));


    // Vertex Buffer Object (VBO) for the index buffer.
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount*sizeof(unsigned int), idata,GL_STATIC_DRAW);
}

// A normal map layout needs the following attributes
//      positions
//      normals
//      texcoords
//      tangent
//      bitangent
void Buffer::CreateBufferNormalMapLayout(unsigned int stride,unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata ){
    std::cout << "Created new normal map buffer layout.\n";

    m_stride = stride;
    
    static_assert(sizeof(GLfloat) == sizeof(float), "GLFloat and gloat are not the same size on this architecture");
    static_assert(sizeof(GLuint) == sizeof(unsigned int), "Gluint and unsigned int are not same size on this architecture");
    
    // VertexArrays
    glGenVertexArrays(1, &m_VAOid);
    glBindVertexArray(m_VAOid);

    // Vertex Buffer Object (VBO)
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount*sizeof(float), vdata, GL_STATIC_DRAW);

    // Three floats for position coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*m_stride, 0);

    // Two floats for texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride,(char*)(sizeof(float) * 3));

    // Three floats for normal coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 5));

    // Three floats for tangent coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,sizeof(float)*m_stride, (char*)(sizeof(float) * 8));

    // Three floats for bi-tangent coordinates
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 11));
    
    // Vertex Buffer Object (VBO) for the index buffer.
    // Setup an index buffer
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);
}







