//
//  VertexBuffer.cpp
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/18/13.
//
//

#include "VertexBuffer.h"

#include "ccMacros.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/CCNotificationCenter.h"
#include "CCEventType.h"
#include "CCGL.h"

#include "cocoa/CCString.h"
#include <stdlib.h>

USING_NS_CC;

void VertexBuffer::AddQuad(const cocos2d::ccV3F_C4B_T2F_Quad& quad)
{
    m_quadsDirty = true;
    m_quads.push_back(quad);
    
    const size_t unusedIndices = m_indices.size() - m_usedIndices;
    if(unusedIndices == 0)
    {
        m_indices.push_back(QuadIndices());
        const size_t index = m_indices.size() - 1;
        QuadIndices& quadIndices = m_indices.back();
        
        const int indexLookup[] = {0, 1, 2, 3, 2 ,1};
        for(size_t i = 0; i < 6; i++)
        {
            quadIndices.indices[i] = index * 4 + indexLookup[i];
        }
        
        m_indicesDirty = true;
    }
    
    m_usedIndices++;
}

void VertexBuffer::Clear()
{
    //m_indices.clear();
    m_quads.clear();
    m_quadsDirty = true;
    m_usedIndices = 0;
}

void VertexBuffer::SetupVao()
{
    
    glGenBuffers(2, &m_buffersNames[0]);
    CHECK_GL_ERROR_DEBUG();
    MapBuffers();
}

void VertexBuffer::DrawAll(GLuint texture)
{
    Draw(0, m_quads.size(), texture);
}

size_t VertexBuffer::Quads() const
{
    return m_quads.size();
}

void VertexBuffer::Draw(int start, int count, GLuint texture)
{
    MapBuffers();
    
    ccGLBindTexture2D(texture);
    
#define kQuadSize sizeof(m_quads[0].bl)
    glBindBuffer(GL_ARRAY_BUFFER, m_buffersNames[0]);
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
    // vertices
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, vertices));
    
    // colors
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, colors));
    
    // tex coords
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffersNames[1]);
    
    glDrawElements(GL_TRIANGLES, (GLsizei)count*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*sizeof(QuadIndices)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CC_INCREMENT_GL_DRAWS(1);

}

void VertexBuffer::MapBuffers()
{
    if(!m_quadsDirty && !m_indicesDirty)
    {
        return;
    }
    // Avoid changing the element buffer for whatever VAO might be bound.
	ccGLBindVAO(0);
    
    if(m_quadsDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffersNames[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_quads[0]) * m_quads.size(), &m_quads[0], GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR_DEBUG();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_quadsDirty = false;
    }
    
    if(m_indicesDirty)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffersNames[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndices) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
        CHECK_GL_ERROR_DEBUG();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        m_indicesDirty = false;
    }
}
